#ifndef __LEARNOPENGL_SHADER_HPP
#define __LEARNOPENGL_SHADER_HPP

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

private:
    // the program ID
    unsigned int _id = 0;
};
}

#endif
