/* Author: leosncz
Handles everything related to drawable nodes
*/
#ifndef RENDEROBJECT_H_INCLUDED
#define RENDEROBJECT_H_INCLUDED
#include "../shader/shader.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
#include <iostream>
#include <string>
#include <vector>
#include "../texturePool/texturePool.h"
class renderObject
{
public:
    renderObject() 
    { 
        id = rand(); 
        std::cout << "--> Creating new renderObject ID=" << id << std::endl;
        tag = "Custom Object";
    }
    string getTag() { return tag; }
    shader* getShader() { return &m_shader; }
    void setData(float* vertices, float* colors, float* texCoord, int nbOfPointToDraw, float* normals=0, texturePool* texturePool_=0, float* tangent=0,float* bitangent=0) // Use this methode to define the mesh by hand
    {
        m_nbOfPointToDraw = nbOfPointToDraw;
        modelMatrix = glm::mat4(1.0);
        m_nbOfTexture = 0;
        m_nbOfTextureToDraw = 0;
        hasSpecularMap = false;
        hasNormalMap = false;
        isInitialized = true;
        m_texturePool = texturePool_;
        //Set texcordds
        if (texCoord != 0) { for (int i = 0; i < nbOfPointToDraw * 2; i++) { texCoords.push_back(texCoord[i]); } }

        m_shader.compileDefaultShader();
        m_depthShader.compileDepthShader();

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_colors);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
        glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), colors, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_texCoords);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
        glBufferData(GL_ARRAY_BUFFER, 2 * m_nbOfPointToDraw * sizeof(float), &texCoords[0], GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), normals, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_tangent);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_tangent);
        glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), tangent, GL_STATIC_DRAW);

        glGenBuffers(1, &vbo_bitangent);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_bitangent);
        glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), bitangent, GL_STATIC_DRAW);

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

        modelID = glGetUniformLocation(m_shader.getShaderID(), "model");
        viewID = glGetUniformLocation(m_shader.getShaderID(), "view");
        projectionID = glGetUniformLocation(m_shader.getShaderID(), "projection");
        viewPosID = glGetUniformLocation(m_shader.getShaderID(), "viewPos");
        texture1ID = glGetUniformLocation(m_shader.getShaderID(), "texture1");
        texture2ID = glGetUniformLocation(m_shader.getShaderID(), "texture2");
        texture3ID = glGetUniformLocation(m_shader.getShaderID(), "texture3");
        specularTextureID = glGetUniformLocation(m_shader.getShaderID(), "specularMap");
        normalTextureID = glGetUniformLocation(m_shader.getShaderID(), "normalMap");
        howManyTexID = glGetUniformLocation(m_shader.getShaderID(), "howManyTex");
        useSpecularMapID = glGetUniformLocation(m_shader.getShaderID(), "useSpecularMap");
        useNormalMapID = glGetUniformLocation(m_shader.getShaderID(), "useNormalMap");
    }
    void setTexturePool(texturePool* texPool)
    {
        m_texturePool = texPool;
    }
    void renderDepth(glm::mat4* projection, glm::mat4* view, glm::mat4* model, light* lightToRender)
    {
        //Update actual model matrix
        glm::mat4 customModelMatrix = *model * modelMatrix;

        glUseProgram(m_depthShader.getShaderID());

        glBindVertexArray(vao);

        //Send object relative data to display the object
        glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(),"model"), 1, GL_FALSE, glm::value_ptr(customModelMatrix));
        glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(), "view"), 1, GL_FALSE, glm::value_ptr(*view));
        glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(), "projection"), 1, GL_FALSE, glm::value_ptr(*projection));

        if (lightToRender->type == DIRECTIONNAL_LIGHT)
        {
            glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 25.0f);
            glm::mat4 lightView = glm::lookAt(lightToRender->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
            glUniformMatrix4fv(glGetUniformLocation(m_depthShader.getShaderID(),"lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
            glUniform1i(glGetUniformLocation(m_depthShader.getShaderID(), "isPoint"), 0);
        }
        else if (lightToRender->type == POINT_LIGHT)
        {
            float aspect = (float)1024 / (float)1024;
            float near = 1.0f;
            float far = 25.0f;
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
    void render(glm::mat4* projection, glm::mat4* view, glm::mat4* model, glm::vec3 viewPos, vector<light*> sceneLights = {}, int numberOfLight = 0)
    {
        //Update actual model matrix
        glm::mat4 customModelMatrix = *model * modelMatrix;

        glUseProgram(m_shader.getShaderID());

        glBindVertexArray(vao);

            //Send object relative data to display the object
            glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(customModelMatrix));
            glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(*view));
            glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(*projection));
            glUniform3f(viewPosID,viewPos.x,viewPos.y,viewPos.z);

            glUniform1i(howManyTexID,m_nbOfTextureToDraw);

            int dirLightID = 0;
            int ptLightID = 0;
            int textureCount = 0; // 0 is hdr texture
            for (int i = 0; i < sceneLights.size(); i++) // For each light
            {
                if (glIsTexture(sceneLights[i]->textureDepthMap) && sceneLights[i]->computeShadows && sceneLights[i]->type == POINT_LIGHT)
                {
                    if (ptLightID == 0)
                    {
                        glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthCubemap"), 0);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_CUBE_MAP, sceneLights[i]->textureDepthMap);
                        ptLightID++;
                    }
                    else
                    {
                        string name = "textureDepthCubemap";
                        name.append(to_string(ptLightID));
                        glUniform1i(glGetUniformLocation(m_shader.getShaderID(), name.c_str()), textureCount);
                        glActiveTexture(GL_TEXTURE0 + textureCount);
                        glBindTexture(GL_TEXTURE_CUBE_MAP, sceneLights[i]->textureDepthMap);
                        ptLightID++;
                    }
                    textureCount++;
                }
                else if (glIsTexture(sceneLights[i]->textureDepthMap) && sceneLights[i]->computeShadows && sceneLights[i]->type == DIRECTIONNAL_LIGHT)
                {
                    string name = "lightSpaceMatrix";
                    if (dirLightID != 0) {
                        name.append(to_string(dirLightID));
                    }
                    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 25.0f);
                    glm::mat4 lightView = glm::lookAt(sceneLights[i]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(glGetUniformLocation(m_shader.getShaderID(),name.c_str()), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

                    if (dirLightID == 0)
                    {
                        glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap"), 0);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, sceneLights[i]->textureDepthMap);
                        dirLightID++;
                    }
                    else
                    {
                        string name = "textureDepthMap";
                        name.append(to_string(dirLightID));
                        glUniform1i(glGetUniformLocation(m_shader.getShaderID(), name.c_str()), textureCount);
                        glActiveTexture(GL_TEXTURE0 + textureCount);
                        glBindTexture(GL_TEXTURE_2D, sceneLights[i]->textureDepthMap);
                        dirLightID++;
                    }
                    textureCount++;
                }
            }

            if (glIsTexture(texture1))
            {
                glUniform1i(texture1ID, textureCount);
                glActiveTexture(GL_TEXTURE0 + textureCount);
                glBindTexture(GL_TEXTURE_2D, texture1);
                textureCount++;
            }
            if (glIsTexture(texture2))
            {
                glUniform1i(texture2ID, textureCount);
                glActiveTexture(GL_TEXTURE0 + textureCount);
                glBindTexture(GL_TEXTURE_2D, texture2);
                textureCount++;
            }
            if (glIsTexture(texture3))
            {
                glUniform1i(texture3ID, textureCount);
                glActiveTexture(GL_TEXTURE0 + textureCount);
                glBindTexture(GL_TEXTURE_2D, texture3);
                textureCount++;
            }
            if (hasSpecularMap)
            {
                glUniform1i(specularTextureID, textureCount);
                glActiveTexture(GL_TEXTURE0 + textureCount);
                glBindTexture(GL_TEXTURE_2D, specularTexture);
                glUniform1i(useSpecularMapID, 1);
                textureCount++;
            }
            else { glUniform1i(useSpecularMapID, 0);}
            if (hasNormalMap)
            {
                glUniform1i(normalTextureID, textureCount);
                glActiveTexture(GL_TEXTURE0 + textureCount);
                glBindTexture(GL_TEXTURE_2D, normalTexture);
                glUniform1i(useNormalMapID, 1);
                textureCount++;
            }
            else { glUniform1i(useNormalMapID, 0); }

            m_shader.registerLightToRender(sceneLights,numberOfLight); // MUST BE CALLED if you want lighting to work

            glDrawArrays(GL_TRIANGLES,0,m_nbOfPointToDraw);
    }
    void removeSpecularMap()
    {
        glDeleteTextures(1, &specularTexture);
        hasSpecularMap = false;
    }
    void removeNormalMap()
    {
        glDeleteTextures(1, &normalTexture);
        hasNormalMap = false;
    }
    void setSpecularMap(const char* path)
    {
        setTexture(&specularTexture, path);
        hasSpecularMap = true;
    }
    void setNormalMap(const char* path)
    {
        setTexture(&normalTexture, path);
        hasNormalMap = true;
    }
    void setNumberOfTextureToDraw(int howMany){ // Set how many texture should be reserved for drawing
        m_nbOfTextureToDraw = howMany;
    }

    void updateVertex(float* vertices, int nbOfPointToDraw)
    {
        m_nbOfPointToDraw = nbOfPointToDraw;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER,0, 3 * m_nbOfPointToDraw * sizeof(float), vertices);
    }
    void setTextureResolution(int res)
    {
        //Get actual texcoords
        glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);

        for(int i = 0;i<m_nbOfPointToDraw*2;i++)
        {
            texCoords[i] = texCoords[i]*res;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * m_nbOfPointToDraw * sizeof(float), &texCoords[0]);
    }
   
    void addTexture(const char* pathToTexture) // Returns the created texture (max 4)
    {
        if(m_nbOfTexture == 0)
        {  
            setTexture(&texture1, pathToTexture,true); 
        }
        else if(m_nbOfTexture == 1)
        {
            setTexture(&texture2,pathToTexture,true);
        }
        else if(m_nbOfTexture == 2)
        {
            setTexture(&texture3,pathToTexture,true);
        }
        m_nbOfTexture++;
    }

    void moveObject(glm::vec3 position) // move from scene origin everytime
    {
        modelMatrix = glm::translate(modelMatrix,position);
    }

    void rotateObject(float angle, glm::vec3 factors) // rotate from last rotation AND scene origin
    {
        modelMatrix = glm::rotate(modelMatrix,angle,factors);
    }
    void scaleObject(glm::vec3 factors)
    {
        modelMatrix = glm::scale(modelMatrix,factors);
    }
    ~renderObject()
    {
        if (isInitialized)
        {
            std::cout << "--> Destroying renderObject ID=" << id << std::endl;
            glDeleteTextures(1, &texture1);
            glDeleteTextures(1, &texture2);
            glDeleteTextures(1, &texture3);
            glDeleteTextures(1, &normalTexture);
            glDeleteTextures(1, &specularTexture);
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
    void setSpecularStrenght(float intensity)
    {
        m_shader.specStrenght = intensity;
    }
    void setAmbiantStrenght(float intensity)
    {
        m_shader.ambStrenght = intensity;
    }

    int getID(){return id;}

protected:

    int id;
    string tag;

    bool isInitialized;

    glm::mat4 modelMatrix;

    int m_nbOfPointToDraw;
    int m_nbOfTexture; // Nb of texture created
    int m_nbOfTextureToDraw; // Nb of texture to draw

    GLuint vbo, vbo_colors, vbo_texCoords, vbo_normals, vbo_tangent, vbo_bitangent;

    vector<float> texCoords;

    texturePool* m_texturePool; //Associated texture pool that contains cached texture

    GLuint vao;
    shader m_shader;
    shader m_depthShader;

    GLuint modelID;
    GLuint viewID;
    GLuint viewPosID;
    GLuint projectionID;
    GLuint howManyTexID;
    GLuint useSpecularMapID;
    GLuint useNormalMapID;
    GLuint texture1ID;
    GLuint texture2ID;
    GLuint texture3ID;
    GLuint specularTextureID;
    GLuint normalTextureID;

    bool hasSpecularMap;
    bool hasNormalMap;

    GLuint texture1;
    GLuint texture2;
    GLuint texture3;
    GLuint specularTexture;
    GLuint normalTexture;

    void setTexture(GLuint *texture, const char* path, bool isDiffuseTexture=false)
    {
        //Check if texture is cached
        GLuint cacheValue = m_texturePool->getCacheTextureID(path);
        if ( cacheValue != -1) // Texture is cached
        {
            *texture = cacheValue;
            return;
        }
        stbi_set_flip_vertically_on_load(true);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if(data)
        {
            glGenTextures(1, texture);
            glBindTexture(GL_TEXTURE_2D, *texture);
            if (isDiffuseTexture)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "renderObject ID=" << id << " Error loading image at " << path << std::endl;
        }
        stbi_image_free(data);
        m_texturePool->addTextureToCache(*texture, path);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

};
#endif // RENDEROBJECT_H_INCLUDED
