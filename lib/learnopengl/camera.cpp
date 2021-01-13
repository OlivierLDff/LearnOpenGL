#include <learnopengl/camera.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/string_cast.hpp>

#include <algorithm>

namespace learnopengl {

Camera::Camera()
{
}

void Camera::move(Movement movement, float delta)
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

void Camera::orbit(float xOffset, float yOffset, bool constrainPitch)
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

void Camera::orbitCenter(float xOffset, float yOffset, bool constrainPitch)
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

void Camera::pan(float xOffset, float yOffset)
{
    const auto cameraRightWorld = glm::normalize(glm::cross(_cameraFront, _cameraUp));
    const auto cameraUpWorld = glm::normalize(glm::cross(_cameraFront, -cameraRightWorld));
    const auto xDelta = cameraRightWorld * xOffset * _panSensitivity;
    const auto yDelta = cameraUpWorld * yOffset * _panSensitivity;
    const auto moveDelta = xDelta + yDelta;
    _cameraPos += moveDelta;
    _viewMatrixDirty = true;
}

void Camera::zoom(float offset)
{
    _cameraPos += offset * _cameraFront * _zoomSensitivity;
    _viewMatrixDirty = true;
}

void Camera::zoomFov(float offset)
{
    setFovDegrees(glm::degrees(_fov) - offset);
}

void Camera::setFovDegrees(float fov)
{
    _fov = glm::radians(std::clamp(fov, 1.f, 90.f));
    _projectionMatrixDirty = true;
}

void Camera::setAspect(float aspect)
{
    if(!glm::epsilonEqual(_aspect, aspect, 0.01f))
    {
        _aspect = aspect;
        _projectionMatrixDirty = true;
    }
}

const glm::mat4& Camera::viewMatrix() const
{
    if(_viewMatrixDirty)
    {
        _viewMatrixDirty = false;
        _viewMatrix = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);
    }
    return _viewMatrix;
}

const glm::mat4& Camera::projectionMatrix() const
{
    if(_projectionMatrixDirty)
    {
        _projectionMatrixDirty = false;
        _projectionMatrix = glm::perspective(_fov, _aspect, _near, _far);
    }
    return _projectionMatrix;
}
}
