#ifndef __LEARNOPENGL_SHADER_HPP__
#define __LEARNOPENGL_SHADER_HPP__

#include <string>

namespace learnopengl {

class PhongMaterial;
class DiffuseSpecularMaterial;
class PointLight;
class DirectionLight;
class SpotLight;

class Shader
{
public:
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    // use/activate the shader
    void use();
    void unUse();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const float* values) const;
    void setMat3(const std::string& name, const float* values) const;
    void setVec3(const std::string& name, const float& x, const float& y, const float& z) const;
    void setPhongMaterial(const std::string& name, const PhongMaterial& material) const;
    void setDiffuseSpecularMaterial(const std::string& name, const DiffuseSpecularMaterial& material) const;
    void setPointLight(const std::string& name, const PointLight& pointLight) const;
    void setDirectionLight(const std::string& name, const DirectionLight& directionLight) const;
    void setSpotLight(const std::string& name, const SpotLight& spotLight) const;

private:
    // the program ID
    unsigned int _id = 0;
};
}

#endif
