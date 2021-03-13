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
    renderObject() { isInitialized = false; }
    void setData(float* vertices, float* colors, float* texCoords, int nbOfPointToDraw, float* normals=0, texturePool* texturePool=0) // Use this methode to define the mesh by hand
    {
        //Initialize members
        id = rand();
        std::cout << "--> Creating new renderObject ID=" << id << std::endl;
        init(vertices,colors,normals,texCoords,nbOfPointToDraw,texturePool);
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

        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f,30.0f);
        glm::mat4 lightView = glm::lookAt(lightToRender->lightPosition, glm::vec3(0,0,0), glm::vec3(0, 1, 0));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        glUniformMatrix4fv(lightSpaceMatrixDepthID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        
        glDrawArrays(GL_TRIANGLES, 0, m_nbOfPointToDraw);
    }
    void render(glm::mat4 *projection, glm::mat4 *view, glm::mat4 *model, glm::vec3 viewPos, light* sceneLights[]=0, int numberOfLight=0, GLuint textureDepthMap=0, GLuint textureDepthMap1 = 0, GLuint textureDepthMap2 = 0, GLuint textureDepthMap3 = 0, GLuint textureDepthMap4 = 0, GLuint textureDepthMap5 = 0, GLuint textureDepthMap6 = 0)
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
                glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 30.0f);

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
                if (numberOfLight >= 3 && sceneLights[2]->type == DIRECTIONNAL_LIGHT && sceneLights[2]->computeShadows)
                {
                    glm::mat4 lightView = glm::lookAt(sceneLights[2]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(lightSpaceMatrix2ID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
                }
                if (numberOfLight >= 4 && sceneLights[3]->type == DIRECTIONNAL_LIGHT && sceneLights[3]->computeShadows)
                {
                    glm::mat4 lightView = glm::lookAt(sceneLights[3]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(lightSpaceMatrix3ID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
                }
                if (numberOfLight >= 5 && sceneLights[4]->type == DIRECTIONNAL_LIGHT && sceneLights[4]->computeShadows)
                {
                    glm::mat4 lightView = glm::lookAt(sceneLights[4]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(lightSpaceMatrix4ID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
                }
                if (numberOfLight >= 6 && sceneLights[5]->type == DIRECTIONNAL_LIGHT && sceneLights[5]->computeShadows)
                {
                    glm::mat4 lightView = glm::lookAt(sceneLights[5]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(lightSpaceMatrix5ID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
                }
                if (numberOfLight == 7 && sceneLights[6]->type == DIRECTIONNAL_LIGHT && sceneLights[6]->computeShadows)
                {
                    glm::mat4 lightView = glm::lookAt(sceneLights[6]->lightPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
                    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
                    glUniformMatrix4fv(lightSpaceMatrix6ID, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
                }
            }

            if(glIsTexture(texture1))
            {
                glUniform1i(texture1ID,0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,texture1);
            }
            if(glIsTexture(texture2))
            {
                glUniform1i(texture2ID,1);
                glActiveTexture(GL_TEXTURE0 + 1);
                glBindTexture(GL_TEXTURE_2D,texture2);
            }
            if(glIsTexture(texture3))
            {
               glUniform1i(texture3ID,2);
               glActiveTexture(GL_TEXTURE0 + 2);
               glBindTexture(GL_TEXTURE_2D,texture3);
            }
            if(glIsTexture(texture4))
            {
               glUniform1i(texture4ID,3);
               glActiveTexture(GL_TEXTURE0 + 3);
               glBindTexture(GL_TEXTURE_2D,texture4);
            }
            if (glIsTexture(textureDepthMap))
            {
                glUniform1i(textureDepthMapID, 4);
                glActiveTexture(GL_TEXTURE0 + 4);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap);
            }
            if (glIsTexture(textureDepthMap1))
            {
                glUniform1i(textureDepthMap1ID, 5);
                glActiveTexture(GL_TEXTURE0 + 5);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap1);
            }
            if (glIsTexture(textureDepthMap2))
            {
                glUniform1i(textureDepthMap2ID, 6);
                glActiveTexture(GL_TEXTURE0 + 6);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap2);
            }
            if (glIsTexture(textureDepthMap3))
            {
                glUniform1i(textureDepthMap3ID, 7);
                glActiveTexture(GL_TEXTURE0 + 7);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap3);
            }
            if (glIsTexture(textureDepthMap4))
            {
                glUniform1i(textureDepthMap4ID, 8);
                glActiveTexture(GL_TEXTURE0 + 8);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap4);
            }
            if (glIsTexture(textureDepthMap5))
            {
                glUniform1i(textureDepthMap5ID, 9);
                glActiveTexture(GL_TEXTURE0 + 9);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap5);
            }
            if (glIsTexture(textureDepthMap6))
            {
                glUniform1i(textureDepthMap6ID, 10);
                glActiveTexture(GL_TEXTURE0 + 10);
                glBindTexture(GL_TEXTURE_2D, textureDepthMap6);
            }
            if (hasSpecularMap)
            {
                glUniform1i(specularTextureID, 11);
                glActiveTexture(GL_TEXTURE0 + 11);
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
        else if(m_nbOfTexture == 3)
        {
            setTexture(&texture4,pathToTexture);
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
            glDeleteTextures(1, &texture4);
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
    GLuint textureDepthMap2ID;
    GLuint textureDepthMap3ID;
    GLuint textureDepthMap4ID;
    GLuint textureDepthMap5ID;
    GLuint textureDepthMap6ID;
    GLuint texture1ID;
    GLuint texture2ID;
    GLuint texture3ID;
    GLuint texture4ID;
    GLuint specularTextureID;

    bool hasSpecularMap;

    GLuint texture1;
    GLuint texture2;
    GLuint texture3;
    GLuint texture4;
    GLuint specularTexture;

    GLuint lightSpaceMatrixID;
    GLuint lightSpaceMatrix1ID;
    GLuint lightSpaceMatrix2ID;
    GLuint lightSpaceMatrix3ID;
    GLuint lightSpaceMatrix4ID;
    GLuint lightSpaceMatrix5ID;
    GLuint lightSpaceMatrix6ID;
    GLuint lightSpaceMatrixDepthID;


    void init(float* vertices, float* colors, float* normals, float* texCoord, int nbOfPointToDraw, texturePool* texturePool)
    {
        m_nbOfPointToDraw = nbOfPointToDraw;
        modelMatrix = glm::mat4(1.0);
        m_nbOfTexture = 0;
        m_nbOfTextureToDraw = 0;
        hasSpecularMap = false;
        isInitialized = true;
        m_texturePool = texturePool;
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
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);

            glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,NULL);

            glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,0,NULL);

            modelID = glGetUniformLocation(m_shader.getShaderID(), "model");
            viewID = glGetUniformLocation(m_shader.getShaderID(), "view");
            projectionID = glGetUniformLocation(m_shader.getShaderID(), "projection");
            viewPosID = glGetUniformLocation(m_shader.getShaderID(),"viewPos");
            texture1ID = glGetUniformLocation(m_shader.getShaderID(),"texture1");
            texture2ID = glGetUniformLocation(m_shader.getShaderID(),"texture2");
            texture3ID = glGetUniformLocation(m_shader.getShaderID(),"texture3");
            texture4ID = glGetUniformLocation(m_shader.getShaderID(),"texture4");
            specularTextureID = glGetUniformLocation(m_shader.getShaderID(), "specularMap");
            textureDepthMapID = glGetUniformLocation(m_shader.getShaderID(),"textureDepthMap");
            textureDepthMap1ID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap1");
            textureDepthMap2ID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap2");
            textureDepthMap3ID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap3");
            textureDepthMap4ID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap4");
            textureDepthMap5ID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap5");
            textureDepthMap6ID = glGetUniformLocation(m_shader.getShaderID(), "textureDepthMap6");
            howManyTexID = glGetUniformLocation(m_shader.getShaderID(),"howManyTex");
            useSpecularMapID = glGetUniformLocation(m_shader.getShaderID(), "useSpecularMap");
            lightSpaceMatrixID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix");
            lightSpaceMatrix1ID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix1");
            lightSpaceMatrix2ID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix2");
            lightSpaceMatrix3ID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix3");
            lightSpaceMatrix4ID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix4");
            lightSpaceMatrix5ID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix5");
            lightSpaceMatrix6ID = glGetUniformLocation(m_shader.getShaderID(), "lightSpaceMatrix6");
            lightSpaceMatrixDepthID = glGetUniformLocation(m_depthShader.getShaderID(), "lightSpaceMatrix");
    }

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
