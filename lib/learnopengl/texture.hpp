#ifndef __LEARNOPENGL_TEXTURE_HPP__
#define __LEARNOPENGL_TEXTURE_HPP__

namespace learnopengl {

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
    };

    // constructor reads and allocate the texture
    Texture(const char* filePath, const Settings& settings = {});
    ~Texture();

    // use/activate the texture
    void use(int textureUnit = 0);

    [[nodiscard]] unsigned id() const { return _id; }

private:
    // the program ID
    unsigned int _id = 0;
};
}

#endif
