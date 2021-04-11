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
#include "../shader/shader.h"

class scene
{
public:
    void init(display* customDisplay, camera* cam) // Set the display that will be used to render the scene
    {
        std::cout << "--> Creating new scene ID=" << id << std::endl;

        m_display = customDisplay;
        m_nbOfLight = 0;

        // Setup ogl matrix
        projection = glm::perspective(glm::radians(45.0f), (float)((float)customDisplay->getDisWidth() / (float)customDisplay->getDisHeight()), 0.01f, 200.0f);
        view = glm::mat4(1.0);
        model = glm::mat4(1.0);

        id = rand();

        m_actualCamera = cam;

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
        
        int32 swizzleMask[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ONE };
        m_deferedShader.compileDefaultShader();
        glGenFramebuffers(1, &gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

        // - normal color buffer
        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        // - color + specular color buffer
        glGenTextures(1, &gAlbedoSpec);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

        glGenTextures(1, &gRoughness);
        glBindTexture(GL_TEXTURE_2D, gRoughness);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gRoughness, 0);

        glGenTextures(1, &gMetallic);
        glBindTexture(GL_TEXTURE_2D, gMetallic);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gMetallic, 0);
       
        glGenRenderbuffers(1, &gRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, gRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_display->getDisWidth(), m_display->getDisHeight());
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBO);

        // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
        unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
        glDrawBuffers(5, attachments);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

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
        drawShadowPass();
        freeTexturesSlot();

        //Generate GBuffer
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        drawGBufferPass();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        freeTexturesSlot();

        //Generate the final scene onto a quad and calculate lighting using generated g buffer
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        drawDeferedQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        freeTexturesSlot();

        //Draw HDR quad
        drawHDRQuad();
        freeTexturesSlot();

        //Check for errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "--> PIPELINE ERROR: " << err << std::endl;
        }
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
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, thelight->shadowResolution, thelight->shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, thelight->shadowResolution, thelight->shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glBindFramebuffer(GL_FRAMEBUFFER, thelight->depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, thelight->textureDepthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        m_nbOfLight++;
    }

    void deleteLight(int id)
    {
        if (lights.size() > id)
        {
            lights.erase(lights.begin() + id);
        }
    }

    void deleteRenderObject(int id)
    {
        if (m_objectHolder.size() > id)
        {
            m_objectHolder.erase(m_objectHolder.begin() + id);
        }
    }

    void clearScene()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();
    }

    void updateCamera(bool isAzerty=true)
    {
        m_actualCamera->update(&view, isAzerty);
        m_actualCamera->updateGUI(&m_objectHolder,lights,gAlbedoSpec,gNormal,gPosition,gRoughness,gMetallic);
    }

    void refreshScene()
    {
        glfwSwapBuffers(m_display->getGLFWWindow());
    }

    display* getDisplay() { return m_display; }

    int getNbOfLight(){return m_nbOfLight;}

    texturePool* getTexturePool(){ return &m_texturePool; }

    void setCamera(camera* cam)
    {
        m_actualCamera = cam;
    }

    ~scene()
    {
        glDeleteTextures(1, &hdrTexture);
        glDeleteTextures(1, &gAlbedoSpec);
        glDeleteTextures(1, &gNormal);
        glDeleteTextures(1, &gPosition);
        glDeleteTextures(1, &gRoughness);
        glDeleteTextures(1, &gMetallic);
        glDeleteRenderbuffers(1, &hdrRBO);
        glDeleteRenderbuffers(1, &gRBO);
        glDeleteFramebuffers(1, &hdrFBO);
        glDeleteFramebuffers(1, &gBuffer);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vbo_colors);
        glDeleteBuffers(1, &vbo_texCoords);
        glDeleteVertexArrays(1, &vao);
        std::cout << "--> Destroying scene ID=" << id << std::endl;
    }

