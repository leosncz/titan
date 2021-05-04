/* Author: leosncz
Handles window and ogl context creation
*/
#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../version/version.h"

class display
{
public:
    display(int x, int y, int aa, bool fullscreen = false, bool minorDisplay=false);
    void exitEngine();
    bool shouldExit()
    {
        if(glfwWindowShouldClose(m_window) || m_shouldExit == true)
        {
            return true;
        }
        return false;
    }
    void setRenderFlag()
    {
        glfwMakeContextCurrent(m_window);
    }
    void updateWindow()
    {
        // Update window content & user interaction
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
    int getDisHeight(){return m_disHeight;}
    int getDisWidth(){return m_disWidth;}
    GLFWwindow *getGLFWWindow(){return m_window;}
    void hideCursor(){ glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
    void showCursor() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
    void setExitStatus(bool shouldQuit) { m_shouldExit = shouldQuit; }
    ~display()
    {
        std::cout << "--> Exiting Engine";
        glfwTerminate();
    }
private:
    GLFWwindow* m_window;
    int m_disWidth;
    int m_disHeight;
    bool m_shouldExit;
};
#endif // DISPLAY_H_INCLUDED
