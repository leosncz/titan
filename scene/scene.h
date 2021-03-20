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
#include "../texturePool/texturePool.h"
#include "../gui/gui.h"

class scene
{
public:
    void setupDisplay(display* customDisplay) // Set the display that will be used to render the scene
    {
        std::cout << "--> Creating new scene ID=" << id << std::endl;

        m_display = customDisplay;
        m_nbOfLight = 0;

        // Setup ogl matrix
        projection = glm::perspective(glm::radians(45.0f), (float)((float)customDisplay->getDisWidth() / (float)customDisplay->getDisHeight()), 0.01f, 200.0f);
        view = glm::mat4(1.0);
        model = glm::mat4(1.0);

        id = rand();

        m_actualCamera.init(m_display, &m_gui);

        m_gui.init(m_display);

        m_gamma = 2.2;
    }
    void renderScene()
    {
        // SHADOW PASSES
        if (m_nbOfLight > 0) {
            if (m_nbOfLight >= 1 && lights[0]->computeShadows) {

                glCullFace(GL_FRONT);
                glViewport(0, 0, 1024, 1024);
                glBindFramebuffer(GL_FRAMEBUFFER, lights[0]->depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);
                for (int i = 0; i < m_objectHolder.size(); i++)
                {
                    m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[0]);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glCullFace(GL_BACK);
            }
            if (m_nbOfLight >= 2 && lights[1]->computeShadows) {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, lights[1]->depthMapFBO);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[1]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
            }
            if (m_nbOfLight >= 3 && lights[2]->computeShadows) {

                glCullFace(GL_FRONT);
                glViewport(0, 0, 1024, 1024);
                glBindFramebuffer(GL_FRAMEBUFFER, lights[2]->depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);
                for (int i = 0; i < m_objectHolder.size(); i++)
                {
                    m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[2]);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glCullFace(GL_BACK);
            }
            if (m_nbOfLight >= 4 && lights[3]->computeShadows) {

                glCullFace(GL_FRONT);
                glViewport(0, 0, 1024, 1024);
                glBindFramebuffer(GL_FRAMEBUFFER, lights[3]->depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);
                for (int i = 0; i < m_objectHolder.size(); i++)
                {
                    m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[3]);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glCullFace(GL_BACK);
            }
            if (m_nbOfLight >= 5 && lights[4]->computeShadows) {

                glCullFace(GL_FRONT);
                glViewport(0, 0, 1024, 1024);
                glBindFramebuffer(GL_FRAMEBUFFER, lights[4]->depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);
                for (int i = 0; i < m_objectHolder.size(); i++)
                {
                    m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[4]);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glCullFace(GL_BACK);
            }
            if (m_nbOfLight >= 6 && lights[5]->computeShadows) {

                glCullFace(GL_FRONT);
                glViewport(0, 0, 1024, 1024);
                glBindFramebuffer(GL_FRAMEBUFFER, lights[5]->depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);
                for (int i = 0; i < m_objectHolder.size(); i++)
                {
                    m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[5]);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glCullFace(GL_BACK);
            }
            if (m_nbOfLight == 7 && lights[6]->computeShadows) {

                glCullFace(GL_FRONT);
                glViewport(0, 0, 1024, 1024);
                glBindFramebuffer(GL_FRAMEBUFFER, lights[6]->depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);
                for (int i = 0; i < m_objectHolder.size(); i++)
                {
                    m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[6]);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glCullFace(GL_BACK);
            }
        }
        
        //REGULAR PASSES, USING PREVIOUS GENERATED DEPTH MAPs
        glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i<m_objectHolder.size();i++)
        {
            m_objectHolder[i]->render(&projection, &view, &model, m_actualCamera.getCameraPos(), lights,m_nbOfLight,m_gamma);
        }

        //Then render the GUI
        m_gui.update();
    }

    void setGamma(float gamma) { m_gamma = gamma; }

    void addDrawableObject(renderObject* object)
    {
        std::cout << "---> Adding new renderObject (ID=" << object->getID() << ") to scene ID=" << id << std::endl;
        m_objectHolder.push_back(object);
    }

    void addLight(light* thelight)
    {
        lights.push_back(thelight);

        if (thelight->type == POINT_LIGHT)
        {
            glGenFramebuffers(1, &thelight->depthMapFBO);
            glGenTextures(1, &thelight->textureDepthMap);
            glBindTexture(GL_TEXTURE_CUBE_MAP, thelight->textureDepthMap);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glBindFramebuffer(GL_FRAMEBUFFER, thelight->depthMapFBO);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, thelight->textureDepthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else
        {
            glGenFramebuffers(1, &thelight->depthMapFBO);
            glGenTextures(1, &thelight->textureDepthMap);
            glBindTexture(GL_TEXTURE_2D, thelight->textureDepthMap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glBindFramebuffer(GL_FRAMEBUFFER, thelight->depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, thelight->textureDepthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        m_nbOfLight++;
    }

    void clearScene()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }

    void updateCamera(bool isAzerty=true)
    {
        m_actualCamera.update(m_display, &view, isAzerty);
    }

    void refreshScene()
    {
        glfwSwapBuffers(m_display->getGLFWWindow());
    }

    display* getDisplay() { return m_display; }

    int getNbOfLight(){return m_nbOfLight;}

    texturePool* getTexturePool(){ return &m_texturePool; }

    ~scene()
    {
        std::cout << "--> Destroying scene ID=" << id << std::endl;
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

private:

    int id;

    display* m_display;
    vector<renderObject*> m_objectHolder;

    glm::mat4 projection, view, model;

    vector<light*> lights; 
    int m_nbOfLight;

    cameraFPS m_actualCamera;

    texturePool m_texturePool; // All scenes have their own texture pool (for now)

    gui m_gui; // Each scene has its own gui

    //gamma
    float m_gamma;
};

#endif // SCENE_H_INCLUDED
