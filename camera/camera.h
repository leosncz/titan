#pragma once
#pragma once
#include <iostream>
#include <glm.hpp>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include "../display/display.h"

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

    virtual void update(glm::mat4* viewMatrix, bool isAzerty = true) = 0;
    virtual void init(display* display_) = 0;
    void pauseControls()
    {
        isPaused = true;
    }
    void resumeControls()
    {
        isPaused = false;
    }

    bool getIsPaused() { return isPaused; }

protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
    bool isPaused;

    display* m_display;

    int id;

};