#include "renderObjectScene.h"
#include "../modelLoaders/objLoader/objLoader.h"

renderObjectScene::renderObjectScene(scene* linkedScene)
{
    id = rand();
    m_scene = linkedScene;
    cout << "--> Creating renderObjectScene ID=" << id << endl;
}

void renderObjectScene::loadOBJFromFile(const char* path)
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
        renderObject* newObject = new renderObject(curMesh.MeshName);
        if (newObject->setData(&vertices[0], &colors[0], &uv[0], curMesh.Vertices.size(), &normals[0], m_scene->getTexturePool()))
        {
            objects.push_back(newObject);
            newObject->setTag(curMesh.MeshName);
            if (curMesh.MeshMaterial.map_Kd != "") // If there is a diffuse texture
            {
                cout << "---> Detected texture !" << endl;
                objects[objects.size() - 1]->addTexture(curMesh.MeshMaterial.map_Kd.c_str());
            }
            objects[objects.size() - 1]->setDeleteStatus(true);
            objects.push_back(newObject);

            m_scene->addDrawableObject(objects[objects.size() - 1]);
        }
        else
        {
            cout << "--> Error while loading obj mesh !" << endl;
            delete newObject;
        }

    }

}

void renderObjectScene::moveRenderObjectScene(glm::vec3 transation) {
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->moveObjectFromSceneOrigin(transation);
        }
    }
}
void renderObjectScene::scaleRenderObjectScene(glm::vec3 scale) {
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->scaleObject(scale);
        }
    }
}
void renderObjectScene::setRenderObjectRoughness(float value)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->setRoughness(value);
        }
    }
}
void renderObjectScene::setRenderObjectTexture(string path)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->addTexture(path.c_str());
        }
    }
}
void renderObjectScene::setRenderObjectMetallic(float value)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->setMetallic(value);
        }
    }
}
void renderObjectScene::setRenderObjectAmbient(float value)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->setAmbient(value);
        }
    }
}
void renderObjectScene::setRenderObjectTextureResolution(int value)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->setTextureResolution(value);
        }
    }
}
void renderObjectScene::setRenderObjectNormalMap(string path)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->setNormalMap(path.c_str());
        }
    }
}
void renderObjectScene::setRenderObjectRoughnessMap(string path)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->setRoughnessMap(path.c_str());
        }
    }
}
void renderObjectScene::setRenderObjectMetallicMap(string path)
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->setMetallicMap(path.c_str());
        }
    }
}
void renderObjectScene::removeRenderObjectNormalMap()
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->removeNormalMap();
        }
    }
}
void renderObjectScene::removeRenderObjectRoughnessMap()
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->removeRoughnessMap();
        }
    }
}
void renderObjectScene::removeRenderObjectMetallicMap()
{
    if (objects.size() > 0)
    {
        for (int i = 0; i < objects.size(); i++)
        {
            objects[i]->removeMetallicMap();
        }
    }
}