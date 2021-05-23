#include "scene.h"
#include <gtc/matrix_transform.hpp>
scene::scene(string tag, display* customDisplay, camera* cam) // Set the display that will be used to render the scene
{
    m_tag = tag;

    std::cout << "--> Creating new scene \"" << m_tag << "\"" << std::endl;

    m_display = customDisplay;

    // Setup ogl matrix
    projection = glm::perspective(glm::radians(45.0f), (float)((float)customDisplay->getDisWidth() / (float)customDisplay->getDisHeight()), 0.01f, 200.0f);
    view = glm::mat4(1.0);
    model = glm::mat4(1.0);

    m_actualCamera = cam;
    m_farShadow = 25.0f;

    m_gamma = 2.2f;
    m_ssaoBias = 0.25f;

    //Create HDR stuff
    m_exposure = 1.0f;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // - normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - color + specular color buffer
    glGenTextures(1, &gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

    glGenTextures(1, &gRoughness);
    glBindTexture(GL_TEXTURE_2D, gRoughness);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gRoughness, 0);

    glGenTextures(1, &gMetallic);
    glBindTexture(GL_TEXTURE_2D, gMetallic);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gMetallic, 0);

    glGenTextures(1, &gAmbient);
    glBindTexture(GL_TEXTURE_2D, gAmbient);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, &swizzleMask[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gAmbient, 0);

    glGenRenderbuffers(1, &gRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, gRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_display->getDisWidth(), m_display->getDisHeight());
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBO);

    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
    glDrawBuffers(6, attachments);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //SSAO
    std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = 0.1f + ((scale * scale) * (1.0f - 0.1f));
        sample *= scale;
        ssaoKernel.push_back(sample);
    }
    // generate noise texture
    // ----------------------
    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_display->getDisWidth(), m_display->getDisHeight(), 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_ssaoShader.compileSSAOShader();

    //Create quad to render HDR scene
    m_hdrShader.compileQuadRenderShader();
    float vertices[] = { -1.0,-1.0,0.0,    1.0,-1.0,0.0,      1.0,1.0,0.0,     1.0,1.0,0.0,   -1.0,1.0,0.0,    -1.0,-1.0,0.0 };
    float colors[] = { 1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0 };
    float texCoords[] = { 0.0,0.0,  1.0,0.0,   1.0,1.0,   1.0,1.0,   0.0,1.0,    0.0,0.0 };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), colors, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_texCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
    glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), &texCoords[0], GL_STATIC_DRAW);

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
void scene::renderScene()
{
    // SHADOW PASS
    drawShadowPass();
    freeTexturesSlot();

    //Generate GBuffer
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    drawGBufferPass();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    freeTexturesSlot();

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    drawSSAO();
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

    //Check for global errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "--> PIPELINE ERROR: " << err << std::endl;
    }
}

void scene::setGamma(float gamma) { m_gamma = gamma; }
void scene::setExposure(float exposure) { m_exposure = exposure; }

void scene::addDrawableObject(renderObject* object)
{
    std::cout << "---> Adding new renderObject \"" << object->getTag() << "\" to scene \"" << m_tag << "\"" << std::endl;
    m_objectHolder.push_back(object);
}

void scene::addLight(light* thelight)
{
    m_lights.push_back(thelight);
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
}

void scene::deleteRenderObject(int id)
{
    if (m_objectHolder.size() > id)
    {
        if (m_objectHolder[id]->getDeleteStatus())
        {
            renderObject* obj = m_objectHolder.at(id);
            m_objectHolder.erase(m_objectHolder.begin() + id);
            delete obj;
        }
        else
        {
            m_objectHolder.erase(m_objectHolder.begin() + id);
        }
    }
}

display* scene::getDisplay() { return m_display; }

int scene::getNbOfLight() { return m_lights.size(); }

texturePool* scene::getTexturePool() { return &m_texturePool; }

void scene::setCamera(camera* cam)
{
    m_actualCamera = cam;
}

GLuint scene::getGPosition() { return gPosition; }
GLuint scene::getGNormals() { return gNormal; }
GLuint scene::getGAlbedo() { return gAlbedo; }
GLuint scene::getGRoughness() { return gRoughness; }
GLuint scene::getGMetallic() { return gMetallic; }
GLuint scene::getGAmbient() { return gAmbient; }
GLuint scene::getSSAO() { return ssaoColorBuffer; }
vector<light*>* scene::getLights() { return &m_lights; }
vector<renderObject*>* scene::getObjectHolder() { return &m_objectHolder; }
mat4* scene::getViewMatrix() { return &view; }
camera* scene::getCamera() { return m_actualCamera; }
float* scene::getExposure() { return &m_exposure; }
float* scene::getGamma() { return &m_gamma; }
float* scene::getFarShadow() { return &m_farShadow; }
float* scene::getSSAOBias() { return &m_ssaoBias; }
void scene::setFarShadow(float value) { m_farShadow = value; }
void scene::setSSAOBias(float value) { m_ssaoBias = value; }

void scene::deleteLight(int i)
{
    light* li = m_lights.at(i);
    m_lights.erase(m_lights.begin() + i);
    if (li->mustBeDeleted)
    {
        delete li;
    }
}

