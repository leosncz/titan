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
//List all shader implemented
#define SHADER_DEFAULT "default"

class renderObject
{
public:
    void setData(float* vertices, float* colors, float* texCoords, int nbOfPointToDraw, float* normals=0)
    {
        // Init variables
        id = rand();
        std::cout << "--> Creating new renderObject ID=" << id << std::endl;
        //Initialize members
        m_nbOfPointToDraw = nbOfPointToDraw;
        modelMatrix = glm::mat4(1.0);
        init(vertices,colors,normals,texCoords);
    }

    void render(glm::mat4 *projection, glm::mat4 *view, glm::mat4 *model, glm::vec3 viewPos, light* sceneLights[]=0, int numberOfLight=0, GLuint textureDepthMap=0)
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
            if(glIsTexture(textureDepthMap))
            {
                glUniform1i(textureDepthMapID,0);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,textureDepthMap);
            }
            if(glIsTexture(texture1))
            {
                glUniform1i(texture1ID,1);
                glActiveTexture(GL_TEXTURE0 + 1);
                glBindTexture(GL_TEXTURE_2D,texture1);
            }
            if(glIsTexture(texture2))
            {
                glUniform1i(texture2ID,2);
                glActiveTexture(GL_TEXTURE0 + 2);
                glBindTexture(GL_TEXTURE_2D,texture2);
            }
            if(glIsTexture(texture3))
            {
               glUniform1i(texture3ID,3);
               glActiveTexture(GL_TEXTURE0 + 3);
               glBindTexture(GL_TEXTURE_2D,texture3);
            }
            if(glIsTexture(texture4))
            {
               glUniform1i(texture4ID,4);
               glActiveTexture(GL_TEXTURE0 + 4);
               glBindTexture(GL_TEXTURE_2D,texture4);
            }
            if (hasSpecularMap)
            {
                glUniform1i(specularTextureID, 5);
                glActiveTexture(GL_TEXTURE0 + 5);
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
    void setTextureResolution(int res, float* texCoords=0)
    {

        for(int i = 0;i<m_nbOfPointToDraw*2;i++)
        {
            texCoords[i] = texCoords[i]*res;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * m_nbOfPointToDraw * sizeof(float), texCoords);
    }

    void addTexture(const char* pathToTexture) // Not all texture must be used for drawing onto the node (for example we can use it for depth test)
    {
        if(m_nbOfTexture == 0)
        {
            setTexture(&texture1,pathToTexture);
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
        std::cout << "--> Destroying renderObject ID=" << id << std::endl;
        glDeleteTextures(1,&texture1);
        glDeleteTextures(1,&texture2);
        glDeleteTextures(1,&texture3);
        glDeleteTextures(1,&texture4);
        glDeleteTextures(1, &specularTexture);
        glDeleteBuffers(1,&vbo);
        glDeleteBuffers(1,&vbo_colors);
        glDeleteBuffers(1,&vbo_texCoords);
        glDeleteBuffers(1,&vbo_normals);
        glDeleteVertexArrays(1,&vao);
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
    int getNBOfTextureCreated(){return m_nbOfTexture;}

protected:

    int id;

    glm::mat4 modelMatrix;

    int m_nbOfPointToDraw;
    int m_nbOfTexture; // Nb of texture created
    int m_nbOfTextureToDraw; // Nb of texture to draw

    GLuint vbo;
    GLuint vbo_colors;
    GLuint vbo_texCoords;
    GLuint vbo_normals;

    GLuint vao;
    shader m_shader;

    GLuint modelID;
    GLuint viewID;
    GLuint viewPosID;
    GLuint projectionID;
    GLuint howManyTexID;
    GLuint useSpecularMapID;
    GLuint textureDepthMapID;
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


    void init(float* vertices, float* colors, float* normals, float* texCoord)
    {
        modelMatrix = glm::mat4(1.0);
        m_nbOfTexture = 0;
        m_nbOfTextureToDraw = 0;
        hasSpecularMap = false;

            m_shader.compileDefaultShader();

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), vertices, GL_STATIC_DRAW);

            glGenBuffers(1, &vbo_colors);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
            glBufferData(GL_ARRAY_BUFFER, 3 * m_nbOfPointToDraw * sizeof(float), colors, GL_STATIC_DRAW);

            glGenBuffers(1, &vbo_texCoords);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoords);
            glBufferData(GL_ARRAY_BUFFER, 2 * m_nbOfPointToDraw * sizeof(float), texCoord, GL_STATIC_DRAW);

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
            howManyTexID = glGetUniformLocation(m_shader.getShaderID(),"howManyTex");
            useSpecularMapID = glGetUniformLocation(m_shader.getShaderID(), "useSpecularMap");
    }

    void setTexture(GLuint *texture, const char* path)
    {
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
    }

};
#endif // RENDEROBJECT_H_INCLUDED
