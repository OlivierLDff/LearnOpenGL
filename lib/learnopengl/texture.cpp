#include <learnopengl/texture.hpp>
#include <learnopengl/fileinfo.hpp>

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

namespace learnopengl {

Texture::Texture(const char* filePath, const Settings& settings)
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    const auto wrapMode = [&]()
    {
        switch(settings.wrapMode)
        {
        case Settings::WrapMode::Repeat: return GL_REPEAT;
        case Settings::WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case Settings::WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
        case Settings::WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
        default: return GL_REPEAT;
        }
    }();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    const auto filterMode = settings.nearest ? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
    // load and generate the texture
    const auto texturePath = FileInfo(filePath).absolutePath();
    stbi_set_flip_vertically_on_load(settings.verticalFlip);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << stbi_failure_reason() << std::endl;
    }

    stbi_image_free(data);
}

Texture::~Texture() { glDeleteTextures(1, &_id); }

void Texture::use(int textureUnit)
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, _id);
}

}
