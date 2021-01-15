#ifndef __LEARNOPENGL_DIRECTION_LIGHT_HPP__
#define __LEARNOPENGL_DIRECTION_LIGHT_HPP__

#include <glm/vec3.hpp>

namespace learnopengl {

class DirectionLight
{
public:
    constexpr DirectionLight() = default;

public:
    [[nodiscard]] constexpr const glm::vec3& ambient() const { return _ambient; }
    constexpr void setAmbient(const glm::vec3& ambient) { _ambient = ambient; }

    [[nodiscard]] constexpr const glm::vec3& diffuse() const { return _diffuse; }
    constexpr void setDiffuse(const glm::vec3& diffuse) { _diffuse = diffuse; }

    [[nodiscard]] constexpr const glm::vec3& specular() const { return _specular; }
    constexpr void setSpecular(const glm::vec3& specular) { _specular = specular; }

    [[nodiscard]] constexpr const glm::vec3& direction() const { return _direction; }
    constexpr void setDirection(const glm::vec3& direction) { _direction = direction; }

private:
    glm::vec3 _ambient = glm::vec3(1.f);
    glm::vec3 _diffuse = glm::vec3(1.f);
    glm::vec3 _specular = glm::vec3(1.0f);
    glm::vec3 _direction = glm::vec3(0.0f);
};

}

#endif
