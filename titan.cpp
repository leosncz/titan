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
    triangle.setSpecularStrenght(1.0);
    triangle.setAmbiantStrenght(0.1);

    cube triangle2 = cube();
    triangle2.initPrefab(colors, myscene.getTexturePool());
    myscene.addDrawableObject(&triangle2);
    triangle2.moveObject(glm::vec3(-0.5, 1.2, 3));
    triangle2.setSpecularStrenght(1.0);
    triangle2.setAmbiantStrenght(0.1);

    plane myplane = plane();
    myplane.initPrefab(colors, myscene.getTexturePool());
    myplane.setSpecularTexture("specularmap.jpg");
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -0.5, -2));
    myplane.scaleObject(glm::vec3(100, 1, 100));
    myplane.addTexture("texture.jpg");
    myplane.setNumberOfTextureToDraw(1);
    myplane.setTextureResolution(100);
    myplane.setSpecularStrenght(1.0);
    myplane.setAmbiantStrenght(0.3);


    light mylight3 = light();
    mylight3.setData(glm::vec3(-1, 1, 3), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1, -1, -1), 1000.0f);
    mylight3.type = DIRECTIONNAL_LIGHT;
    myscene.addLight(&mylight3);

   /*light mylight4 = light();
    mylight4.setData(glm::vec3(2, 1, 3), glm::vec3(1,1,1), glm::vec3(0, -1, -1), 1000.0f);
    mylight4.type = DIRECTIONNAL_LIGHT;
    myscene.addLight(&mylight4);

    light mylight5 = light();
    mylight5.setData(glm::vec3(6, 1, -3), glm::vec3(1, 1, 1), glm::vec3(0, -1, -1), 1000.0f);
    mylight5.type = DIRECTIONNAL_LIGHT;
    myscene.addLight(&mylight5);*/

    

    /*renderObjectScene objScene = renderObjectScene(&myscene);
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
