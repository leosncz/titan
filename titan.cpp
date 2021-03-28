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
    mainDisplay.init(1920, 1080, 4, true);

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
    triangle2.setSpecularStrenght(1.0);
    triangle2.setAmbiantStrenght(0.0);

    plane myplane = plane();
    myplane.initPrefab(colors, myscene.getTexturePool());
    myplane.setSpecularTexture("specularmap.jpg");
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -0.5, -2));
    myplane.scaleObject(glm::vec3(100, 1, 100));
    myplane.addTexture("texture.jpg");
    myplane.setNumberOfTextureToDraw(1);
    myplane.setTextureResolution(100);
    myplane.setSpecularStrenght(2.0);
    myplane.setAmbiantStrenght(0.1);


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
    mylight5.setData(glm::vec3(0, 4.0, 7.0), glm::vec3(1, 1, 1), glm::vec3(0, -1, -1), 1000.0f);
    mylight5.type = POINT_LIGHT;
    myscene.addLight(&mylight5);

    light mylight6 = light();
    mylight6.setData(glm::vec3(10, 2, 30.0), glm::vec3(10, 10, 0), glm::vec3(0, -1, -1), 1000.0f);
    mylight6.type = POINT_LIGHT;
    myscene.addLight(&mylight6);



    renderObjectScene objScene = renderObjectScene(&myscene);
    objScene.loadOBJFromFile("tree.obj");
    objScene.scaleRenderObjectScene(glm::vec3(0.2, 0.2, 0.2));
    objScene.moveRenderObjectScene(glm::vec3(0, -2.0, -2));

    mainDisplay.hideCursor();

    while (!mainDisplay.shouldExit()) {
        myscene.clearScene();
        myscene.updateCamera();
        myscene.renderScene();
        myscene.refreshScene();
    }

    return 0;
}
