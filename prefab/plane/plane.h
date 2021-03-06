#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED
#include "../../renderObject/renderObject.h"
class plane : public renderObject
{
public:
    plane(){
        id = rand();
        std::cout << "--> Creating new plane prefab ID=" << id << std::endl;
    }

    void initPrefab(float* colors, texturePool* texturePool)
    {
        float planeVertices[] = {
            -0.5,0.0,0.5,  0.5,0.0,0.5,  0.5,0.0,-0.5,
            0.5,0.0,-0.5,   -0.5,0.0,-0.5,   -0.5,0.0,0.5
        };

        float planeNormals[] = {0,1,0,     0,1,0,      0,1,0,     0,1,0,     0,1,0,     0,1,0};
        float texcoord[] = { 0,0,    1,0,   1,1,  1,1,  0,1, 0,0};
        
        setData(planeVertices,colors,texcoord,6,planeNormals, texturePool);
    }
};
#endif 
