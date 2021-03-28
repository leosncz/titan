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
#include "../shader/shader.h"

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

        //Create HDR stuff
        m_exposure = 1.0f;
        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrTexture, 0);
        glGenRenderbuffers(1, &hdrRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, hdrRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_display->getDisWidth(), m_display->getDisHeight());
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, hdrRBO);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //Create quad to render HDR scene
        m_hdrShader.compileQuadRenderShader();
        float vertices[] = {-1.0,-1.0,0.0,    1.0,-1.0,0.0,      1.0,1.0,0.0,     1.0,1.0,0.0,   -1.0,1.0,0.0,    -1.0,-1.0,0.0};
        float colors[] = {1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0};
        float texCoords[] = { 0.0,0.0,  1.0,0.0,   1.0,1.0,   1.0,1.0,   0.0,1.0,    0.0,0.0 };
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_colors);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
        glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), colors, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_texCoords);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
        glBufferData(GL_ARRAY_BUFFER, 2 *6 * sizeof(float), &texCoords[0], GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    }
    void renderScene()
    {
        // SHADOW PASSES
        for (int i = 0; i < lights.size(); i++)
        {
            if (lights[i]->computeShadows)
            {
                glCullFace(GL_FRONT);
                glViewport(0, 0, 1024, 1024);
                glBindFramebuffer(GL_FRAMEBUFFER, lights[i]->depthMapFBO);
                glClear(GL_DEPTH_BUFFER_BIT);
                for (int i2 = 0; i2 < m_objectHolder.size(); i2++)
                {
                    m_objectHolder[i2]->renderDepth(&projection, &view, &model, lights[i]);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glCullFace(GL_BACK);
            }
        }
        
        //Generate the final scene onto a quad
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int i = 0; i<m_objectHolder.size();i++)
        {
            m_objectHolder[i]->render(&projection, &view, &model, m_actualCamera.getCameraPos(), lights,m_nbOfLight);
        }
        //Then render the GUI
        m_gui.update(&m_objectHolder);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //Draw quad
        glUseProgram(m_hdrShader.getShaderID());
        glBindVertexArray(vao);
        glUniform1i(glGetUniformLocation(m_hdrShader.getShaderID(), "hdrTexture"), 15);
        glUniform1f(glGetUniformLocation(m_hdrShader.getShaderID(), "gamma"), m_gamma);
        glUniform1f(glGetUniformLocation(m_hdrShader.getShaderID(), "exposure"), m_exposure);
        glActiveTexture(GL_TEXTURE0+15);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void setGamma(float gamma) { m_gamma = gamma; }
    void setExposure(float exposure) { m_exposure = exposure; }

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
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
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
        m_actualCamera.update(m_display, &view, &isAzerty);
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
        glDeleteTextures(1, &hdrTexture);
        glDeleteRenderbuffers(1, &hdrRBO);
        glDeleteFramebuffers(1, &hdrFBO);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vbo_colors);
        glDeleteBuffers(1, &vbo_texCoords);
        glDeleteVertexArrays(1, &vao);
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

    //hdr
    GLuint hdrFBO, hdrTexture, hdrRBO;
    shader m_hdrShader;
    GLuint vbo, vbo_colors, vbo_texCoords, vao;
    float m_exposure;
};

#endif // SCENE_H_INCLUDED
