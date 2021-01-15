#include <learnopengl/shader.hpp>
#include <learnopengl/phongmaterial.hpp>
#include <learnopengl/diffusespecularmaterial.hpp>
#include <learnopengl/pointlight.hpp>
#include <learnopengl/directionlight.hpp>
#include <learnopengl/spotlight.hpp>
#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

namespace learnopengl {

bool checkShaderCompilationError(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return false;
    }

    return true;
}

bool checkShaderProgramCompilationError(unsigned int shaderProgram)
{
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return false;
    }

    return true;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Create a vertex Shader
    // The vertex shader map each of our vertice to screen coordinate (between -1 to 1)
    const unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    // Compile the shader from its source. A compile error can occurs.
    glCompileShader(vertexShader);
    checkShaderCompilationError(vertexShader);

    // Create fragment shader
    const unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);
    checkShaderCompilationError(fragmentShader);

    // Create our shader program that use the 2 already compiled shaders
    const unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkShaderProgramCompilationError(shaderProgram);

    // Shaders objects are no longer required onced they have been linked
    // It's liked obj file (.o) for c/cpp
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _id = shaderProgram;
}

Shader::~Shader()
{
    if(_id)
    {
        glDeleteProgram(_id);
    }
}

void Shader::use() { glUseProgram(_id); }

void Shader::unUse() { glUseProgram(0); }

void Shader::setBool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value); }

void Shader::setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(_id, name.c_str()), value); }

void Shader::setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(_id, name.c_str()), value); }

void Shader::setMat4(const std::string& name, const float* values) const
{
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, values);
}

void Shader::setMat3(const std::string& name, const float* values) const
{
    glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, values);
}

void Shader::setVec3(const std::string& name, const float& x, const float& y, const float& z) const
{
    glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
}

void Shader::setPhongMaterial(const std::string& name, const PhongMaterial& material) const
{
    const std::string ambientName = name + ".ambient";
    const std::string diffuseName = name + ".diffuse";
    const std::string specularName = name + ".specular";
    const std::string shininessName = name + ".shininess";

    setVec3(ambientName, material.ambient().x, material.ambient().x, material.ambient().x);
    setVec3(diffuseName, material.diffuse().x, material.diffuse().y, material.diffuse().z);
    setVec3(specularName, material.specular().x, material.specular().y, material.specular().z);
    setFloat(shininessName, material.shininess());
}

void Shader::setDiffuseSpecularMaterial(const std::string& name, const DiffuseSpecularMaterial& material) const
{
    const std::string diffuseName = name + ".diffuse";
    const std::string specularName = name + ".specular";
    const std::string shininessName = name + ".shininess";

    setInt(diffuseName, material.diffuseTextureUnit());
    setInt(specularName, material.specularTextureUnit());
    setFloat(shininessName, material.shininess());
}

void Shader::setPointLight(const std::string& name, const PointLight& light) const
{
    const std::string ambientName = name + ".ambient";
    const std::string diffuseName = name + ".diffuse";
    const std::string specularName = name + ".specular";
    const std::string positionName = name + ".position";

    setVec3(ambientName, light.ambient().x, light.ambient().x, light.ambient().x);
    setVec3(diffuseName, light.diffuse().x, light.diffuse().y, light.diffuse().z);
    setVec3(specularName, light.specular().x, light.specular().y, light.specular().z);
    setVec3(positionName, light.position().x, light.position().y, light.position().z);

    const std::string constantName = name + ".constant";
    const std::string linearName = name + ".linear";
    const std::string quadraticName = name + ".quadratic";

    setFloat(constantName, light.attenuationConstant());
    setFloat(linearName, light.attenuationLinear());
    setFloat(quadraticName, light.attenuationQuadratic());
}

void Shader::setDirectionLight(const std::string& name, const DirectionLight& light) const
{
    const std::string ambientName = name + ".ambient";
    const std::string diffuseName = name + ".diffuse";
    const std::string specularName = name + ".specular";
    const std::string directionName = name + ".direction";

    setVec3(ambientName, light.ambient().x, light.ambient().x, light.ambient().x);
    setVec3(diffuseName, light.diffuse().x, light.diffuse().y, light.diffuse().z);
    setVec3(specularName, light.specular().x, light.specular().y, light.specular().z);
    setVec3(directionName, light.direction().x, light.direction().y, light.direction().z);
}

void Shader::setSpotLight(const std::string& name, const SpotLight& light) const
{
    const std::string ambientName = name + ".ambient";
    const std::string diffuseName = name + ".diffuse";
    const std::string specularName = name + ".specular";
    const std::string directionName = name + ".direction";
    const std::string positionName = name + ".position";
    const std::string cutOffName = name + ".cutOff";
    const std::string outerCutOffName = name + ".outerCutOff";
    const std::string constantName = name + ".constant";
    const std::string linearName = name + ".linear";
    const std::string quadraticName = name + ".quadratic";

    setVec3(ambientName, light.ambient().x, light.ambient().x, light.ambient().x);
    setVec3(diffuseName, light.diffuse().x, light.diffuse().y, light.diffuse().z);
    setVec3(specularName, light.specular().x, light.specular().y, light.specular().z);
    setVec3(directionName, light.direction().x, light.direction().y, light.direction().z);
    setVec3(positionName, light.position().x, light.position().y, light.position().z);
    setFloat(cutOffName, light.cutOff());
    setFloat(outerCutOffName, light.outerCutOff());

    setFloat(constantName, light.attenuationConstant());
    setFloat(linearName, light.attenuationLinear());
    setFloat(quadraticName, light.attenuationQuadratic());
}

}
