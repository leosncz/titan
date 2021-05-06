/* Author: leosncz
Handles everything related to scenes and object rendering loop in scenes
*/
#pragma once
#include <iostream>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>
#include "../display/display.h"
#include "../renderObject/renderObject.h"
#include "../camera/camera.h"
#include "../texturePool/texturePool.h"
#include "../shader/shader.h"

class scene
{
public:
    scene(display* customDisplay, camera* cam);
    void renderScene();

    void setGamma(float gamma);
    void setExposure(float exposure);

    void addDrawableObject(renderObject* object);

    void addLight(light* thelight);

    void deleteRenderObject(int id);

    display* getDisplay();

    int getNbOfLight();

    texturePool* getTexturePool();

    void setCamera(camera* cam);

    GLuint getGPosition();
    GLuint getGNormals();
    GLuint getGAlbedo();
    GLuint getGRoughness();
    GLuint getGMetallic();
    GLuint getGAmbient();
    GLuint getSSAO();
    vector<light*>* getLights();
    vector<renderObject*>* getObjectHolder();
    mat4* getViewMatrix();
    camera* getCamera();
    float* getExposure();
    float* getGamma();
    float* getFarShadow();
    float* getSSAOBias();
    void setFarShadow(float value = 25.0f);
    void setSSAOBias(float value = 0.25f);

    void deleteLight(int i);

    ~scene();

private:

    int id;

    display* m_display;
    vector<renderObject*> m_objectHolder;

    glm::mat4 projection, view, model;

    vector<light*> m_lights; 

    camera *m_actualCamera;

    texturePool m_texturePool; // All scenes have their own texture pool (for now)

    float m_farShadow; // Used to tell the maximum distance where we have to calculate shadow

    //gamma
    float m_gamma;

    //hdr
    GLuint hdrFBO, hdrTexture, hdrRBO;
    shader m_hdrShader;
    GLuint vbo, vbo_colors, vbo_texCoords, vao;
    float m_exposure;

    GLuint gBuffer, gPosition, gNormal, gAlbedo, gRoughness, gMetallic, gAmbient, gRBO;
    shader m_deferedShader;

    //SSAO
    std::vector<glm::vec3> ssaoKernel;
    GLuint noiseTexture, ssaoFBO, ssaoColorBuffer;
    shader m_ssaoShader;
    float m_ssaoBias;


    void freeTexturesSlot();
    void drawHDRQuad();
    void drawDeferedQuad();
    void drawShadowPass();
    void drawSSAO();
    void drawGBufferPass();
};