private:

    int id;

    display* m_display;
    vector<renderObject*> m_objectHolder;

    glm::mat4 projection, view, model;

    vector<light*> lights; 
    int m_nbOfLight;

    camera *m_actualCamera;

    texturePool m_texturePool; // All scenes have their own texture pool (for now)

    //gamma
    float m_gamma;

    //hdr
    GLuint hdrFBO, hdrTexture, hdrRBO;
    shader m_hdrShader;
    GLuint vbo, vbo_colors, vbo_texCoords, vao;
    float m_exposure;

    GLuint gBuffer, gPosition, gNormal, gAlbedoSpec, gRoughness, gMetallic, gRBO;
    shader m_deferedShader;

    void freeTexturesSlot() // Free all texture stuff related to binded texture or slot
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        for (int i = 0; i < 20; i++)
        {
            glBindTextureUnit(i, 0);
        }
    }

    void drawHDRQuad()
    {
        glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_hdrShader.getShaderID());
        glBindVertexArray(vao);
        glUniform1f(glGetUniformLocation(m_hdrShader.getShaderID(), "gamma"), m_gamma);
        glUniform1f(glGetUniformLocation(m_hdrShader.getShaderID(), "exposure"), m_exposure);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glUniform1i(glGetUniformLocation(m_hdrShader.getShaderID(), "hdrTexture"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void drawDeferedQuad()
    {
        glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_deferedShader.getShaderID());
        glBindVertexArray(vao);

        glUniform3f(glGetUniformLocation(m_deferedShader.getShaderID(), "viewPos"), m_actualCamera->getCameraPos().x, m_actualCamera->getCameraPos().y, m_actualCamera->getCameraPos().z);

        int dirLightID = 0;
        int ptLightID = 0;
        int textureCount = 0; // 0 is hdr texture
        for (int i = 0; i < lights.size(); i++) // For each light
        {
            if (glIsTexture(lights[i]->textureDepthMap) && lights[i]->computeShadows && lights[i]->type == POINT_LIGHT)
            {
                if (ptLightID == 0)
                {
                    glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "textureDepthCubemap[0]"), 0);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->textureDepthMap);
                    ptLightID++;
                }
                else
                {
                    string name = "textureDepthCubemap[";
                    name.append(to_string(ptLightID));
                    name.append("]");
                    glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), name.c_str()), textureCount);
                    glActiveTexture(GL_TEXTURE0 + textureCount);
                    glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->textureDepthMap);
                    ptLightID++;
                }
                textureCount++;
            }
            else if (glIsTexture(lights[i]->textureDepthMap) && lights[i]->computeShadows && lights[i]->type == DIRECTIONNAL_LIGHT)
            {
                string name = "lightSpaceMatrix[";
                name.append(to_string(dirLightID));
                name.append("]");
                glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 25.0f);
                glm::mat4 lightView = glm::lookAt(lights[i]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                glUniformMatrix4fv(glGetUniformLocation(m_deferedShader.getShaderID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

                if (dirLightID == 0)
                {
                    glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "textureDepthMap[0]"), 0);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, lights[i]->textureDepthMap);
                    dirLightID++;
                }
                else
                {
                    string name = "textureDepthMap[";
                    name.append(to_string(dirLightID));
                    name.append("]");
                    glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), name.c_str()), textureCount);
                    glActiveTexture(GL_TEXTURE0 + textureCount);
                    glBindTexture(GL_TEXTURE_2D, lights[i]->textureDepthMap);
                    dirLightID++;
                }
                textureCount++;
            }
        }
        m_deferedShader.registerLightToRender(lights,m_nbOfLight);
        if (glIsTexture(gPosition))
        {
            glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(),"gPosition") , textureCount);
            glActiveTexture(GL_TEXTURE0 + textureCount);
            glBindTexture(GL_TEXTURE_2D, gPosition);
            textureCount++;
        }
        if (glIsTexture(gNormal))
        {
            glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "gNormals"), textureCount);
            glActiveTexture(GL_TEXTURE0 + textureCount);
            glBindTexture(GL_TEXTURE_2D, gNormal);
            textureCount++;
        }
        if (glIsTexture(gMetallic))
        {
            glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "gMetallic"), textureCount);
            glActiveTexture(GL_TEXTURE0 + textureCount);
            glBindTexture(GL_TEXTURE_2D, gMetallic);
            textureCount++;
        }
        if (glIsTexture(gRoughness))
        {
            glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "gRoughness"), textureCount);
            glActiveTexture(GL_TEXTURE0 + textureCount);
            glBindTexture(GL_TEXTURE_2D, gRoughness);
            textureCount++;
        }
        if (glIsTexture(gAlbedoSpec))
        {
            glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "gAlbedoSpec"), textureCount);
            glActiveTexture(GL_TEXTURE0 + textureCount);
            glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
            textureCount++;
        }
       
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void drawShadowPass()
    {
        for (int i = 0; i < lights.size(); i++)
        {
            if (lights[i]->computeShadows)
            {
                glCullFace(GL_FRONT);
                glViewport(0, 0, lights[i]->shadowResolution, lights[i]->shadowResolution);
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
    }

    void drawGBufferPass()
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
        glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < m_objectHolder.size(); i++)
        {
            m_objectHolder[i]->renderGBuffer(&projection, &view, &model, m_actualCamera->getCameraPos(), lights, m_nbOfLight);
        }
    }
};

#endif // SCENE_H_INCLUDED
