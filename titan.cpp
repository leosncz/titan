// titan.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

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
    mainDisplay.init(1024, 768, 4);

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



    cube triangle = cube();
    triangle.initPrefab(colors,myscene.getTexturePool());
    myscene.addDrawableObject(&triangle);
    triangle.moveObject(glm::vec3(0, 0.0, -1));
    //triangle.addTexture("texture2.jpg");
    //triangle.setNumberOfTextureToDraw(1);
    //triangle.setSpecularTexture("specularmap2.jpg");
    triangle.setSpecularStrenght(3.0);
    triangle.setAmbiantStrenght(0.3);

    plane myplane = plane();
    myplane.initPrefab(colors, myscene.getTexturePool());
    myplane.setSpecularTexture("specularmap.jpg");
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -0.5, -2));
    myplane.scaleObject(glm::vec3(10, 1, 10));
    myplane.addTexture("texture.jpg");
    myplane.setNumberOfTextureToDraw(1);
    myplane.setTextureResolution(10);
    myplane.setSpecularStrenght(2.0);
    myplane.setAmbiantStrenght(0.3);

    /*light mylight3 = light();
    mylight3.setData(glm::vec3(0, 1, 3), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, -0.5, -1), 1000.0f);
    mylight3.type = SPOT_LIGHT;
    myscene.addLight(&mylight3);
    mylight3.quadratic = 0.019;
    mylight3.linear = 0.022;
    mylight3.constant = 1.0;*/

    light mylight4 = light();
    mylight4.setData(glm::vec3(0, 1, 3), glm::vec3(1,1,1), glm::vec3(0, -1, -1), 1000.0f);
    mylight4.type = DIRECTIONNAL_LIGHT;
    myscene.addLight(&mylight4);
    //mylight4.quadratic = 0.09;
    //mylight4.linear = 0.032;

    /*light mylight5 = light();
    mylight5.setData(glm::vec3(1, 0.0, 0.0), glm::vec3(0.0, 1.0, 1.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight5.type = POINT_LIGHT;
    myscene.addLight(&mylight5);
    /*mylight5.quadratic = 0.09;
    mylight5.linear = 0.032;*/

    /*light mylight6 = light();
    mylight6.setData(glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight6.type = POINT_LIGHT;
    myscene.addLight(&mylight6);
    /*mylight6.quadratic = 0.09;
    mylight6.linear = 0.032;*/

   /* renderObjectScene objScene = renderObjectScene(&myscene);
    objScene.loadOBJFromFile("ironman.obj");
    objScene.moveRenderObjectScene(glm::vec3(0, 0.1, -2));
    objScene.scaleRenderObjectScene(glm::vec3(0.1, 0.1, 0.1));
    objScene.setRenderObjectSpecularStrenght(2.0);*/

    while (!mainDisplay.shouldExit()) {
        mainDisplay.clearWindow();
        myscene.updateCamera();
        myscene.renderScene();
        mainDisplay.refreshWindow();
    }

    return 0;
}
