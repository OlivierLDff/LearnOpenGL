#ifndef __LEARNOPENGL_CAMERA_HPP__
#define __LEARNOPENGL_CAMERA_HPP__

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/string_cast.hpp>

#include <algorithm>
#include <iostream>

namespace learnopengl {

// Mix of Camera & CameraController
class Camera
{
public:
    Camera() {}

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

    void move(Movement movement, float delta)
    {
        const float deltaSpeed = _speed * delta;

        glm::vec3 moveDelta(0);

        if(movement == Movement::Forward)
            moveDelta = _cameraFront * deltaSpeed;
        else if(movement == Movement::Backward)
            moveDelta = -_cameraFront * deltaSpeed;
        else if(movement == Movement::Left)
            moveDelta = -glm::normalize(glm::cross(_cameraFront, _cameraUp)) * deltaSpeed;
        else if(movement == Movement::Right)
            moveDelta = glm::normalize(glm::cross(_cameraFront, _cameraUp)) * deltaSpeed;

        _cameraPos += moveDelta;

        _viewMatrixDirty = true;
    }

    void orbit(float xOffset, float yOffset, bool constrainPitch = true)
    {
        xOffset *= _orbitSensitivity;
        yOffset *= _orbitSensitivity;

        const glm::vec3 xUnit(1, 0, 0);
        const glm::vec3 yUnit(0, 1, 0);
        const glm::vec3 zUnit(0, 0, 1);

        auto forward = _cameraFront;
        auto pitch = glm::degrees(std::asinf(glm::dot(forward, yUnit)));
        forward.y = 0;
        forward = glm::normalize(forward);
        auto yaw = -glm::degrees(std::acosf(glm::dot(forward, xUnit)));
        if(glm::dot(forward, zUnit) > 0)
            yaw = 360 - yaw;

        yaw += xOffset;
        pitch += yOffset;

        pitch = std::clamp(pitch + yOffset, -89.f, 89.f);

        glm::vec3 direction;
        direction.x = std::cosf(glm::radians(yaw)) * std::cosf(glm::radians(pitch));
        direction.y = std::sinf(glm::radians(pitch));
        direction.z = std::sinf(glm::radians(yaw)) * std::cosf(glm::radians(pitch));
        _cameraFront = glm::normalize(direction);

        _viewMatrixDirty = true;
    }

    void orbitCenter(float xOffset, float yOffset, bool constrainPitch = true)
    {
        xOffset *= _orbitSensitivity;
        yOffset *= _orbitSensitivity;

        const glm::vec3 xUnit(1, 0, 0);
        const glm::vec3 yUnit(0, 1, 0);
        const glm::vec3 zUnit(0, 0, 1);

        auto forward = _cameraFront;
        auto pitch = glm::degrees(std::asinf(glm::dot(forward, yUnit)));
        forward.y = 0;
        forward = glm::normalize(forward);
        auto yaw = -glm::degrees(std::acosf(glm::dot(forward, xUnit)));
        if(glm::dot(forward, zUnit) > 0)
            yaw = 360 - yaw;

        yaw += xOffset;
        pitch += yOffset;

        pitch = std::clamp(pitch + yOffset, -89.f, 89.f);

        glm::vec3 direction;
        direction.x = std::cosf(glm::radians(yaw)) * std::cosf(glm::radians(pitch));
        direction.y = std::sinf(glm::radians(pitch));
        direction.z = std::sinf(glm::radians(yaw)) * std::cosf(glm::radians(pitch));
        _cameraFront = glm::normalize(direction);

        _viewMatrixDirty = true;
    }

    void pan(float xOffset, float yOffset)
    {
        const auto cameraRightWorld = glm::normalize(glm::cross(_cameraFront, _cameraUp));
        const auto cameraUpWorld = glm::normalize(glm::cross(_cameraFront, -cameraRightWorld));
        const auto xDelta = cameraRightWorld * xOffset * _panSensitivity;
        const auto yDelta = cameraUpWorld * yOffset * _panSensitivity;
        const auto moveDelta = xDelta + yDelta;
        _cameraPos += moveDelta;
        _viewMatrixDirty = true;
    }

    void zoom(float offset)
    {
        _cameraPos += offset * _cameraFront * _zoomSensitivity;
        _viewMatrixDirty = true;
    }

    void zoomFov(float offset) { setFovDegrees(glm::degrees(_fov) - offset); }

    // Camera API
public:
    void setFovDegrees(float fov)
    {
        _fov = glm::radians(std::clamp(fov, 1.f, 90.f));
        _projectionMatrixDirty = true;
    }

    void setAspect(float aspect)
    {
        if(!glm::epsilonEqual(_aspect, aspect, 0.01f))
        {
            _aspect = aspect;
            _projectionMatrixDirty = true;
        }
    }

    const glm::mat4& viewMatrix() const
    {
        if(_viewMatrixDirty)
        {
            _viewMatrixDirty = false;
            _viewMatrix = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
        }
        return _viewMatrix;
    }

    const glm::mat4& projectionMatrix() const
    {
        if(_projectionMatrixDirty)
        {
            _projectionMatrixDirty = false;
            _projectionMatrix = glm::perspective(_fov, _aspect, _near, _far);
        }
        return _projectionMatrix;
    }

private:
    glm::vec3 _cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 _cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float _speed = 2.5f;

    float _orbitSensitivity = 0.1f;
    float _panSensitivity = 0.01f;
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
