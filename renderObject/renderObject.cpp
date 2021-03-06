#include "renderObject.h"

renderObject::renderObject(string tag)
{
    m_tag = tag;
    std::cout << "--> Creating new renderObject \"" << m_tag << "\"" << std::endl;
    mustBeDeleted = false;
    texture1 = 0;
    texture2 = 0;
    m_position[0] = 0.0;
    m_position[1] = 0.0;
    m_position[2] = 0.0;
    m_scale[0] = 1.0f;
    m_scale[1] = 1.0f;
    m_scale[2] = 1.0f;
    positionMatrix = glm::mat4(1.0);
    scaleMatrix = glm::mat4(1.0);
    rotationMatrix = glm::mat4(1.0);
    m_nbOfTexture = 0;
    hasNormalMap = false;
    hasRoughnessMap = false;
    hasMetallicMap = false;
    isInitialized = true;
}
string renderObject::getTag() { return m_tag; }
void renderObject::setTag(string tag) { m_tag = tag; }
shader* renderObject::getShader() { return &m_gShader; }
bool renderObject::setData(float* vertices, float* colors, float* texCoord, int nbOfPointToDraw, float* normals, texturePool* texturePool_) // Use this methode to define the mesh by hand
{
    m_nbOfPointToDraw = nbOfPointToDraw;
    m_texturePool = texturePool_;
    //Set texcordds
    if (texCoord != 0) { for (int i = 0; i < nbOfPointToDraw * 2; i++) { m_texCoords.push_back(texCoord[i]); } }

    //m_shader.compileDefaultShader();
    m_depthShader.compileDepthShader();
    m_gShader.compileGShaderShader();

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), colors, GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_texCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
    glBufferData(GL_ARRAY_BUFFER, 2 * m_nbOfPointToDraw * sizeof(float), &m_texCoords[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), normals, GL_STATIC_DRAW);


    //Compute tangent and bitangent
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec2> texCoord_;
    std::vector<glm::vec3> normals_;
    int i2 = 0;
    for (int i = 0; i < m_nbOfPointToDraw; i++)
    {
        vertices_.push_back(glm::vec3(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]));
        normals_.push_back(glm::vec3(normals[i2], normals[i2 + 1], normals[i2 + 2]));
        i2 += 3;
    }
    i2 = 0;
    for (int i = 0; i < m_nbOfPointToDraw; i++)
    {
        texCoord_.push_back(glm::vec2(texCoord[i2], texCoord[i2 + 1]));
        i2 += 2;
    }

    if (isMultipleOf3(vertices_.size()))
    {
        std::vector<glm::vec3> tangent;
        std::vector<glm::vec3> bitangent;

        computeTangent(vertices_, texCoord_, normals_, tangent, bitangent);

        glGenBuffers(1, &vbo_tangent);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tangent);
        glBufferData(GL_ARRAY_BUFFER, tangent.size() * sizeof(glm::vec3), &tangent[0], GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_bitangent);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_bitangent);
        glBufferData(GL_ARRAY_BUFFER, bitangent.size() * sizeof(glm::vec3), &bitangent[0], GL_STATIC_DRAW);
    }
    else
    {
        std::cout << "--> Error : The mesh \"" << m_tag << "\" is probably not triangulated !" << std::endl;
        return false;
    }

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

    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_tangent);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_bitangent);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    return true;
}
void renderObject::setTexturePool(texturePool* texPool)
{
    m_texturePool = texPool;
}
void renderObject::renderDepth(glm::mat4* projection, glm::mat4* view, glm::mat4* model, light* lightToRender, float farShadow)
{
    //Update actual model matrix
    glm::mat4 customModelMatrix = *model * positionMatrix * rotationMatrix * scaleMatrix;

    glUseProgram(m_depthShader.getShaderID());

    glBindVertexArray(vao);

    //Send object relative data to display the object
    glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(), "model"), 1, GL_FALSE, glm::value_ptr(customModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(), "view"), 1, GL_FALSE, glm::value_ptr(*view));
    glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(), "projection"), 1, GL_FALSE, glm::value_ptr(*projection));
    glUniform1f(glGetUniformLocation(m_depthShader.getShaderID(), "farShadow"), farShadow);

    if (lightToRender->type == DIRECTIONNAL_LIGHT)
    {
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, farShadow);
        glm::mat4 lightView = glm::lookAt(lightToRender->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform1i(glGetUniformLocation(m_depthShader.getShaderID(), "isPoint"), 0);
    }
    else if (lightToRender->type == POINT_LIGHT)
    {
        float aspect = (float)1024 / (float)1024;
        float near = 1.0f;
        float far = farShadow;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj *
            glm::lookAt(lightToRender->lightPosition, lightToRender->lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        shadowTransforms.push_back(shadowProj *
            glm::lookAt(lightToRender->lightPosition, lightToRender->lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
        shadowTransforms.push_back(shadowProj *
            glm::lookAt(lightToRender->lightPosition, lightToRender->lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        shadowTransforms.push_back(shadowProj *
            glm::lookAt(lightToRender->lightPosition, lightToRender->lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
        shadowTransforms.push_back(shadowProj *
            glm::lookAt(lightToRender->lightPosition, lightToRender->lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
        shadowTransforms.push_back(shadowProj *
            glm::lookAt(lightToRender->lightPosition, lightToRender->lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
        glUniform3f(glGetUniformLocation(m_depthShader.getShaderID(), "lightPos"), lightToRender->lightPosition.x, lightToRender->lightPosition.y, lightToRender->lightPosition.z);
        for (int i = 0; i < 6; i++) {
            string value = "shadowMatrices["; value.append(to_string(i)); value.append("]");
            glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(), value.c_str()), 1, GL_FALSE, value_ptr(shadowTransforms[i]));
        }
        glUniform1i(glGetUniformLocation(m_depthShader.getShaderID(), "isPoint"), 1);
    }

    glDrawArrays(GL_TRIANGLES, 0, m_nbOfPointToDraw);
}

void renderObject::renderGBuffer(glm::mat4* projection, glm::mat4* view, glm::mat4* model, glm::vec3 viewPos, vector<light*> sceneLights, int numberOfLight)
{
    //Update actual model matrix
    glm::mat4 customModelMatrix = *model * positionMatrix * rotationMatrix * scaleMatrix;

    glUseProgram(m_gShader.getShaderID());

    glBindVertexArray(vao);

    //Send object relative data to display the object
    glUniformMatrix4fv(glGetUniformLocation(m_gShader.getShaderID(), "model"), 1, GL_FALSE, glm::value_ptr(customModelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_gShader.getShaderID(), "view"), 1, GL_FALSE, glm::value_ptr(*view));
    glUniformMatrix4fv(glGetUniformLocation(m_gShader.getShaderID(), "projection"), 1, GL_FALSE, glm::value_ptr(*projection));
    glUniform3f(glGetUniformLocation(m_gShader.getShaderID(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "howManyTex"), m_nbOfTexture);
    glm::mat4 inversedModel = glm::inverse(customModelMatrix);
    glUniformMatrix4fv(glGetUniformLocation(m_gShader.getShaderID(), "inversedModel"), 1, GL_FALSE, glm::value_ptr(inversedModel));

    glUniform1f(glGetUniformLocation(m_gShader.getShaderID(), "metallic"), m_gShader.metallic);
    glUniform1f(glGetUniformLocation(m_gShader.getShaderID(), "roughness"), m_gShader.roughness);
    glUniform1f(glGetUniformLocation(m_gShader.getShaderID(), "ambient"), m_gShader.ambient);


    int dirLightID = 0;
    int ptLightID = 0;
    int textureCount = 0; // 0 is hdr texture

    if (glIsTexture(texture1))
    {
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "texture1"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, texture1);
        textureCount++;
    }
    if (glIsTexture(texture2))
    {
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "texture2"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, texture2);
        textureCount++;
    }
    if (hasNormalMap)
    {
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "normalMap"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "useNormalMap"), 1);
        textureCount++;
    }
    else { glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "useNormalMap"), 0); }
    if (hasRoughnessMap)
    {
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "roughnessMap"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, roughnessTexture);
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "useRoughnessMap"), 1);
        textureCount++;
    }
    else { glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "useRoughnessMap"), 0); }
    if (hasMetallicMap)
    {
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "metallicMap"), textureCount);
        glActiveTexture(GL_TEXTURE0 + textureCount);
        glBindTexture(GL_TEXTURE_2D, metallicTexture);
        glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "useMetallicMap"), 1);
        textureCount++;
    }
    else { glUniform1i(glGetUniformLocation(m_gShader.getShaderID(), "useMetallicMap"), 0); }

    glDrawArrays(GL_TRIANGLES, 0, m_nbOfPointToDraw);
}

