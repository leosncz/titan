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



    /*cube triangle = cube();
    triangle.initPrefab(colors,myscene.getTexturePool());
    myscene.addDrawableObject(&triangle);
    triangle.moveObject(glm::vec3(0, 0.0, -1));
    triangle.setSpecularStrenght(1.0);
    triangle.setAmbiantStrenght(0.0);*/

    cube triangle2 = cube();
    triangle2.initPrefab(colors, myscene.getTexturePool());
    myscene.addDrawableObject(&triangle2);
    triangle2.moveObject(glm::vec3(5, 0.0, 2));
    triangle2.setSpecularStrenght(3.0);
    triangle2.setAmbiantStrenght(0.0);
    triangle2.addTexture("texture.jpg");
    triangle2.setNumberOfTextureToDraw(1);
    triangle2.setNormalMap("normalmap.jpg");


    plane myplane = plane();
    myplane.initPrefab(colors, myscene.getTexturePool());
    myplane.setSpecularMap("specularmap.jpg");
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -0.5, -2));
    myplane.scaleObject(glm::vec3(100, 1, 100));
    myplane.addTexture("texture.jpg");
    myplane.setNumberOfTextureToDraw(1);
    myplane.setTextureResolution(100);
    myplane.setAmbiantStrenght(0.1);
    myplane.setNormalMap("normalmap.jpg");


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
    mylight5.setData(glm::vec3(3, 1.0, 0.0), glm::vec3(1, 1, 1), glm::vec3(0, -1, -1), 1000.0f);
    mylight5.type = POINT_LIGHT;
    myscene.addLight(&mylight5);

    light mylight6 = light();
    mylight6.setData(glm::vec3(10, 5, 30.0), glm::vec3(1, 1, 0), glm::vec3(0, -1, -1), 1000.0f);
    mylight6.type = POINT_LIGHT;
    myscene.addLight(&mylight6);



    renderObjectScene objScene = renderObjectScene(&myscene);
    objScene.loadOBJFromFile("cottage_obj.obj");
    /*objScene.scaleRenderObjectScene(glm::vec3(0.01, 0.01, 0.01));
    objScene.moveRenderObjectScene(glm::vec3(0, -4.5, -2));
    objScene.setRenderObjectSpecularStrenght(1.5f);
  //  objScene.setRenderObjectNormalMap("normalmap.jpg");
  // objScene.setRenderObjectTextureResolution(30);*/

    mainDisplay.hideCursor();
    while (!mainDisplay.shouldExit()) {
        myscene.clearScene();
        myscene.updateCamera();
        myscene.renderScene();
        myscene.refreshScene();
    } 

    return 0;
}
