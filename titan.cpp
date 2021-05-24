#include <iostream>
#include "display/display.h"
#include "prefab/plane/plane.h"
#include "prefab/cube/cube.h"
#include "light/light.h"
#include "scene/scene.h"
#include "renderObjectScene/renderObjectScene.h"
#include "cameraFPS/cameraFPS.h"
#include "gui/guiEngine.h"
using namespace std;

int main()
{
    display mainDisplay(1620, 880, 4, false);

    cameraFPS cam("camFPS");
    cam.init(&mainDisplay);


    scene myscene("firstScene", &mainDisplay, &cam);

    guiEngine gui_("guiEngine",&mainDisplay, &myscene);

    plane myplane("bigplane", myscene.getTexturePool());
    myscene.addDrawableObject(&myplane);
    myplane.moveObject(glm::vec3(0, -0.5, -2));
    myplane.scaleObject(glm::vec3(100, 1, 100));
    myplane.addTexture("graphicData/texture.jpg");
    myplane.setTextureResolution(100);
    myplane.setNormalMap("graphicData/normalmap.jpg");

    cube triangle2("tr2",myscene.getTexturePool());
    myscene.addDrawableObject(&triangle2);
    triangle2.moveObject(glm::vec3(-1.5, 0.0, -2));
    triangle2.addTexture("graphicData/texture.jpg");
    triangle2.setNormalMap("graphicData/normalmap.jpg");

    

    cube triangle3("tr3",myscene.getTexturePool());
    myscene.addDrawableObject(&triangle3);
    triangle3.moveObject(glm::vec3(0,0,-2)); // 0 0 1
    triangle3.addTexture("graphicData/PaintedMetal007_1K_Color.jpg");
    triangle3.setNormalMap("graphicData/PaintedMetal007_1K_Normal.jpg");
    triangle3.setRoughnessMap("graphicData/PaintedMetal007_1K_Roughness-min.jpg");
    triangle3.setMetallicMap("graphicData/PaintedMetal007_1K_Metalness-min.jpg");


    cube triangle4("tr4",myscene.getTexturePool());
    myscene.addDrawableObject(&triangle4);
    triangle4.moveObject(glm::vec3(1.2, 0.0, -2));
    triangle4.addTexture("graphicData/texturemetal.jpg");
    triangle4.setNormalMap("graphicData/metalnormalmap.jpg");

    light light_[10];
    int i2 = 0;
    for (int i = -2; i < 2; i+=1)
    {
        light_[i2] = light(POINT_LIGHT, vec3(i, 0.5, 2), vec3(10, 10, 10), vec3(0, -0.3, -1),true);
        myscene.addLight(&light_[i2]);
        i2++;
    }

    while (!mainDisplay.shouldExit()) {
        myscene.renderScene();
        cam.update(true,myscene.getViewMatrix());
        gui_.update(&myscene);

        mainDisplay.updateWindow();
    } 

    return 0;
}
