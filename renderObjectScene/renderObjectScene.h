#pragma once
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../renderObject/renderObject.h"
#include "../scene/scene.h"
using namespace std;
class renderObjectScene
{
public:
    renderObjectScene(scene* linkedScene)
    {
        id = rand();
        m_scene = linkedScene;
        nbOfObjects = 0;
        cout << "--> Creating renderObjectScene ID=" << id << endl;
    }
    void loadFromFile(string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "Error in renderObject ID=" << id << "::ASSIMP::" << import.GetErrorString() << endl;
            return;
        }
        processNode(scene->mRootNode, scene);
    }
    renderObject* getObject(int idOrder)
    {
        if (idOrder >= 0 && idOrder <= nbOfObjects - 1)
        {
            return &objects[idOrder];
        }
        std::cout << "--> Error : cannot get Object (idOrder not in range)" << std::endl;
        return 0;
    }
private:
    renderObject objects[100];
    int id;
    int nbOfObjects;
    scene* m_scene;

    void processNode(aiNode* node, const aiScene* scene)
    {
        std::cout << "---> Processing renderObject ID=" << id << std::endl;
        std::cout << "---> Detected " << node->mNumMeshes << " entity in this iteration" << std::endl;
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene);
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    void processMesh(aiMesh* mesh, const aiScene* scene)
    {
        float vertices[500]; // 500 vertex max
        float normals[500];
        float colors[500];
        float texcoords[500];

        std::cout << "---> Processing mesh for renderObject ID=" << id << std::endl;

        for (int i = 0; i < 499; i++) { colors[i] = 1.0; } // Make diffuse color white so it has no incidence on diffuse texture

        int i2 = 0;
        int i3 = 0;
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices[i2] = mesh->mVertices[i].x;
            vertices[i2 + 1] = mesh->mVertices[i].y;
            vertices[i2 + 2] = mesh->mVertices[i].z;
            normals[i2] = mesh->mNormals[i].x;
            normals[i2 + 1] = mesh->mNormals[i].y;
            normals[i2 + 2] = mesh->mNormals[i].z;

            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                texcoords[i3] = (mesh->mTextureCoords[0][i].x);
                texcoords[i3 + 1] = (mesh->mTextureCoords[0][i].y);
            }
            i3 = i3 + 2;
            i2 = i2 + 3;
        }

        if (mesh->mMaterialIndex >= 0)
        {

        }

        //Create the renderObject
        objects[nbOfObjects] = renderObject();
        objects[nbOfObjects].setData(vertices, colors, texcoords, 36, normals);
        //objects[nbOfObjects].setTextureResolution(10, texcoords);
        m_scene->addDrawableObject(&objects[nbOfObjects]);
        nbOfObjects++;
        return;
    }
};