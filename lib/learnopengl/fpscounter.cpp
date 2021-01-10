#include <learnopengl/fpscounter.hpp>
#include <GLFW/glfw3.h>

#include <sstream>

namespace learnopengl {

void showFPS(GLFWwindow* pWindow)
{
    static double lastTime = 0;
    static int nbFrames = 0;
    // Measure speed
    const double currentTime = glfwGetTime();
    const double delta = currentTime - lastTime;
    nbFrames++;
    if(delta >= 1.0)
    {
        const double fps = double(nbFrames) / delta;

        std::stringstream ss;
        ss << "LearnOpenGL"
           << " [" << fps << " FPS]";

        glfwSetWindowTitle(pWindow, ss.str().c_str());

        nbFrames = 0;
        lastTime = currentTime;
    }
}

}