void renderObject::updateVertex(float* vertices, int nbOfPointToDraw)
{
    m_nbOfPointToDraw = nbOfPointToDraw;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * m_nbOfPointToDraw * sizeof(float), vertices);
}
void renderObject::setTextureResolution(int res)
{
    //Get actual texcoords
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);

    for (int i = 0; i < m_nbOfPointToDraw * 2; i++)
    {
        m_texCoords[i] = m_texCoords[i] * res;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * m_nbOfPointToDraw * sizeof(float), &m_texCoords[0]);
}

void renderObject::addTexture(const char* pathToTexture) // Returns the created texture (max 4)
{
    if (m_nbOfTexture < 2)
    {
        if (m_nbOfTexture == 0)
        {
            setTexture(&texture1, pathToTexture, true);
        }
        else if (m_nbOfTexture == 1)
        {
            setTexture(&texture2, pathToTexture, true);
        }
    }
}

void renderObject::moveObject(glm::vec3 position) // move from object origin 
{
    positionMatrix = glm::translate(positionMatrix, position);
    m_position[0] += position.x;
    m_position[1] += position.y;
    m_position[2] += position.z;
}
void renderObject::moveObjectFromSceneOrigin(glm::vec3 position) // move from scene origin 
{
    positionMatrix = glm::mat4(1.0);
    positionMatrix = glm::translate(positionMatrix, position);
    m_position[0] = position.x;
    m_position[1] = position.y;
    m_position[2] = position.z;
}
void renderObject::rotateObjectFromLastRotation(float angle, glm::vec3 factors) // rotate from object origin
{
    rotationMatrix = glm::rotate(rotationMatrix, angle, factors);
}
void renderObject::rotateObject(float angle, glm::vec3 factors) // rotate from object last rotation
{
    rotationMatrix = mat4(1.0);
    rotationMatrix = glm::rotate(rotationMatrix, angle, factors);
}
void renderObject::scaleObject(glm::vec3 factors)
{
    scaleMatrix = mat4(1.0);
    scaleMatrix = glm::scale(scaleMatrix, factors);
    m_scale[0] = factors.x;
    m_scale[1] = factors.y;
    m_scale[2] = factors.z;
}
renderObject::~renderObject()
{
    if (isInitialized)
    {
        std::cout << "--> Destroying renderObject \"" << m_tag << "\"" << std::endl;
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &vbo_colors);
        glDeleteBuffers(1, &vbo_texCoords);
        glDeleteBuffers(1, &vbo_normals);
        glDeleteBuffers(1, &vbo_tangent);
        glDeleteBuffers(1, &vbo_bitangent);
        glDeleteVertexArrays(1, &vao);
    }
}

