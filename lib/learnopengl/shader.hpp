#ifndef __LEARNOPENGL_SHADER_HPP__
#define __LEARNOPENGL_SHADER_HPP__

#include <string>

namespace learnopengl {

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

private:
    // the program ID
    unsigned int _id = 0;
};
}

#endif
