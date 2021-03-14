#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED
#include "../../renderObject/renderObject.h"
class cube : public renderObject
{
public:
    void initPrefab(float* colors, texturePool* texturePool)
    {
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
                              0,0,   1,0,    1,1,  1,1,    0,1,    0,0};

        setData(cubeVertices,colors,texCoords,36,cubeNormals,texturePool);
    }
};
#endif // CUBE_H_INCLUDED