scene::~scene()
{
    glDeleteTextures(1, &hdrTexture);
    glDeleteTextures(1, &gAlbedo);
    glDeleteTextures(1, &gNormal);
    glDeleteTextures(1, &gPosition);
    glDeleteTextures(1, &gRoughness);
    glDeleteTextures(1, &gMetallic);
    glDeleteTextures(1, &gAmbient);
    glDeleteTextures(1, &ssaoColorBuffer);
    glDeleteTextures(1, &noiseTexture);
    glDeleteRenderbuffers(1, &hdrRBO);
    glDeleteRenderbuffers(1, &gRBO);
    glDeleteFramebuffers(1, &ssaoFBO);
    glDeleteFramebuffers(1, &hdrFBO);
    glDeleteFramebuffers(1, &gBuffer);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo_colors);
    glDeleteBuffers(1, &vbo_texCoords);
    glDeleteVertexArrays(1, &vao);

    for (int i = 0; i < m_lights.size(); i++)
    {
        if (m_lights[i]->mustBeDeleted)
        {
            delete m_lights[i];
        }
    }
    for (int i = 0; i < m_objectHolder.size(); i++)
    {
        if (m_objectHolder[i]->getDeleteStatus())
        {
            delete m_objectHolder[i];
        }
    }

    std::cout << "--> Destroying scene \"" << m_tag << "\"" << std::endl;
}

void scene::freeTexturesSlot() // Free all texture stuff related to binded texture or slot
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    for (int i = 0; i < 20; i++)
    {
        glBindTextureUnit(i, 0);
    }
}

void scene::drawHDRQuad()
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

void scene::drawDeferedQuad()
{
    glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_deferedShader.getShaderID());
    glBindVertexArray(vao);

    glUniform3f(glGetUniformLocation(m_deferedShader.getShaderID(), "viewPos"), m_actualCamera->getCameraPos().x, m_actualCamera->getCameraPos().y, m_actualCamera->getCameraPos().z);
    glUniform1f(glGetUniformLocation(m_deferedShader.getShaderID(), "farShadow"), m_farShadow);

    int textureCount = 0; // 0 is hdr texture
    for (int i = 0; i < m_lights.size(); i++) // For each light
    {
        if (glIsTexture(m_lights[i]->textureDepthMap) && m_lights[i]->doesComputeShadows() && m_lights[i]->type == POINT_LIGHT)
        {
            string name = "textureDepthCubemap[";
            name.append(to_string(textureCount));
            name.append("]");
            glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), name.c_str()), textureCount);
            glActiveTexture(GL_TEXTURE0 + textureCount);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_lights[i]->textureDepthMap);
            textureCount++;
        }
        else if (glIsTexture(m_lights[i]->textureDepthMap) && m_lights[i]->doesComputeShadows() && m_lights[i]->type == DIRECTIONNAL_LIGHT)
        {
            string name = "lightSpaceMatrix[";
            name.append(to_string(textureCount));
            name.append("]");
            glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, m_farShadow);
            glm::mat4 lightView = glm::lookAt(m_lights[i]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
            glUniformMatrix4fv(glGetUniformLocation(m_deferedShader.getShaderID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

            name = "textureDepthMap[";
            name.append(to_string(textureCount));
            name.append("]");
            glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), name.c_str()), textureCount);
            glActiveTexture(GL_TEXTURE0 + textureCount);
            glBindTexture(GL_TEXTURE_2D, m_lights[i]->textureDepthMap);
            textureCount++;
        }
    }
    m_deferedShader.registerLightToRender(m_lights, m_lights.size());
    if (glIsTexture(gPosition))
    {
        glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "gPosition"), textureCount);
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
    if (glIsTexture(gAlbedo))
    {
        glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "gAlbedoSpec"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        textureCount++;
    }
    if (glIsTexture(gAmbient))
    {
        glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "gAmbient"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, gAmbient);
        textureCount++;
    }
    if (glIsTexture(ssaoColorBuffer))
    {
        glUniform1i(glGetUniformLocation(m_deferedShader.getShaderID(), "ssao"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        textureCount++;
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void scene::drawShadowPass()
{
    for (int i = 0; i < m_lights.size(); i++)
    {
        if (m_lights[i]->doesComputeShadows())
        {
            glCullFace(GL_FRONT);
            glViewport(0, 0, m_lights[i]->shadowResolution, m_lights[i]->shadowResolution);
            glBindFramebuffer(GL_FRAMEBUFFER, m_lights[i]->depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            for (int i2 = 0; i2 < m_objectHolder.size(); i2++)
            {
                m_objectHolder[i2]->renderDepth(&projection, &view, &model, m_lights[i], m_farShadow);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glCullFace(GL_BACK);
        }
    }
}

void scene::drawSSAO()
{
    //glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_ssaoShader.getShaderID());
    glBindVertexArray(vao);

    int textureCount = 0; // 0 is hdr texture

    if (glIsTexture(gPosition))
    {
        glUniform1i(glGetUniformLocation(m_ssaoShader.getShaderID(), "gPosition"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        textureCount++;
    }
    if (glIsTexture(gNormal))
    {
        glUniform1i(glGetUniformLocation(m_ssaoShader.getShaderID(), "gNormals"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        textureCount++;
    }
    if (glIsTexture(noiseTexture))
    {
        glUniform1i(glGetUniformLocation(m_ssaoShader.getShaderID(), "noiseTexture"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        textureCount++;
    }
    for (int i = 0; i < ssaoKernel.size(); i++)
    {
        string name = "samples[";
        name.append(to_string(i));
        name.append("]");
        glUniform3f(glGetUniformLocation(m_ssaoShader.getShaderID(), name.c_str()), ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);
    }
    glUniformMatrix4fv(glGetUniformLocation(m_ssaoShader.getShaderID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(m_ssaoShader.getShaderID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform1f(glGetUniformLocation(m_ssaoShader.getShaderID(), "bias"), m_ssaoBias);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void scene::drawGBufferPass()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // keep it black so it doesn't leak into g-buffer
    glViewport(0, 0, m_display->getDisWidth(), m_display->getDisHeight());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < m_objectHolder.size(); i++)
    {
        m_objectHolder[i]->renderGBuffer(&projection, &view, &model, m_actualCamera->getCameraPos(), m_lights, m_lights.size());
    }
}