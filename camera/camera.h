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
    }

    glm::vec3 getCameraPos() { return cameraPos; }

protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;

    int id;
};