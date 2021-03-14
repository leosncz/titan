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

        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2024, 2024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depthMapFBO1);
        glGenTextures(1, &depthMap1);
        glBindTexture(GL_TEXTURE_2D, depthMap1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2024, 2024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO1);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap1, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depthMapFBO2);
        glGenTextures(1, &depthMap2);
        glBindTexture(GL_TEXTURE_2D, depthMap2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2024, 2024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO2);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap2, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depthMapFBO3);
        glGenTextures(1, &depthMap3);
        glBindTexture(GL_TEXTURE_2D, depthMap3);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2024, 2024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO3);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap3, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depthMapFBO4);
        glGenTextures(1, &depthMap4);
        glBindTexture(GL_TEXTURE_2D, depthMap4);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2024, 2024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO4);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap4, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depthMapFBO5);
        glGenTextures(1, &depthMap5);
        glBindTexture(GL_TEXTURE_2D, depthMap5);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2024, 2024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO5);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap5, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depthMapFBO6);
        glGenTextures(1, &depthMap6);
        glBindTexture(GL_TEXTURE_2D, depthMap6);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2024, 2024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO6);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap6, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void renderScene()
    {
        // SHADOW PASSES
        if (m_nbOfLight > 0) {
            if (m_nbOfLight >= 1) {
                if (lights[0]->type == DIRECTIONNAL_LIGHT && lights[0]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 2024, 2024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[0]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
            }
            if (m_nbOfLight >= 2) {
                if (lights[1]->type == DIRECTIONNAL_LIGHT && lights[1]->computeShadows) // we only compute shadows for directionnal lights
                { 
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 2024, 2024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO1);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[1]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
            }
            if (m_nbOfLight >= 3) {
                if (lights[2]->type == DIRECTIONNAL_LIGHT && lights[2]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 2024, 2024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO2);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[2]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
            }
            if (m_nbOfLight >= 4) {
                if (lights[3]->type == DIRECTIONNAL_LIGHT && lights[3]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 2024, 2024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO3);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[3]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
            }
            if (m_nbOfLight >= 5) {
                if (lights[4]->type == DIRECTIONNAL_LIGHT && lights[4]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 2024, 2024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO4);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[4]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
            }
            if (m_nbOfLight >= 6) {
                if (lights[5]->type == DIRECTIONNAL_LIGHT && lights[5]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 2024, 2024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO5);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[5]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
            }
            if (m_nbOfLight >= 7) {
                if (lights[6]->type == DIRECTIONNAL_LIGHT && lights[6]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 2024, 2024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO6);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[6]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
            }
        }
        
        //REGULAR PASSES, USING PREVIOUS GENERATED DEPTH MAP
        glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i<m_objectHolder.size();i++)
        {
            m_objectHolder[i]->render(&projection, &view, &model, m_actualCamera.getCameraPos(), lights,m_nbOfLight,depthMap,depthMap1,depthMap2,depthMap3,depthMap4, depthMap5, depthMap6);
        }

        //Then render the GUI
        m_gui.update();
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
        glDeleteTextures(1, &depthMap);
        glDeleteTextures(1, &depthMap1);
        glDeleteTextures(1, &depthMap2);
        glDeleteTextures(1, &depthMap3);
        glDeleteTextures(1, &depthMap4);
        glDeleteTextures(1, &depthMap5);
        glDeleteTextures(1, &depthMap6);
        glDeleteFramebuffers(1, &depthMapFBO);
        glDeleteFramebuffers(1, &depthMapFBO1);
        glDeleteFramebuffers(1, &depthMapFBO2);
        glDeleteFramebuffers(1, &depthMapFBO3);
        glDeleteFramebuffers(1, &depthMapFBO4);
        glDeleteFramebuffers(1, &depthMapFBO5);
        glDeleteFramebuffers(1, &depthMapFBO6);
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

private:

    int id;

    display* m_display;
    vector<renderObject*> m_objectHolder;

    glm::mat4 projection, view, model;

    light* lights[7]; // 7 lights max supported
    int m_nbOfLight;

    cameraFPS m_actualCamera;

    texturePool m_texturePool; // All scenes have their own texture pool (for now)

    GLuint depthMap, depthMapFBO, depthMap1, depthMap2, depthMap3, depthMap4, depthMap5, depthMap6, depthMapFBO1, depthMapFBO2, depthMapFBO3, depthMapFBO4, depthMapFBO5, depthMapFBO6; // Shadow stuff

    gui m_gui; // Each scene has its own gui
};

#endif // SCENE_H_INCLUDED
