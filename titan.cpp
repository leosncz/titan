// titan.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "display/display.h"
#include "prefab/plane/plane.h"
#include "light/light.h"
#include "scene/scene.h"
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



    plane triangle = plane();
    triangle.initPrefab(colors);
    myscene.addDrawableObject(&triangle);
    triangle.moveObject(glm::vec3(0, 0, -1));
    triangle.rotateObject(20.0f, glm::vec3(1, 0, 0));
    triangle.setSpecularStrenght(1.0);
    triangle.setSpecularTexture("specularmap.jpg");
    triangle.removeSpecularTexture();

    // triangle.setAmbiantStrenght(0.5);

    plane myplane = plane();
    myplane.initPrefab(colors);
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -1, -2));
    myplane.scaleObject(glm::vec3(10, 10, 10));
    myplane.addTexture("texture.jpg");
    myplane.setNumberOfTextureToDraw(1);
    float texcoord[] = { 0,0,    0,1,      1,1,      1,1,     1,0,    0,0 };
    myplane.setTextureResolution(20, texcoord);

    light mylight2 = light();
    mylight2.setData(glm::vec3(0, 0, 0), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, -1, -1), 1000.0f);
    mylight2.type = DIRECTIONNAL_LIGHT;
   // myscene.addLight(&mylight2);

    light mylight3 = light();
    mylight3.setData(glm::vec3(-2, 1, 1), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight3.type = POINT_LIGHT;
    myscene.addLight(&mylight3);
    mylight3.quadratic = 0.09;
    mylight3.linear = 0.032;

    light mylight4 = light();
    mylight4.setData(glm::vec3(0, 1, 1), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight4.type = POINT_LIGHT;
    myscene.addLight(&mylight4);
    mylight4.quadratic = 0.09;
    mylight4.linear = 0.032;

    light mylight5 = light();
    mylight5.setData(glm::vec3(2, 1, 1), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0, 0, -1), 1000.0f);
    mylight5.type = POINT_LIGHT;
    myscene.addLight(&mylight5);
    mylight5.quadratic = 0.09;
    mylight5.linear = 0.032;


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