//Material properties edition
void renderObject::setRoughness(float intensity)
{
    m_gShader.roughness = intensity;
}
void renderObject::setMetallic(float intensity)
{
    m_gShader.metallic = intensity;
}
void renderObject::setAmbient(float intensity)
{
    m_gShader.ambient = intensity;
}
void renderObject::removeNormalMap()
{
    hasNormalMap = false;
}
void renderObject::setNormalMap(const char* path)
{
    setTexture(&normalTexture, path);
    hasNormalMap = true;
}
void renderObject::removeMetallicMap()
{
    hasMetallicMap = false;
}
void renderObject::setMetallicMap(const char* path)
{
    setTexture(&metallicTexture, path);
    hasMetallicMap = true;
}
void renderObject::removeRoughnessMap()
{
    hasRoughnessMap = false;
}
void renderObject::setRoughnessMap(const char* path)
{
    setTexture(&roughnessTexture, path);
    hasRoughnessMap = true;
}

float* renderObject::getPosition() { return m_position; }
float* renderObject::getScale() { return m_scale; }

void renderObject::removeTexture1()
{
    if (m_nbOfTexture == 1)
    {
        m_nbOfTexture = 0;
    }
    else if (m_nbOfTexture == 2)
    {
        texture1 = texture2;
        m_nbOfTexture = 1;
        texture2 = 0;
    }
}

