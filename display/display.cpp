#include "display.h"
#include <string>
#include <iostream>
#include "../version/version.h"
display::display(int x, int y, int aa, bool fullscreen, bool minorDisplay)
{
    std::cout << "--> Titan Engine "  << version::getTitanVersion() << " is warming up !" << std::endl;
    if( !glfwInit() )
    {
        std::cout << "---> CRITICAL ERROR -1 (display)" << std::endl;
    }

    glfwWindowHint(GLFW_SAMPLES, aa); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make apple happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We are courageous
    glfwWindowHint(GLFW_RESIZABLE, false);

    if (fullscreen)
    {
        m_window = glfwCreateWindow(x, y, "Titan Engine debug - Press ESCAPE to show menu",glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        m_window = glfwCreateWindow(x, y, "Titan Engine debug - Press ESCAPE to show menu", NULL, NULL);
    }

    if( m_window == NULL ){
        std::cout << "--> CRITICAL ERROR : OPENGL 3.X IS PROBABLY NOT SUPPORTED BY YOUR GRAPHIC CARD ! (-2)" << std::endl;
        glfwTerminate();
    }
    if (!minorDisplay)
    {
        glfwMakeContextCurrent(m_window); // setup GLEW
        glewExperimental = true; // Nécessaire dans le profil de base
        if (glewInit() != GLEW_OK) {
            std::cout << "--> CRITICAL ERROR : OPENGL 3.X IS PROBABLY NOT SUPPORTED BY YOUR GRAPHIC CARD ! (-3)" << std::endl;
            glfwTerminate();
        }
    }
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we will be able to capture all our keys
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glEnable(GL_MULTISAMPLE);  // Ensure the AA is on
    glDepthFunc(GL_LESS);

    //Comment this section to disable face-culling optimization
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    m_disHeight = y;
    m_disWidth = x;
    m_shouldExit = false;
    std::cout << "---> Titan Engine successfuly created OpenGL Context" << std::endl;
}

void display::exitEngine()
{
    glfwTerminate();
}

bool display::shouldExit()
{
    if (glfwWindowShouldClose(m_window) || m_shouldExit == true)
    {
        return true;
    }
    return false;
}

void display::updateWindow()
{
    // Update window content & user interaction
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void display::setRenderFlag()
{
    glfwMakeContextCurrent(m_window);
}

int display::getDisHeight() { return m_disHeight; }

int display::getDisWidth() { return m_disWidth; }

GLFWwindow* display::getGLFWWindow() { return m_window; }

void display::hideCursor() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

void display::showCursor() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

void display::setExitStatus(bool shouldQuit) { m_shouldExit = shouldQuit; }

display::~display()
{
    std::cout << "--> Exiting Engine";
    glfwTerminate();
}


