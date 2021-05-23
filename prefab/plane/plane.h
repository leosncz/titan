#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED
#include "../../renderObject/renderObject.h"
class plane : public renderObject
{
public:
    plane(string tag, texturePool* texturePool, float* colors = 0, bool inverseNormals=false) : renderObject(tag)
    {
        float planeVertices[] = {
            -0.5,0.0,0.5,  0.5,0.0,0.5,  0.5,0.0,-0.5,
            0.5,0.0,-0.5,   -0.5,0.0,-0.5,   -0.5,0.0,0.5
        };

        float planeNormals[] = {0,1,0,     0,1,0,      0,1,0,     0,1,0,     0,1,0,     0,1,0};

        float texcoord[] = { 0,0,    1,0,   1,1,  1,1,  0,1, 0,0 };

        if (inverseNormals)
        {
            float badVertices[6][3];
            float badNormals[6][3];
            float badTexCoord[6][2];

            int indice = 0;
            int indiceTex = 0;

            for (int i = 0; i < 6; i++)
            {
                badVertices[i][0] = planeVertices[indice];
                badVertices[i][1] = planeVertices[indice + 1];
                badVertices[i][2] = planeVertices[indice + 2];

                badNormals[i][0] = planeNormals[indice];
                badNormals[i][1] = planeNormals[indice + 1];
                badNormals[i][2] = planeNormals[indice + 2];

                badTexCoord[i][0] = texcoord[indiceTex];
                badTexCoord[i][1] = texcoord[indiceTex + 1];

                indice += 3;
                indiceTex += 2;
            }

            //Now assign values
            indice = 5;
            for (int i = 0; i < 18; i += 3) // Assign normals and vertex
            {
                planeVertices[i] = badVertices[indice][0];
                planeVertices[i + 1] = badVertices[indice][1];
                planeVertices[i + 2] = badVertices[indice][2];

                planeNormals[i] = badNormals[indice][0] * (-1);
                planeNormals[i + 1] = badNormals[indice][1] * (-1);
                planeNormals[i + 2] = badNormals[indice][2] * (-1);

                indice--;
            }
            indice = 5;
            for (int i = 0; i < 12; i += 2) // Assign texcoord
            {
                texcoord[i] = badTexCoord[indice][0];
                texcoord[i + 1] = badTexCoord[indice][1];
                indice--;
            }
        }

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
