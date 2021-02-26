/* Author: leosncz
Handles everything related to scenes and object rendering loop in scenes
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../display/display.h"
#include "../renderObject/renderObject.h"

#define CAMERA_FPS 0

class scene
{
public:
    void setupDisplay(display* customDisplay, int cameraType=CAMERA_FPS) // Set the display that will be used to render the scene
    {
        m_display = customDisplay;
        m_nbOfObjects = 0;
        m_nbOfLight = 0;

        if(cameraType == CAMERA_FPS)
        {
            cameraPos = glm::vec3(0.0f, 0.0f,  0.0f);
            cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            cameraUp =  glm::vec3(0.0f, 1.0f,  0.0f);
            cameraSpeed = 0.1f;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }

        // Setup ogl matrix
        projection = glm::perspective(glm::radians(45.0f), (float)((float)customDisplay->getDisWidth() / (float)customDisplay->getDisHeight()), 0.01f, 200.0f);
        view = glm::mat4(1.0);
        model = glm::mat4(1.0);

        id = rand();

        std::cout << "--> Creating new scene ID=" << id << std::endl;
        std::cout << "---> Scene ID=" << id << " camera is " << cameraType << std::endl;
    }

    void renderScene()
    {
        for(int i = 0; i<m_nbOfObjects;i++)
        {
            m_objectHolder[i]->render(&projection, &view, &model, cameraPos, lights,m_nbOfLight);
        }
    }

    void addDrawableObject(renderObject* object)
    {
        std::cout << "---> Adding new renderObject (ID=" << object->getID() << ") to scene ID=" << id << std::endl;
        m_objectHolder[m_nbOfObjects] = object;
        m_nbOfObjects++;
    }

    void moveScene(glm::vec3 position)
    {
        cameraPos = position;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void rotateScene(float angle, glm::vec3 factors)
    {
        view= glm::rotate(view,angle,factors);
    }

    void addLight(light* thelight)
    {
        lights[m_nbOfLight] = thelight;
        m_nbOfLight++;
    }

    void updateFPSCamera(bool isAzerty=true)
    {
        if(isAzerty)
        {
            if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_W ) == GLFW_PRESS){ cameraPos += cameraSpeed * cameraFront;}
            else if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_A ) == GLFW_PRESS){cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;}
            else if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_S ) == GLFW_PRESS){cameraPos -= cameraSpeed * cameraFront;}
            else if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_D ) == GLFW_PRESS){cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;}
        }
        else
        {
            if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_Z ) == GLFW_PRESS){ cameraPos += cameraSpeed * cameraFront;}
            else if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_Q ) == GLFW_PRESS){cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;}
            else if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_S ) == GLFW_PRESS){cameraPos -= cameraSpeed * cameraFront;}
            else if(glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_D ) == GLFW_PRESS){cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;}
        }
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    int getNbOfLight(){return m_nbOfLight;}

    ~scene()
    {
        std::cout << "--> Destroying scene ID=" << id << std::endl;
    }

private:

    int id;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;

    display* m_display;
    renderObject* m_objectHolder[1000]; // Max 1000 objects drawable
    int m_nbOfObjects;

    glm::mat4 projection, view, model;

    light* lights[7]; // 7 lights max
    int m_nbOfLight;
};

#endif // SCENE_H_INCLUDED
