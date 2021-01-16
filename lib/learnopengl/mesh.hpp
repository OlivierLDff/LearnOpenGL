#ifndef __LEARNOPENGL_MESH_HPP__
#define __LEARNOPENGL_MESH_HPP__

#include <learnopengl/texture.hpp>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <string>
#include <utility>
#include <vector>

namespace learnopengl {

class Shader;

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    Mesh(std::vector<Vertex> vertices, std::vector<std::uint32_t> indices, std::vector<Texture> textures) :
        _vertices(std::move(vertices)), _indices(std::move(indices)), _textures(std::move(textures))
    {
        setup();
    }
    ~Mesh();

public:
    void draw(const Shader& shader) const;

private:
    void setup();

    std::vector<Vertex> _vertices;
    std::vector<std::uint32_t> _indices;
    std::vector<Texture> _textures;

    std::uint32_t _VAO = 0;
    std::uint32_t _VBO = 0;
    std::uint32_t _EBO = 0;
};

}

#endif
