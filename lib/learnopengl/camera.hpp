#ifndef __LEARNOPENGL_CAMERA_HPP__
#define __LEARNOPENGL_CAMERA_HPP__

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace learnopengl {

// Mix of Camera & CameraController
class Camera
{
public:
    Camera();

    // CameraController Api
public:
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum class Movement
    {
        Forward,
        Backward,
        Left,
        Right
    };

    void move(Movement movement, float delta);
    void orbit(float xOffset, float yOffset, bool constrainPitch = true);
    void orbitCenter(float xOffset, float yOffset, float deltaAngleX = 0.1f, float deltaAngleY = 0.1f);
    void pan(float xOffset, float yOffset);
    void zoom(float offset);
    void zoomFov(float offset);

    // Camera API
public:
    void setFovDegrees(float fov);
    void setAspect(float aspect);

    const glm::mat4& viewMatrix() const;
    const glm::mat4& projectionMatrix() const;

    const glm::vec3& cameraPos() const { return _cameraPos; }
    void setCameraPos(const glm::vec3& cameraPos) { _cameraPos = cameraPos; }

    glm::vec3 cameraFront() const { return glm::normalize(_cameraCenter - _cameraPos); }
    void setCameraFront(const glm::vec3& cameraFront)
    {
        const auto distanceToCenter = glm::length(_cameraCenter - _cameraPos);
        _cameraCenter = _cameraPos + cameraFront * distanceToCenter;
    }

    glm::vec3 cameraRight() const { return glm::normalize(glm::cross(cameraFront(), _cameraUp)); }
    glm::vec3 cameraCenter() const { return _cameraCenter; }

    [[nodiscard]] float near() const { return _near; }

    void setNear(float near)
    {
        _near = near;
        _projectionMatrixDirty = true;
    }

    [[nodiscard]] float far() const { return _far; }

    void setFar(float far)
    {
        _far = far;
        _projectionMatrixDirty = true;
    }

private:
    glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 _cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float _speed = 2.5f;

    float _orbitSensitivity = 0.1f;
    float _zoomSensitivity = 0.01f;

    float _fov = glm::radians(45.f);
    float _aspect = 1.f;
    float _near = 0.1f;
    float _far = 100.f;

    mutable bool _viewMatrixDirty = true;
    mutable bool _projectionMatrixDirty = true;

    mutable glm::mat4 _viewMatrix = glm::mat4(1);
    mutable glm::mat4 _projectionMatrix = glm::mat4(1);
};

}

#endif
