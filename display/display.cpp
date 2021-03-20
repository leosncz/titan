#include "display.h"
int display::init(int x, int y, int aa)
{
    std::cout << "--> Titan Engine " << "dev 0.1" << " is warming up !" << std::endl;
    if( !glfwInit() )
    {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, aa); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make apple happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We are courageous

    m_window = glfwCreateWindow( x, y, "Titan Engine debug", NULL, NULL);
    if( m_window == NULL ){
        std::cout << "--> CRITICAL ERROR : OPENGL 3.X IS PROBABLY NOT SUPPORTED BY YOUR GRAPHIC CARD ! (-2)" << std::endl;
        glfwTerminate();
        return -2;
    }
    glfwMakeContextCurrent(m_window); // setup GLEW
    glewExperimental=true; // Nécessaire dans le profil de base
    if (glewInit() != GLEW_OK) {
        std::cout << "--> CRITICAL ERROR : OPENGL 3.X IS PROBABLY NOT SUPPORTED BY YOUR GRAPHIC CARD ! (-3)" << std::endl;
        glfwTerminate();
        return -3;
    }
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we will be able to capture all our keys
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glEnable(GL_MULTISAMPLE);  // Ensure the AA is on
    glDepthFunc(GL_LESS);

    //Comment this section to disable face-culling optimization
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    m_disHeight = y;
    m_disWidth = x;

    std::cout << "---> Titan Engine successfuly created OpenGL Context" << std::endl;

    return 0;
}

void display::exitEngine()
{
    glfwTerminate();
}

