#include <learnopengl/cameracontroller.hpp>
#include <learnopengl/camera.hpp>
#include <GLFW/glfw3.h>

namespace learnopengl {

CameraController::CameraController(Camera* camera) : _camera(camera) {}

void CameraController::processInput(GLFWwindow* window)
{
    const auto currentFrame = float(glfwGetTime());
    _deltaTime = currentFrame - _lastFrame;
    _lastFrame = currentFrame;

    if(!_camera)
        return;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        _camera->move(learnopengl::Camera::Movement::Forward, _deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        _camera->move(learnopengl::Camera::Movement::Backward, _deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        _camera->move(learnopengl::Camera::Movement::Left, _deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        _camera->move(learnopengl::Camera::Movement::Right, _deltaTime);
}

void CameraController::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        if(action == GLFW_PRESS)
        {
            if(_mousePressedMode == MouseMode::None)
            {
                _firstMouse = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                if(button == GLFW_MOUSE_BUTTON_RIGHT)
                {
                    _mousePressedMode = MouseMode::Orbit;
                }
                else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
                {
                    if(mods & GLFW_MOD_SHIFT)
                        _mousePressedMode = MouseMode::Pan;
                    else if(mods & GLFW_MOD_CONTROL)
                        _mousePressedMode = MouseMode::Zoom;
                    else
                        _mousePressedMode = MouseMode::OrbitCenter;
                }
            }
        }
        else if(action == GLFW_RELEASE)
        {
            if((_mousePressedMode == MouseMode::Orbit && button == GLFW_MOUSE_BUTTON_RIGHT) ||
                ((_mousePressedMode == MouseMode::Pan || _mousePressedMode == MouseMode::Zoom ||
                     _mousePressedMode == MouseMode::OrbitCenter) &&
                    button == GLFW_MOUSE_BUTTON_MIDDLE))
            {
                _mousePressedMode = MouseMode::None;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }
}

void CameraController::mouseMoveCallback(float xpos, float ypos)
{
    if(_mousePressedMode == MouseMode::None)
        return;

    if(_firstMouse)
    {
        _lastPressedX = float(xpos);
        lastPressedY = float(ypos);
        _firstMouse = false;
    }

    float xoffset = float(xpos) - _lastPressedX;
    float yoffset = lastPressedY - float(ypos);
    _lastPressedX = float(xpos);
    lastPressedY = float(ypos);

    if(!_camera)
        return;

    if(_mousePressedMode == MouseMode::Orbit)
        _camera->orbit(xoffset, yoffset);
    else if(_mousePressedMode == MouseMode::Pan)
        _camera->pan(xoffset, yoffset);
    else if(_mousePressedMode == MouseMode::Zoom)
        _camera->zoom(yoffset);
    else if(_mousePressedMode == MouseMode::OrbitCenter)
        _camera->orbitCenter(xoffset, yoffset);
}

void CameraController::scrollCallback(float offset)
{
    if(!_camera)
        return;
    _camera->zoomFov(offset);
}

}
