#ifndef __LEARNOPENGL_MATERIAL_HPP__
#define __LEARNOPENGL_MATERIAL_HPP__

#include <glm/vec3.hpp>

namespace learnopengl {

class PhongMaterial
{
public:
    constexpr PhongMaterial() = default;

public:
    [[nodiscard]] constexpr const glm::vec3& ambient() const { return _ambient; }
    constexpr void setAmbient(const glm::vec3& ambient) { _ambient = ambient; }

    [[nodiscard]] constexpr const glm::vec3& diffuse() const { return _diffuse; }
    constexpr void setDiffuse(const glm::vec3& diffuse) { _diffuse = diffuse; }

    [[nodiscard]] constexpr const glm::vec3& specular() const { return _specular; }
    constexpr void setSpecular(const glm::vec3& specular) { _specular = specular; }

    [[nodiscard]] constexpr float shininess() const { return _shininess; }
    constexpr void setShininess(float shininess) { _shininess = shininess; }

private:
    glm::vec3 _ambient = glm::vec3(0.2f);
    glm::vec3 _diffuse = glm::vec3(1.0f);
    glm::vec3 _specular = glm::vec3(0.5f);
    float _shininess = 32.f;
};

}

#endif
