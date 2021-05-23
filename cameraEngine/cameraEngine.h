#pragma once
#include "../camera/camera.h"
#include <glm.hpp>

class cameraEngine : public camera
{
public:
    cameraEngine(string tag);
    void update(bool isAzerty = true, glm::mat4* viewMatrix = 0);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void init(display* display_);

private:
    glm::vec3 direction;

    bool firstMouse;
    float yaw, pitch, lastX, lastY;
};