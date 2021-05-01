#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED
#include "../../renderObject/renderObject.h"
class plane : public renderObject
{
public:
    plane(texturePool* texturePool, float* colors = 0, bool inverseNormals=false, string tag_="Plane")
    {
        tag = tag_;
        float planeVertices[] = {
            -0.5,0.0,0.5,  0.5,0.0,0.5,  0.5,0.0,-0.5,
            0.5,0.0,-0.5,   -0.5,0.0,-0.5,   -0.5,0.0,0.5
        };

        float planeNormals[] = {0,1,0,     0,1,0,      0,1,0,     0,1,0,     0,1,0,     0,1,0};
        if (inverseNormals)
        {
            for (int i = 0; i < 18; i++)
            {
                planeNormals[i] = planeNormals[i] * (-1);
            }
        }
        float texcoord[] = { 0,0,    1,0,   1,1,  1,1,  0,1, 0,0};

        if (colors == 0)
        {
            float colors_[] = { 1.0,1.0,1.0,   1.0,1.0,1.0,     1.0,1.0,1.0,
                      1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0 };
            setData(planeVertices, colors_, texcoord, 6, planeNormals, texturePool);
        }
        else
        {
            setData(planeVertices, colors, texcoord, 6, planeNormals, texturePool);
        }
        
        
    }
};
#endif 
