/* Author: leosncz
Handles everything related to drawable nodes
*/
#pragma once
#include "../shader/shader.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "../texturePool/texturePool.h"
class renderObject
{
public:
    renderObject(string tag="new object");
    string getTag();
    void setTag(string tag_);
    shader* getShader();

    bool setData(float* vertices, float* colors, float* texCoord, int nbOfPointToDraw, float* normals = 0, texturePool* texturePool_ = 0);
    void setTexturePool(texturePool* texPool);

    void renderDepth(glm::mat4* projection, glm::mat4* view, glm::mat4* model, light* lightToRender, float farShadow = 25.0f);
    void renderGBuffer(glm::mat4* projection, glm::mat4* view, glm::mat4* model, glm::vec3 viewPos, vector<light*> sceneLights = {}, int numberOfLight = 0);

    void updateVertex(float* vertices, int nbOfPointToDraw);
    void setTextureResolution(int res);
   
    void addTexture(const char* pathToTexture);

    void moveObject(glm::vec3 position);
    void moveObjectFromSceneOrigin(glm::vec3 position);
    void rotateObjectFromLastRotation(float angle, glm::vec3 factors);
    void rotateObject(float angle, glm::vec3 factors);
    void scaleObject(glm::vec3 factors);

    ~renderObject();

    //Material properties edition
    void setRoughness(float intensity);
    void setMetallic(float intensity);
    void setAmbient(float intensity);
    void removeNormalMap();
    void setNormalMap(const char* path);
    void removeMetallicMap();
    void setMetallicMap(const char* path);
    void removeRoughnessMap();
    void setRoughnessMap(const char* path);

    float* getPosition();
    float* getScale();

    void removeTexture1();
    void removeTexture2();

    bool doesMeshHasNormalMap();
    bool doesMeshHasRoughnessMap();
    bool doesMeshHasMetallicMap();
    GLuint getNormalMap();
    GLuint getMetallicMap();
    GLuint getRoughnessMap();
    GLuint* getTextures();
    int getNbOfTextures();
    void setDeleteStatus(bool status);
    bool getDeleteStatus();

protected:

    string m_tag;

    float m_position[3];
    float m_scale[3];

    bool isInitialized;
    bool mustBeDeleted; // Indicates if this renderObject has been dynamically created on the heap, if yes the scene will delete it

    glm::mat4 positionMatrix;
    glm::mat4 rotationMatrix;
    glm::mat4 scaleMatrix;


    int m_nbOfPointToDraw; // Nb of point to draw
    int m_nbOfTexture; // Nb of texture created

    GLuint vbo, vbo_colors, vbo_texCoords, vbo_normals, vbo_tangent, vbo_bitangent;

    vector<float> m_texCoords;

    texturePool* m_texturePool; //Associated texture pool that contains cached texture

    GLuint vao;
    shader m_depthShader;
    shader m_gShader;

    bool hasNormalMap;
    bool hasMetallicMap;
    bool hasRoughnessMap;

    GLuint texture1, texture2, normalTexture, roughnessTexture, metallicTexture;

    // Used to compute tangent and bitangent for normal mapping
    void computeTangent(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);
    int isMultipleOf3(int n);
    void setTexture(GLuint* texture, const char* path, bool isDiffuseTexture = false);

};
