#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../scene/scene.h"

using namespace std;
class renderObjectScene
{
public:
    renderObjectScene(scene* linkedScene);
    void loadOBJFromFile(const char* path);

    void moveRenderObjectScene(glm::vec3 transation);
    void scaleRenderObjectScene(glm::vec3 scale);
    void setRenderObjectRoughness(float value);
    void setRenderObjectTexture(string path);
    void setRenderObjectMetallic(float value);
    void setRenderObjectAmbient(float value);
    void setRenderObjectTextureResolution(int value);
    void setRenderObjectNormalMap(string path);
    void setRenderObjectRoughnessMap(string path);
    void setRenderObjectMetallicMap(string path);
    void removeRenderObjectNormalMap();
    void removeRenderObjectRoughnessMap();
    void removeRenderObjectMetallicMap();

private:
    vector<renderObject*> objects;
    int id;
    scene* m_scene;
};