#pragma once
#pragma once
#include <iostream>
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include "../display/display.h"
#include "../gui/gui.h"

class camera
{
public:
    camera()
    {
        id = rand();
        cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraSpeed = 0.1f;
        isPaused = false;
    }

    glm::vec3 getCameraPos() { return cameraPos; }

    virtual void update(display* display, glm::mat4* viewMatrix, bool isAzerty = true) = 0;
    virtual void init(display* display_, gui* gui_) = 0;
    virtual void pauseControls() = 0;
    virtual void resumeControls() = 0;

    bool getIsPaused() { return isPaused; }

protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
    bool isPaused;

    display* m_display;

    int id;

    gui* m_gui;

};