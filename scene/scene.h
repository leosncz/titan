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

        // Generate buffers & textures

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO1);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap1, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenFramebuffers(1, &depthCubemapFBO);
        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024,1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthCubemapFBO1);
        glGenTextures(1, &depthCubemap1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap1);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO1);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap1, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthCubemapFBO2);
        glGenTextures(1, &depthCubemap2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap2);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO2);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap2, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthCubemapFBO3);
        glGenTextures(1, &depthCubemap3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap3);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO3);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap3, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthCubemapFBO4);
        glGenTextures(1, &depthCubemap4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap4);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO4);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap4, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthCubemapFBO5);
        glGenTextures(1, &depthCubemap5);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap5);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO5);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap5, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenFramebuffers(1, &depthCubemapFBO6);
        glGenTextures(1, &depthCubemap6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap6);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO6);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap6, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
       
    }
    void renderScene()
    {
        // SHADOW PASSES
        if (m_nbOfLight > 0) {
            if (m_nbOfLight >= 1) {
                if (lights[0]->type == DIRECTIONNAL_LIGHT && lights[0]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[0]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
                else if (lights[0]->type == POINT_LIGHT && lights[0]->computeShadows)
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024,1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO);
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
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO1);
                    glClear(GL_DEPTH_BUFFER_BIT);
                    for (int i = 0; i < m_objectHolder.size(); i++)
                    {
                        m_objectHolder[i]->renderDepth(&projection, &view, &model, lights[1]);
                    }
                    glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    glCullFace(GL_BACK);
                }
                else if (lights[1]->type == POINT_LIGHT && lights[1]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO1);
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
                if (lights[2]->type == POINT_LIGHT && lights[2]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO2);
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
                if (lights[3]->type == POINT_LIGHT && lights[3]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO3);
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
                if (lights[4]->type == POINT_LIGHT && lights[4]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO4);
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
                if (lights[5]->type == POINT_LIGHT && lights[5]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO5);
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
                if (lights[6]->type == POINT_LIGHT && lights[6]->computeShadows) // we only compute shadows for directionnal lights
                {
                    glCullFace(GL_FRONT);
                    glViewport(0, 0, 1024, 1024);
                    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFBO6);
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
        
        //REGULAR PASSES, USING PREVIOUS GENERATED DEPTH MAPs
        glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i<m_objectHolder.size();i++)
        {
            m_objectHolder[i]->render(&projection, &view, &model, m_actualCamera.getCameraPos(), lights,m_nbOfLight,depthMap,depthMap1,depthCubemap, depthCubemap1, depthCubemap2, depthCubemap3, depthCubemap4, depthCubemap5, depthCubemap6);
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
        glDeleteTextures(1, &depthCubemap);
        glDeleteTextures(1, &depthCubemap1);
        glDeleteTextures(1, &depthCubemap2);
        glDeleteTextures(1, &depthCubemap3);
        glDeleteTextures(1, &depthCubemap4);
        glDeleteTextures(1, &depthCubemap5);
        glDeleteTextures(1, &depthCubemap6);
        glDeleteFramebuffers(1, &depthMapFBO);
        glDeleteFramebuffers(1, &depthMapFBO1);
        glDeleteFramebuffers(1, &depthCubemapFBO);
        glDeleteFramebuffers(1, &depthCubemapFBO1);
        glDeleteFramebuffers(1, &depthCubemapFBO2);
        glDeleteFramebuffers(1, &depthCubemapFBO3);
        glDeleteFramebuffers(1, &depthCubemapFBO4);
        glDeleteFramebuffers(1, &depthCubemapFBO5);
        glDeleteFramebuffers(1, &depthCubemapFBO6);
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

    //Directionnal shadows
    GLuint depthMap, depthMapFBO, depthMap1, depthMapFBO1; // Shadow stuff

    //Point shadows
    GLuint depthCubemap, depthCubemapFBO, depthCubemap1, depthCubemap2, depthCubemap3, depthCubemap4, depthCubemap5, depthCubemap6, depthCubemapFBO1, depthCubemapFBO2, depthCubemapFBO3, depthCubemapFBO4, depthCubemapFBO5, depthCubemapFBO6;

    gui m_gui; // Each scene has its own gui
};

#endif // SCENE_H_INCLUDED
