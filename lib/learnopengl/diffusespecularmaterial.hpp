#ifndef __LEARNOPENGL_DIFFUSE_SPECULAR_MATERIAL_HPP__
#define __LEARNOPENGL_DIFFUSE_SPECULAR_MATERIAL_HPP__

namespace learnopengl {

class DiffuseSpecularMaterial
{
public:
    constexpr DiffuseSpecularMaterial() = default;

public:
    [[nodiscard]] int diffuseTextureUnit() const { return _diffuseTextureUnit; }
    void setDiffuseTextureUnit(int diffuseTextureUnit) { _diffuseTextureUnit = diffuseTextureUnit; }

    [[nodiscard]] int specularTextureUnit() const { return _specularTextureUnit; }
    void setSpecularTextureUnit(int specularTextureUnit) { _specularTextureUnit = specularTextureUnit; }

    [[nodiscard]] constexpr float shininess() const { return _shininess; }
    constexpr void setShininess(float shininess) { _shininess = shininess; }

private:
    int _diffuseTextureUnit = 0;
    int _specularTextureUnit = 0;
    float _shininess = 32.f;
};

}

#endif
