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
        m_gui = 0;
    }

    glm::vec3 getCameraPos() { return cameraPos; }
    void updateGUI()
    {
        if (m_gui != 0)
        {
            std::vector<light*> null;
            m_gui->update(0, null, 0, 0, 0, 0, 0);
        }
    }
    void setGUI(gui* gui_)
    {
        m_gui = gui_;
        m_gui->init(m_display);
        m_gui->setVisibility(false);
    }
    virtual void update(glm::mat4* viewMatrix, bool isAzerty = true) = 0;
    virtual void init(display* display_) = 0;
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

    gui *m_gui;

};