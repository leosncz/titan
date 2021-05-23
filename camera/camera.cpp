#include "camera.h"
camera::camera(string tag)
{
    m_tag = tag;
    cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraSpeed = 0.1f;
    isPaused = false;
}
glm::vec3 camera::getCameraPos()
{ 
    return cameraPos;
}
void camera::pauseControls()
{
    isPaused = true;
}
void camera::resumeControls()
{
    isPaused = false;
}
bool camera::getIsPaused()
{ 
    return isPaused; 
}