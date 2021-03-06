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
    triangle.initPrefab(colors);
    //myscene.addDrawableObject(&triangle);
    triangle.moveObject(glm::vec3(0, 0.3, -1));
    //triangle.rotateObject(20.0f, glm::vec3(1, 0, 0));
    triangle.setSpecularStrenght(1.0);
    triangle.setSpecularTexture("specularmap.jpg");
    //triangle.removeSpecularTexture();
    triangle.addTexture("texture.jpg");
    triangle.setNumberOfTextureToDraw(1);
    triangle.setSpecularStrenght(3.0);

    // triangle.setAmbiantStrenght(0.5);

    plane myplane = plane();
    myplane.initPrefab(colors);
    myplane.setSpecularTexture("specularmap.jpg");
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -0.5, -2));
    myplane.scaleObject(glm::vec3(100, 1, 100));
    myplane.addTexture("texture.jpg");
    myplane.setNumberOfTextureToDraw(1);
    //float texcoord[] = { 0,0,    10,0,   10,10,  10,10,  0,10, 0,0 };
    myplane.setTextureResolution(100);
    myplane.setSpecularStrenght(3.0);

    light mylight3 = light();
    mylight3.setData(glm::vec3(0, 1, 3), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, -0.5, -1), 1000.0f);
    mylight3.type = SPOT_LIGHT;
    myscene.addLight(&mylight3);
    mylight3.quadratic = 0.019;
    mylight3.linear = 0.022;
    mylight3.constant = 1.0;

    light mylight4 = light();
    mylight4.setData(glm::vec3(0, 1, 1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight4.type = POINT_LIGHT;
    myscene.addLight(&mylight4);
    mylight4.quadratic = 0.09;
    mylight4.linear = 0.032;

    light mylight5 = light();
    mylight5.setData(glm::vec3(2, 1, 1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight5.type = POINT_LIGHT;
    myscene.addLight(&mylight5);
    mylight5.quadratic = 0.09;
    mylight5.linear = 0.032;

    light mylight6 = light();
    mylight6.setData(glm::vec3(-3, 1, -5), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight6.type = POINT_LIGHT;
    myscene.addLight(&mylight6);
    mylight6.quadratic = 0.09;
    mylight6.linear = 0.032;

    renderObjectScene objScene = renderObjectScene(&myscene);
    objScene.loadOBJFromFile("backpack.obj");
    objScene.moveRenderObjectScene(glm::vec3(0, 2, -0.5));

    while (!mainDisplay.shouldExit()) {
        mainDisplay.clearWindow();
        myscene.updateCamera();
        myscene.renderScene();
        mainDisplay.refreshWindow();
    }

    return 0;
}


// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
