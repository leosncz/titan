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
    int init(int x, int y, int aa); // Create window and context
    void exitEngine();
    void refreshWindow()
    {
        glfwSwapBuffers(m_window);
    }
    bool shouldExit()
    {
        if(glfwGetKey(m_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(m_window))
        {
            return true;
        }
        return false;
    }

    int getDisHeight(){return m_disHeight;}
    int getDisWidth(){return m_disWidth;}
    GLFWwindow *getGLFWWindow(){return m_window;}

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
