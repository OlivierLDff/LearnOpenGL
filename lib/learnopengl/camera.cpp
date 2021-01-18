#include <learnopengl/camera.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <algorithm>

namespace learnopengl {

Camera::Camera() = default;

void Camera::move(Movement movement, float delta)
{
    const float deltaSpeed = _speed * delta;

    glm::vec3 moveDelta(0);

    if(movement == Movement::Forward)
    {
        moveDelta = cameraFront() * deltaSpeed;
    }
    else if(movement == Movement::Backward)
    {
        moveDelta = -cameraFront() * deltaSpeed;
    }
    else if(movement == Movement::Left)
    {
        moveDelta = -cameraRight() * deltaSpeed;
        _cameraCenter += moveDelta;
    }
    else if(movement == Movement::Right)
    {
        moveDelta = cameraRight() * deltaSpeed;
        _cameraCenter += moveDelta;
    }

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

    auto forward = cameraFront();
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
    setCameraFront(glm::normalize(direction));

    _viewMatrixDirty = true;
}

void Camera::orbitCenter(float xOffset, float yOffset, float deltaAngleX, float deltaAngleY)
{
    // Get the homogenous position of the camera and pivot point
    glm::vec4 position = glm::vec4(_cameraPos, 1.f);
    glm::vec4 pivot = glm::vec4(_cameraCenter, 1.f);

    // step 1 : Calculate the amount of rotation given the mouse movement.
    float xAngle = (xOffset)*deltaAngleX;
    float yAngle = (yOffset)*deltaAngleY;

    // Limit pitch
    float cosAngle = glm::dot(-cameraFront(), _cameraUp);
    if(cosAngle > 0.9f && yAngle < 0 || cosAngle < -0.9f && yAngle > 0)
        yAngle = 0;

    // step 2: Rotate the camera around the pivot point on the first axis.
    glm::mat4x4 rotationMatrixX(1.0f);
    rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, _cameraUp);
    position = rotationMatrixX * (position - pivot) + pivot;
    // step 3: Rotate the camera around the pivot point on the second axis.
    glm::mat4x4 rotationMatrixY(1.0f);
    const auto cameraRight = glm::normalize(glm::cross(cameraFront(), _cameraUp));
    rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, cameraRight);
    position = (rotationMatrixY * (position - pivot)) + pivot;

    _cameraPos = position;

    _viewMatrixDirty = true;
}

void Camera::pan(float xOffset, float yOffset)
{
    const auto projectionView = _projectionMatrix * _viewMatrix;
    const auto projectionViewInv = glm::inverse(projectionView);

    // Project pivot point in frustum
    auto pivotFrustum = projectionView * glm::vec4(_cameraCenter, 1.0);
    pivotFrustum /= pivotFrustum.w;

    // Apply delta in frustum
    const auto newPivotFrustum = glm::vec4(pivotFrustum.x - xOffset * 2, pivotFrustum.y - yOffset * 2, pivotFrustum.z, 1);

    // Project new pivot back in model space.
    auto newPivotView = projectionViewInv * newPivotFrustum;
    newPivotView /= newPivotView.w;

    // Compute delta in model space.
    const auto deltaModel = glm::vec3(newPivotView) - _cameraCenter;

    _cameraPos += glm::vec3(deltaModel);
    _cameraCenter += glm::vec3(deltaModel);
    _viewMatrixDirty = true;
}

void Camera::zoom(float offset)
{
    const auto newCameraPos = _cameraPos + offset * cameraFront() * _zoomSensitivity;

    // Avoid zoom reversing the camerapos
    if(glm::dot(_cameraCenter - _cameraPos, _cameraCenter - newCameraPos) <= 0.01f)
        return;

    _cameraPos = newCameraPos;

    _viewMatrixDirty = true;
}

void Camera::zoomFov(float offset) { setFovDegrees(glm::degrees(_fov) - offset); }

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
        _viewMatrix = glm::lookAt(_cameraPos, _cameraCenter, _cameraUp);
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
