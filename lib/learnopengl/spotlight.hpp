#ifndef __LEARNOPENGL_SPOT_LIGHT_HPP__
#define __LEARNOPENGL_SPOT_LIGHT_HPP__

#include <glm/vec3.hpp>

namespace learnopengl {

class SpotLight
{
public:
    constexpr SpotLight() = default;

public:
    [[nodiscard]] constexpr const glm::vec3& ambient() const { return _ambient; }
    constexpr void setAmbient(const glm::vec3& ambient) { _ambient = ambient; }

    [[nodiscard]] constexpr const glm::vec3& diffuse() const { return _diffuse; }
    constexpr void setDiffuse(const glm::vec3& diffuse) { _diffuse = diffuse; }

    [[nodiscard]] constexpr const glm::vec3& specular() const { return _specular; }
    constexpr void setSpecular(const glm::vec3& specular) { _specular = specular; }

    [[nodiscard]] constexpr const glm::vec3& position() const { return _position; }
    constexpr void setPosition(const glm::vec3& position) { _position = position; }

    [[nodiscard]] constexpr const glm::vec3& direction() const { return _direction; }
    constexpr void setDirection(const glm::vec3& direction) { _direction = direction; }

    [[nodiscard]] constexpr float cutOff() const { return _cutOff; }
    constexpr void setCutOff(float cutOff) { _cutOff = cutOff; }

    [[nodiscard]] constexpr float outerCutOff() const { return _outerCutOff; }
    void constexpr setOuterCutOff(float outerCutOff) { _outerCutOff = outerCutOff; }

    [[nodiscard]] constexpr float attenuationConstant() const { return _attenuationConstant; }
    [[nodiscard]] constexpr float attenuationLinear() const { return _attenuationLinear; }
    [[nodiscard]] constexpr float attenuationQuadratic() const { return _attenuationQuadratic; }
    constexpr void setAttenuation(float constant, float linear, float quadratic)
    {
        _attenuationConstant = constant;
        _attenuationLinear = linear;
        _attenuationQuadratic = quadratic;
    }

private:
    glm::vec3 _ambient = glm::vec3(1.f);
    glm::vec3 _diffuse = glm::vec3(1.f);
    glm::vec3 _specular = glm::vec3(1.0f);

    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _direction = glm::vec3(0.0f);

    // the cutoff angle that specifies the spotlight's radius. Everything outside this angle is not lit by the spotlight.
    float _cutOff = 0.f;
    float _outerCutOff = 0.f;

    float _attenuationConstant = 1.f;
    float _attenuationLinear = 0.045f;
    float _attenuationQuadratic = 0.0075f;
};

}

#endif