void renderObject::removeTexture2()
{
    if (m_nbOfTexture == 2)
    {
        m_nbOfTexture = 1;
        texture2 = 0;
    }
}

bool renderObject::doesMeshHasNormalMap() { return hasNormalMap; }
bool renderObject::doesMeshHasRoughnessMap() { return hasRoughnessMap; }
bool renderObject::doesMeshHasMetallicMap() { return hasMetallicMap; }
GLuint renderObject::getNormalMap() { return normalTexture; }
GLuint renderObject::getMetallicMap() { return metallicTexture; }
GLuint renderObject::getRoughnessMap() { return roughnessTexture; }
GLuint* renderObject::getTextures() { GLuint textures[2] = { texture1,texture2 }; return textures; }
int renderObject::getNbOfTextures() { return m_nbOfTexture; }
void renderObject::setDeleteStatus(bool status) { mustBeDeleted = status; }
bool renderObject::getDeleteStatus() { return mustBeDeleted; }

// Used to compute tangent and bitangent for normal mapping
void renderObject::computeTangent(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
{
    for (int i = 0; i < vertices.size(); i += 3) {
        // Shortcuts for vertices
        glm::vec3& v0 = vertices[i + 0];
        glm::vec3& v1 = vertices[i + 1];
        glm::vec3& v2 = vertices[i + 2];

        // Shortcuts for UVs
        glm::vec2& uv0 = uvs[i + 0];
        glm::vec2& uv1 = uvs[i + 1];
        glm::vec2& uv2 = uvs[i + 2];

        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        // Set the same tangent for all three vertices of the triangle.
        // They will be merged later, in vboindexer.cpp
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        // Same thing for binormals
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
}
int renderObject::isMultipleOf3(int n) // Used to determine if a mesh is triangulated
{
    int odd_count = 0;
    int even_count = 0;

    /* Make no positive if +n is multiple of 3
    then is -n. We are doing this to avoid
    stack overflow in recursion*/
    if (n < 0)
        n = -n;
    if (n == 0)
        return 1;
    if (n == 1)
        return 0;

    while (n) {
        /* If odd bit is set then
        increment odd counter */
        if (n & 1)
            odd_count++;

        /* If even bit is set then
        increment even counter */
        if (n & 2)
            even_count++;
        n = n >> 2;
    }

    return isMultipleOf3(abs(odd_count - even_count));
}
void renderObject::setTexture(GLuint* texture, const char* path, bool isDiffuseTexture)
{
    *texture = m_texturePool->getCacheTextureID(path, isDiffuseTexture);
    if (isDiffuseTexture && *texture != 0)
    {
        m_nbOfTexture++;
    }
}