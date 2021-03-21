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
    int init(int x, int y, int aa, bool fullscreen=false); // Create window and context
    void exitEngine();
    bool shouldExit()
    {
        if(glfwWindowShouldClose(m_window))
        {
            return true;
        }
        return false;
    }

    int getDisHeight(){return m_disHeight;}
    int getDisWidth(){return m_disWidth;}
    GLFWwindow *getGLFWWindow(){return m_window;}
    void hideCursor(){ glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
    void showCursor() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

    ~display()
    {
        std::cout << "--> Exiting Engine";
        glfwTerminate();
    }
private:
    GLFWwindow* m_window;
    int m_disWidth;
    int m_disHeight;
};
#endif // DISPLAY_H_INCLUDED
