#include <learnopengl/window.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>

namespace learnopengl {

int initGLAD()
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 0;
}

int initGLFW()
{
    glfwInit();

    // Use min OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Use OpenGL in Core Profile (vs old opengl, deprecated function are not available here)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }

GLFWwindow* createWindowContext(const char* title, int width = 800, int height = 600)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSwapInterval(1);

    return window;
}
struct GLFWwindow* createWindow(int width, int height)
{
    if(initGLFW() < 0)
        return nullptr;

    // Create a valid openGl Context
    auto* window = createWindowContext("LearnOpenGL");
    if(!window)
        return nullptr;

    // GLAD required a valid OpenGL context to load opengl functions
    if(initGLAD() < 0)
        return nullptr;

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "GL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    return window;
}

}
