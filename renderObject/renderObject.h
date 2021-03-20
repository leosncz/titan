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
    renderObject() { id = rand(); std::cout << "--> Creating new renderObject ID=" << id << std::endl;
    }
    void setData(float* vertices, float* colors, float* texCoord, int nbOfPointToDraw, float* normals=0, texturePool* texturePool_=0) // Use this methode to define the mesh by hand
    {
        m_nbOfPointToDraw = nbOfPointToDraw;
        modelMatrix = glm::mat4(1.0);
        m_nbOfTexture = 0;
        m_nbOfTextureToDraw = 0;
        hasSpecularMap = false;
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

        modelID = glGetUniformLocation(m_shader.getShaderID(), "model");
        viewID = glGetUniformLocation(m_shader.getShaderID(), "view");
        projectionID = glGetUniformLocation(m_shader.getShaderID(), "projection");
        viewPosID = glGetUniformLocation(m_shader.getShaderID(), "viewPos");
        texture1ID = glGetUniformLocation(m_shader.getShaderID(), "texture1");
        texture2ID = glGetUniformLocation(m_shader.getShaderID(), "texture2");
        texture3ID = glGetUniformLocation(m_shader.getShaderID(), "texture3");
        specularTextureID = glGetUniformLocation(m_shader.getShaderID(), "specularMap");
        textureDepthMapID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap");
        textureDepthMap1ID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap1");
        howManyTexID = glGetUniformLocation(m_shader.getShaderID(), "howManyTex");
        useSpecularMapID = glGetUniformLocation(m_shader.getShaderID(), "useSpecularMap");
        lightSpaceMatrixID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix");
        lightSpaceMatrix1ID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix1");
        lightSpaceMatrixDepthID = glGetUniformLocation(m_depthShader.getShaderID(), "lightSpaceMatrix");
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
            glUniformMatrix4fv(lightSpaceMatrixDepthID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
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
    void render(glm::mat4 *projection, glm::mat4 *view, glm::mat4 *model, glm::vec3 viewPos, light* sceneLights[]=0, int numberOfLight=0, GLuint textureDepthMap=0, GLuint textureDepthMap1 = 0, GLuint textureDepthCubemap = 0, GLuint textureDepthCubemap1=0, GLuint textureDepthCubemap2 = 0, GLuint textureDepthCubemap3 = 0, GLuint textureDepthCubemap4 = 0, GLuint textureDepthCubemap5 = 0, GLuint textureDepthCubemap6 = 0)
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

            if (numberOfLight > 0)
            {
                glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 25.0f);

                if(numberOfLight >= 1 && sceneLights[0]->type == DIRECTIONNAL_LIGHT && sceneLights[0]->computeShadows)
                {
                    glm::mat4 lightView = glm::lookAt(sceneLights[0]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
                }
                if (numberOfLight >= 2 && sceneLights[1]->type == DIRECTIONNAL_LIGHT && sceneLights[1]->computeShadows)
                {
                    glm::mat4 lightView = glm::lookAt(sceneLights[1]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(lightSpaceMatrix1ID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
                }                
            }

            if (glIsTexture(textureDepthCubemap))
            {
                glUniform1i(glGetUniformLocation(m_shader.getShaderID(),"textureDepthCubemap"), 0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureDepthCubemap);
            }
            if (glIsTexture(textureDepthCubemap1))
            {
                glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthCubemap1"), 1);
                glActiveTexture(GL_TEXTURE0+1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureDepthCubemap1);
            }
            if (glIsTexture(textureDepthCubemap2))
            {
                glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthCubemap2"), 2);
                glActiveTexture(GL_TEXTURE0 + 2);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureDepthCubemap2);
            }
            if (glIsTexture(textureDepthCubemap3))
            {
                glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthCubemap3"), 3);
                glActiveTexture(GL_TEXTURE0 + 3);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureDepthCubemap3);
            }
            if (glIsTexture(textureDepthCubemap4))
            {
                glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthCubemap4"), 4);
                glActiveTexture(GL_TEXTURE0 + 4);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureDepthCubemap4);
            }
            if (glIsTexture(textureDepthCubemap5))
            {
                glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthCubemap5"), 5);
                glActiveTexture(GL_TEXTURE0 + 5);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureDepthCubemap5);
            }
            if (glIsTexture(textureDepthCubemap6))
            {
                glUniform1i(glGetUniformLocation(m_shader.getShaderID(), "textureDepthCubemap6"), 6);
                glActiveTexture(GL_TEXTURE0 + 6);
                glBindTexture(GL_TEXTURE_CUBE_MAP, textureDepthCubemap6);
            }
            if(glIsTexture(texture1))
            {
                glUniform1i(texture1ID,7);
                glActiveTexture(GL_TEXTURE0 + 7);
                glBindTexture(GL_TEXTURE_2D,texture1);
            }
            if(glIsTexture(texture2))
            {
                glUniform1i(texture2ID,8);
                glActiveTexture(GL_TEXTURE0 + 8);
                glBindTexture(GL_TEXTURE_2D,texture2);
            }
            if(glIsTexture(texture3))
            {
               glUniform1i(texture3ID,9);
               glActiveTexture(GL_TEXTURE0 + 9);
               glBindTexture(GL_TEXTURE_2D,texture3);
            }
            if (glIsTexture(textureDepthMap))
            {
                glUniform1i(textureDepthMapID, 10);
                glActiveTexture(GL_TEXTURE0 + 10);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap);
            }
            if (glIsTexture(textureDepthMap1))
            {
                glUniform1i(textureDepthMap1ID, 11);
                glActiveTexture(GL_TEXTURE0 + 11);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap1);
            }
            if (hasSpecularMap)
            {
                glUniform1i(specularTextureID, 12);
                glActiveTexture(GL_TEXTURE0 + 12);
                glBindTexture(GL_TEXTURE_2D, specularTexture);

                glUniform1i(useSpecularMapID, 1);
            }
            else { glUniform1i(useSpecularMapID, 0);}

            m_shader.registerLightToRender(sceneLights,numberOfLight); // MUST BE CALLED if you want lighting to work

            glDrawArrays(GL_TRIANGLES,0,m_nbOfPointToDraw);
            
    }
    void removeSpecularTexture()
    {
        glDeleteTextures(1, &specularTexture);
        hasSpecularMap = false;
    }
    void setSpecularTexture(const char* path)
    {
        setTexture(&specularTexture, path);
        hasSpecularMap = true;
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
            setTexture(&texture1, pathToTexture); 
        }
        else if(m_nbOfTexture == 1)
        {
            setTexture(&texture2,pathToTexture);
        }
        else if(m_nbOfTexture == 2)
        {
            setTexture(&texture3,pathToTexture);
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
            glDeleteTextures(1, &specularTexture);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &vbo_colors);
            glDeleteBuffers(1, &vbo_texCoords);
            glDeleteBuffers(1, &vbo_normals);
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

    bool isInitialized;

    glm::mat4 modelMatrix;

    int m_nbOfPointToDraw;
    int m_nbOfTexture; // Nb of texture created
    int m_nbOfTextureToDraw; // Nb of texture to draw

    GLuint vbo;
    GLuint vbo_colors;
    GLuint vbo_texCoords;
    GLuint vbo_normals;

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
    GLuint textureDepthMapID;
    GLuint textureDepthMap1ID;
    GLuint texture1ID;
    GLuint texture2ID;
    GLuint texture3ID;
    GLuint specularTextureID;

    bool hasSpecularMap;

    GLuint texture1;
    GLuint texture2;
    GLuint texture3;
    GLuint specularTexture;

    GLuint lightSpaceMatrixID;
    GLuint lightSpaceMatrix1ID;
    GLuint lightSpaceMatrixDepthID;

    void setTexture(GLuint *texture, const char* path)
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
