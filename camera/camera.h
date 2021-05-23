#pragma once
#include "../display/display.h"
#include <glm.hpp>
#include <string>
using namespace std;
class camera
{
public:
    camera(string tag);
    glm::vec3 getCameraPos();
    virtual void update(bool isAzerty = true, glm::mat4* viewMatrix=0) = 0;
    virtual void init(display* display_) = 0;
    void pauseControls();
    void resumeControls();

    bool getIsPaused();

protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
    bool isPaused;

    display* m_display;
    glm::mat4 *m_viewMatrix;

    string m_tag;
};