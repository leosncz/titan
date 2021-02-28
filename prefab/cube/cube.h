#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED
#include "../../renderObject/renderObject.h"
class cube : public renderObject
{
public:
    cube(){
        id = rand();
        std::cout << "--> Creating new cube prefab ID=" << id << std::endl;
    }

    ~cube()
    {
        std::cout << "--> Deleting cube prefab ID=" << id << std::endl;
    }

    void initPrefab(float* colors)
    {
        float cubeVertices[] = {
          -0.5,-0.5,0.5,    0.5,-0.5,0.5,    0.5,0.5,0.5,   0.5,0.5,0.5,  -0.5,0.5,0.5,    -0.5,-0.5,0.5,  //front
          -0.5,0.5,0.5,    0.5,0.5,0.5,    0.5,0.5,-0.5,    0.5,0.5,-0.5,   -0.5,0.5,-0.5,   -0.5,0.5,0.5,//top
          -0.5,-0.5,-0.5,  -0.5,-0.5,0.5,  -0.5,0.5,0.5,   -0.5,0.5,0.5,   -0.5,0.5,-0.5,    -0.5,-0.5,-0.5, //left
          0.5,-0.5,0.5,  0.5,-0.5,-0.5,   0.5,0.5,-0.5,   0.5,0.5,-0.5,  0.5,0.5,0.5,   0.5,-0.5,0.5  //right
            //back
            //bottom
           
        };

        float cubeNormals[] = {0,0,1,   0,0,1,    0,0,1,     0,0,1,     0,0,1,     0,0,1,
                               0,1,0,   0,1,0,   0,1,0,    0,1,0,    0,1,0,      0,1,0,
                               -1,0,0,  -1,0,0,   -1,0,0,  -1,0,0,   -1,0,0,    -1,0,0,
                                1,0,0,  1,0,0,    1,0,0,   1,0,0,   1,0,0,   1,0,0
                               };

        float texCoords[] = { 0,0,   1,0,    1,1,   1,1,   0,1,    0,0,
                              0,0,   1,0,    1,1,   1,1,   0,1,    0,0,
                              0,0,   1,0,    1,1,   1,1,   0,1,    0,0,
                              0,0,   1,0,    1,1,  1,1,    0,1,    0,0};

        setData(cubeVertices,colors,texCoords,24,cubeNormals);
    }
};
#endif // CUBE_H_INCLUDED
