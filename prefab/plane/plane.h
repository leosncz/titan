#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED
#include "../../renderObject/renderObject.h"
class plane : public renderObject
{
public:
    void initPrefab(float* colors, texturePool* texturePool)
    {
        tag = "Plane";
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
