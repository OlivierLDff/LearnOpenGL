#ifndef __LEARNOPENGL_TEXTURE_HPP__
#define __LEARNOPENGL_TEXTURE_HPP__

#include <string>
#include <memory>

namespace learnopengl {

class SharedTexture;

class Texture
{
public:
    struct Settings
    {
        bool verticalFlip = false;
        enum class WrapMode
        {
            Repeat,
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder,
        };
        WrapMode wrapMode = WrapMode::Repeat;
        bool nearest = false;
        std::string name;
    };

    // constructor reads and allocate the texture
    Texture(const std::string& filePath, const Settings& settings = {});
    ~Texture();

    // use/activate the texture
    void use(std::uint32_t textureUnit = 0) const;

    [[nodiscard]] std::uint32_t id() const;
    [[nodiscard]] std::string name() const;

private:
    std::shared_ptr<SharedTexture> _impl;

    friend SharedTexture;
};

}

#endif
