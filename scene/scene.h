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
#include <vector>
#include "../display/display.h"
#include "../renderObject/renderObject.h"
#include "../cameraFPS/cameraFPS.h"


class scene
{
public:
    void setupDisplay(display* customDisplay) // Set the display that will be used to render the scene
    {
        m_display = customDisplay;
        m_nbOfLight = 0;

        // Setup ogl matrix
        projection = glm::perspective(glm::radians(45.0f), (float)((float)customDisplay->getDisWidth() / (float)customDisplay->getDisHeight()), 0.01f, 200.0f);
        view = glm::mat4(1.0);
        model = glm::mat4(1.0);

        id = rand();

        m_actualCamera.init(m_display);

        std::cout << "--> Creating new scene ID=" << id << std::endl;
    }

    void renderScene()
    {
        for(int i = 0; i<m_objectHolder.size();i++)
        {
            m_objectHolder[i]->render(&projection, &view, &model, m_actualCamera.getCameraPos(), lights,m_nbOfLight);
        }
    }

    void addDrawableObject(renderObject* object)
    {
        std::cout << "---> Adding new renderObject (ID=" << object->getID() << ") to scene ID=" << id << std::endl;
        m_objectHolder.push_back(object);
    }

    void addLight(light* thelight)
    {
        lights[m_nbOfLight] = thelight;
        m_nbOfLight++;
    }

    void updateCamera(bool isAzerty=true)
    {
        m_actualCamera.update(m_display, &view, isAzerty);
    }

    int getNbOfLight(){return m_nbOfLight;}

    ~scene()
    {
        std::cout << "--> Destroying scene ID=" << id << std::endl;
    }

private:

    int id;

    display* m_display;
    vector<renderObject*> m_objectHolder;

    glm::mat4 projection, view, model;

    light* lights[7]; // 7 lights max
    int m_nbOfLight;

    cameraFPS m_actualCamera;

};

#endif // SCENE_H_INCLUDED
