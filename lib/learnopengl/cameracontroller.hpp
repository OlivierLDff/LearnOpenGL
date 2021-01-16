#ifndef __LEARNOPENGL_CAMERA_CONTROLLER_HPP__
#define __LEARNOPENGL_CAMERA_CONTROLLER_HPP__

struct GLFWwindow;

namespace learnopengl {

class Camera;

class CameraController
{
public:
    enum class MouseMode
    {
        None,
        Orbit,
        Pan,
        OrbitCenter,
        Zoom
    };

public:
    CameraController(Camera* camera);

    void processInput(GLFWwindow* window);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void mouseMoveCallback(GLFWwindow* window, float xpos, float ypos);
    void scrollCallback(float offset);

private:
    float _deltaTime = 0.0f;
    float _lastFrame = 0.0f;

    float _lastPressedX = 0;
    float lastPressedY = 0;
    bool _firstMouse = true;
    MouseMode _mousePressedMode = MouseMode::None;

    Camera* _camera = nullptr;
};

}

#endif
