/* Author: leosncz
Handles window and ogl context creation
*/
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class display
{
public:
    display(int x, int y, int aa, bool fullscreen = false, bool minorDisplay=false);
    void exitEngine();
    bool shouldExit();
    void updateWindow();
    void setRenderFlag();
    int getDisHeight();
    int getDisWidth();
    GLFWwindow* getGLFWWindow();
    void hideCursor();
    void showCursor();
    void setExitStatus(bool shouldQuit);
    ~display();
private:
    GLFWwindow* m_window;
    int m_disWidth;
    int m_disHeight;
    bool m_shouldExit;
};