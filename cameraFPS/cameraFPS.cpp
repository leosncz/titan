#include "cameraFPS.h"
#include <iostream>
#include <gtc/matrix_transform.hpp>

cameraFPS::cameraFPS()
{
    std::cout << "--> Creating camera FPS id=" << id << std::endl;
    firstMouse = true;
    yaw = -90.0f;
}
void cameraFPS::update(bool isAzerty, glm::mat4* viewMatrix)
{
    if (!isPaused)
    {
        if (isAzerty)
        {
            if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS) { cameraPos += cameraSpeed * cameraFront; }
            else if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS) { cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; }
            else if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS) { cameraPos -= cameraSpeed * cameraFront; }
            else if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS) { cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; }
        }
        else
        {
            if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_Z) == GLFW_PRESS) { cameraPos += cameraSpeed * cameraFront; }
            else if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_Q) == GLFW_PRESS) { cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; }
            else if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS) { cameraPos -= cameraSpeed * cameraFront; }
            else if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS) { cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; }
        }
        *viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
}

void cameraFPS::init(display* display_)
{
    m_display = display_;
    glfwSetWindowUserPointer(display_->getGLFWWindow(), this);
    glfwSetCursorPosCallback(m_display->getGLFWWindow(), mouse_callback);
}

void cameraFPS::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    cameraFPS* getThis = reinterpret_cast<cameraFPS*>(glfwGetWindowUserPointer(window));

    if (getThis->firstMouse)
    {
        getThis->lastX = xpos;
        getThis->lastY = ypos;
        getThis->firstMouse = false;
    }

    float xoffset = xpos - getThis->lastX;
    float yoffset = getThis->lastY - ypos;
    getThis->lastX = xpos;
    getThis->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    getThis->yaw += xoffset;
    getThis->pitch += yoffset;

    if (getThis->pitch > 89.0f)
        getThis->pitch = 89.0f;
    if (getThis->pitch < -89.0f)
        getThis->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(getThis->yaw)) * cos(glm::radians(getThis->pitch));
    direction.y = sin(glm::radians(getThis->pitch));
    direction.z = sin(glm::radians(getThis->yaw)) * cos(glm::radians(getThis->pitch));
    getThis->cameraFront = glm::normalize(direction);
}