#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED
#include "../../renderObject/renderObject.h"
class cube : public renderObject
{
public:
    cube(texturePool* texturePool, float* colors = 0, bool inverseNormals=false, string tag_="Cube" )
    {
        tag = tag_;
        float cubeVertices[] = {
          -0.5,-0.5,0.5,    0.5,-0.5,0.5,    0.5,0.5,0.5,   0.5,0.5,0.5,  -0.5,0.5,0.5,    -0.5,-0.5,0.5,  //front
          -0.5,0.5,0.5,    0.5,0.5,0.5,    0.5,0.5,-0.5,    0.5,0.5,-0.5,   -0.5,0.5,-0.5,   -0.5,0.5,0.5,//top
          -0.5,-0.5,-0.5,  -0.5,-0.5,0.5,  -0.5,0.5,0.5,   -0.5,0.5,0.5,   -0.5,0.5,-0.5,    -0.5,-0.5,-0.5, //left
          0.5,-0.5,0.5,  0.5,-0.5,-0.5,   0.5,0.5,-0.5,   0.5,0.5,-0.5,  0.5,0.5,0.5,   0.5,-0.5,0.5,  //right
          0.5,-0.5,-0.5, -0.5,-0.5,-0.5,  -0.5,0.5,-0.5,  -0.5,0.5,-0.5,  0.5,0.5,-0.5,  0.5,-0.5,-0.5, //back
          0.5,-0.5,0.5,   -0.5,-0.5,0.5,   -0.5,-0.5,-0.5,  -0.5,-0.5,-0.5,  0.5,-0.5,-0.5,  0.5,-0.5,0.5  //bottom
           
        };

        float cubeNormals[] = {0,0,1,   0,0,1,    0,0,1,     0,0,1,     0,0,1,     0,0,1,
                               0,1,0,   0,1,0,   0,1,0,    0,1,0,    0,1,0,      0,1,0,
                               -1,0,0,  -1,0,0,   -1,0,0,  -1,0,0,   -1,0,0,    -1,0,0,
                                1,0,0,  1,0,0,    1,0,0,   1,0,0,   1,0,0,   1,0,0,
                               0,0,-1,  0,0,-1,   0,0,-1,  0,0,-1,   0,0,-1,  0,0,-1,
                                0,-1,0, 0,-1,0,  0,-1,0,   0,-1,0,   0,-1,0,   0,-1,0};

        float texCoords[] = { 0,0,   1,0,    1,1,   1,1,   0,1,    0,0,
                      0,0,   1,0,    1,1,   1,1,   0,1,    0,0,
                      0,0,   1,0,    1,1,   1,1,   0,1,    0,0,
                      0,0,   1,0,    1,1,  1,1,    0,1,    0,0,
                      0,0,   1,0,    1,1,  1,1,    0,1,    0,0,
                      0,0,   1,0,    1,1,  1,1,    0,1,    0,0 };

        if (inverseNormals)
        {
            float badVertices[36][3];
            float badNormals[36][3];
            float badTexCoord[36][2];

            int indice = 0;
            int indiceTex = 0;

            for (int i = 0; i < 36; i++)
            {
                badVertices[i][0] = cubeVertices[indice];
                badVertices[i][1] = cubeVertices[indice + 1];
                badVertices[i][2] = cubeVertices[indice + 2];

                badNormals[i][0] = cubeNormals[indice];
                badNormals[i][1] = cubeNormals[indice + 1];
                badNormals[i][2] = cubeNormals[indice + 2];

                badTexCoord[i][0] = texCoords[indiceTex];
                badTexCoord[i][1] = texCoords[indiceTex + 1];

                indice += 3;
                indiceTex += 2;
            }

            //Now assign values
            indice = 35;
            for (int i = 0; i < 108; i+=3) // Assign normals and vertex
            {
                cubeVertices[i] = badVertices[indice][0];
                cubeVertices[i+1] = badVertices[indice][1];
                cubeVertices[i+2] = badVertices[indice][2];

                cubeNormals[i] = badNormals[indice][0];
                cubeNormals[i + 1] = badNormals[indice][1];
                cubeNormals[i + 2] = badNormals[indice][2];

                indice--;
            }
            indice = 35;
            for (int i = 0; i < 72; i += 2) // Assign texcoord
            {
                texCoords[i] = badTexCoord[indice][0];
                texCoords[i + 1] = badTexCoord[indice][1];
                indice--;
            }
        }


        if (colors == 0)
        {
            float colors_[] = { 1.0,1.0,1.0,   1.0,1.0,1.0,     1.0,1.0,1.0,
                      1.0,1.0,1.0,    1.0,1.0,1.0,    1.0,1.0,1.0,
                    1.0,1.0,1.0,      1.0,1.0,1.0,     1.0,1.0,1.0,
                    1.0,1.0,1.0,       1.0,1.0,1.0,   1.0,1.0,1.0,
                    1.0,1.0,1.0,      1.0,1.0,1.0,     1.0,1.0,1.0,
                    1.0,1.0,1.0,       1.0,1.0,1.0,   1.0,1.0,1.0,
                    1.0,1.0,1.0,      1.0,1.0,1.0,     1.0,1.0,1.0,
                    1.0,1.0,1.0,       1.0,1.0,1.0,   1.0,1.0,1.0,
                    1.0,1.0,1.0,      1.0,1.0,1.0,     1.0,1.0,1.0,
                    1.0,1.0,1.0,       1.0,1.0,1.0,   1.0,1.0,1.0,
                    1.0,1.0,1.0,      1.0,1.0,1.0,     1.0,1.0,1.0,
                    1.0,1.0,1.0,       1.0,1.0,1.0,   1.0,1.0,1.0 };
            setData(cubeVertices, colors_, texCoords, 36, cubeNormals, texturePool);
        }
        else
        {
            setData(cubeVertices, colors, texCoords, 36, cubeNormals, texturePool);
        }
    }
};
#endif // CUBE_H_INCLUDED
