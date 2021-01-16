#include <learnopengl/texture.hpp>
#include <learnopengl/fileinfo.hpp>

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <vector>

namespace learnopengl {

class SharedTexture
{
public:
    SharedTexture(const std::string& filePath, const Texture::Settings& settings = {}) : _path(filePath), _name(settings.name)
    {
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        const auto wrapMode = [&]()
        {
            switch(settings.wrapMode)
            {
            case Texture::Settings::WrapMode::Repeat: return GL_REPEAT;
            case Texture::Settings::WrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            case Texture::Settings::WrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
            case Texture::Settings::WrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
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
            std::cout << "Load Texture " << _path << std::endl;
            glTexImage2D(GL_TEXTURE_2D,
                0,
                nrChannels == 3 ? GL_RGB : GL_RGBA,
                width,
                height,
                0,
                nrChannels == 3 ? GL_RGB : GL_RGBA,
                GL_UNSIGNED_BYTE,
                data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cerr << "Failed to load texture " << filePath << " : " << stbi_failure_reason() << std::endl;
        }

        stbi_image_free(data);
    }
    ~SharedTexture()
    {
        std::cout << "Delete Texture " << _path << std::endl;
        glDeleteTextures(1, &_id);
    }

    void use(std::uint32_t textureUnit) const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, _id);
    }
    std::uint32_t id() const { return _id; }

    // the program ID
    unsigned int _id = 0;
    std::string _path;
    std::string _name;
};

class TexturePool
{
public:
    static std::shared_ptr<SharedTexture> get(const std::string& filePath, const Texture::Settings& settings)
    {
        instance().cleanCache();
        auto texture = instance().find(filePath, settings.name);
        if(texture)
            return texture;

        texture = std::make_shared<SharedTexture>(filePath, settings);

        instance()._cache.push_back(texture);

        return texture;
    }

    static TexturePool& instance()
    {
        static TexturePool pool;
        return pool;
    }

private:
    void cleanCache()
    {
        const auto expired = [](const std::weak_ptr<SharedTexture>& texture) { return texture.expired(); };

        _cache.erase(std::remove_if(_cache.begin(), _cache.end(), expired), _cache.end());
    }

    std::shared_ptr<SharedTexture> find(const std::string& filePath, const std::string& name)
    {
        for(const auto& texture: _cache)
        {
            auto sharedTexture = texture.lock();
            if(!sharedTexture)
                continue;

            if(sharedTexture->_name == name && sharedTexture->_path == filePath)
                return sharedTexture;
        }

        return {};
    }

    std::vector<std::weak_ptr<SharedTexture>> _cache;
};

Texture::Texture(const std::string& filePath, const Settings& settings) : _impl(TexturePool::get(filePath, settings)) {}

Texture::~Texture() = default;

void Texture::use(std::uint32_t textureUnit) const { _impl->use(textureUnit); }

std::uint32_t Texture::id() const { return _impl->id(); }

std::string Texture::name() const { return _impl->_name; }
}
