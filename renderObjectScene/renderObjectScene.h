#pragma once
#include <iostream>
#include <string>
#include "../renderObject/renderObject.h"
#include "../scene/scene.h"
#include <vector>
#include "../modelLoaders/objLoader/objLoader.h"
#include <glm.hpp>
using namespace std;
class renderObjectScene
{
public:
    renderObjectScene(scene* linkedScene)
    {
        id = rand();
        m_scene = linkedScene;
        cout << "--> Creating renderObjectScene ID=" << id << endl;
    }
    void loadOBJFromFile(const char* path)
    {
        std::cout << "--> Processing " << path << " obj file..." << std::endl;
        objl::Loader Loader;
        bool loadout = Loader.LoadFile(path);
        if (loadout == false) { std::cout << "Error loading obj " << path << std::endl; return; }
        std::cout << "---> Detected " << Loader.LoadedMeshes.size() << " entity !" << std::endl;

        for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = Loader.LoadedMeshes[i];
            vector<float> vertices;
            vector<float> colors;
            vector<float> normals;
            vector<float> uv;
            std::cout << "---> Working on " << curMesh.MeshName << std::endl;
            for (int j = 0; j < curMesh.Vertices.size(); j++)
            {
                vertices.push_back(curMesh.Vertices[j].Position.X);
                vertices.push_back(curMesh.Vertices[j].Position.Y);
                vertices.push_back(curMesh.Vertices[j].Position.Z);
                normals.push_back(curMesh.Vertices[j].Normal.X);
                normals.push_back(curMesh.Vertices[j].Normal.Y);
                normals.push_back(curMesh.Vertices[j].Normal.Z);
                uv.push_back(curMesh.Vertices[j].TextureCoordinate.X);
                uv.push_back(curMesh.Vertices[j].TextureCoordinate.Y);
                colors.push_back(curMesh.MeshMaterial.Kd.X); //r
                colors.push_back(curMesh.MeshMaterial.Kd.Y); //g
                colors.push_back(curMesh.MeshMaterial.Kd.Z); //b
                
            }
            objects.push_back(new renderObject());
            objects[objects.size() - 1]->setData(&vertices[0], &colors[0], &uv[0], curMesh.Vertices.size(), &normals[0],m_scene->getTexturePool());
            if (curMesh.MeshMaterial.map_Kd != "") // If there is a diffuse texture
            {
                objects[objects.size() - 1]->addTexture(curMesh.MeshMaterial.map_Kd.c_str());
                objects[objects.size() - 1]->setNumberOfTextureToDraw(1);
            }
            m_scene->addDrawableObject(objects[objects.size() - 1]);
        }
    }
    void moveRenderObjectScene(glm::vec3 transation) {
        if (objects.size() > 0)
        {
            for (int i = 0; i < objects.size(); i++)
            {
                objects[i]->moveObject(transation);
            }
        }
    }
    void scaleRenderObjectScene(glm::vec3 scale) {
        if (objects.size() > 0)
        {
            for (int i = 0; i < objects.size(); i++)
            {
                objects[i]->scaleObject(scale);
            }
        }
    }
    void setRenderObjectSpecularStrenght(int value)
    {
        if (objects.size() > 0)
        {
            for (int i = 0; i < objects.size(); i++)
            {
                objects[i]->setSpecularStrenght(value);
            }
        }
    }
    void setRenderObjectAmbiantStrenght(int value)
    {
        if (objects.size() > 0)
        {
            for (int i = 0; i < objects.size(); i++)
            {
                objects[i]->setAmbiantStrenght(value);
            }
        }
    }
    ~renderObjectScene()
    {
        std::cout << "--> Destroying renderObjectScene ID=" << id << std::endl;
        if (objects.size() > 0)
        {
            for (int i = 0; i < objects.size() - 1; i++)
            {
                delete objects[i];
            }
        }
    }
private:
    vector<renderObject*> objects;
    int id;
    scene* m_scene;
};