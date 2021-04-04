#include <iostream>
#include "display/display.h"
#include "prefab/plane/plane.h"
#include "prefab/cube/cube.h"
#include "light/light.h"
#include "scene/scene.h"
#include "renderObjectScene/renderObjectScene.h"
using namespace std;

int main()
{
    display mainDisplay = display();
    mainDisplay.init(1620, 780, 4, false);

    scene myscene = scene();
    myscene.setupDisplay(&mainDisplay);


    float colors[] = { 1.0,1.0,1.0,   1.0,1.0,1.0,     1.0,1.0,1.0,
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

    cube triangle2 = cube();
    triangle2.initPrefab(colors, myscene.getTexturePool());
    myscene.addDrawableObject(&triangle2);
    triangle2.moveObject(glm::vec3(5, 0.0, 2));
    triangle2.addTexture("graphicData/texture.jpg");
    triangle2.setNumberOfTextureToDraw(1);
    triangle2.setNormalMap("graphicData/normalmap.jpg");

    plane myplane = plane();
    myplane.initPrefab(colors, myscene.getTexturePool());
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -0.5, -2));
    myplane.scaleObject(glm::vec3(100, 1, 100));
    myplane.addTexture("graphicData/texture.jpg");
    myplane.setNumberOfTextureToDraw(1);
    myplane.setTextureResolution(100);
    myplane.setNormalMap("graphicData/normalmap.jpg");
    myplane.setMetallic(0.5);
    myplane.setRoughness(0.6);

    /*light mylight3 = light();
    mylight3.setData(glm::vec3(0.5,1,4), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, -1, -1), 1000.0f);
    mylight3.type = DIRECTIONNAL_LIGHT;
    myscene.addLight(&mylight3);

    /*light mylight4 = light();
    mylight4.setData(glm::vec3(-2, 2, 0), glm::vec3(1,1,1), glm::vec3(0, -1, -1), 1000.0f);
    mylight4.type = DIRECTIONNAL_LIGHT;
    myscene.addLight(&mylight4);
    /*mylight4.linear = 0.09;
    mylight4.quadratic = 0.032;*/

    light mylight5 = light();
    mylight5.setData(glm::vec3(1, 1.0, 2), glm::vec3(10, 10, 10), glm::vec3(0, -1, -1), 1000.0f);
    mylight5.type = POINT_LIGHT;
    myscene.addLight(&mylight5);

    light mylight6 = light();
    mylight6.setData(glm::vec3(10, 5, 30.0), glm::vec3(30, 30, 0), glm::vec3(0, -1, -1), 1000.0f);
    mylight6.type = POINT_LIGHT;
    myscene.addLight(&mylight6);



    /*renderObjectScene objScene = renderObjectScene(&myscene);
    objScene.loadOBJFromFile("Container Games.obj");
    objScene.scaleRenderObjectScene(glm::vec3(0.08, 0.08, 0.08));
    objScene.moveRenderObjectScene(glm::vec3(0, -5.5, -2));
    objScene.setRenderObjectMetallicMap("Container LP Complekt_DefaultMaterial_MetallicSmoothness.png");
    objScene.setRenderObjectNormalMap("Container LP Complekt_DefaultMaterial_Normal.png");
    objScene.setRenderObjectTexture("Container LP Complekt_DefaultMaterial_AlbedoTransparency.png");*/

    cube triangle3 = cube();
    triangle3.initPrefab(colors, myscene.getTexturePool());
    myscene.addDrawableObject(&triangle3);
    triangle3.moveObject(glm::vec3(0,0,1)); // 0 0 1
    triangle3.addTexture("graphicData/texturemetal.jpg");
    triangle3.setNumberOfTextureToDraw(1);
    triangle3.setNormalMap("graphicData/metalnormalmap.jpg");
    triangle3.setRoughness(0.4);
    triangle3.setMetallic(0.1);
    //triangle3.setMetallicMap("graphicData/metalmap.jpg");
    //triangle3.setMetallicMap("graphicData/metalmap.jpg");
    //triangle3.setRoughness(0.1);

    cube triangle4 = cube();
    triangle4.initPrefab(colors, myscene.getTexturePool());
    myscene.addDrawableObject(&triangle4);
    triangle4.moveObject(glm::vec3(0, 0.0, 3));
    triangle4.addTexture("graphicData/texturemetal.jpg");
    triangle4.setNumberOfTextureToDraw(1);
    triangle4.setNormalMap("graphicData/metalnormalmap.jpg");
    //triangle4.setMetallicMap("graphicData/metalmap.jpg");
    triangle4.setRoughness(0.1);

    mainDisplay.hideCursor();

    //myscene.setExposure(3.0);
    //myscene.setGamma(1.0);

    while (!mainDisplay.shouldExit()) {
        myscene.clearScene();
        myscene.updateCamera();
        myscene.renderScene();
        myscene.refreshScene();
    } 

    return 0;
}
