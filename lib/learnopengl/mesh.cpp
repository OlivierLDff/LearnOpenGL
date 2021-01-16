#include <learnopengl/mesh.hpp>
#include <learnopengl/shader.hpp>

#include <glad/glad.h>

#include <cstddef>

namespace learnopengl {

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_EBO);
    glDeleteBuffers(1, &_VBO);
}

void Mesh::draw(const Shader& shader) const
{
    shader.use();

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for(int i = 0; i < int(_textures.size()); i++)
    {
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = _textures[i].name();
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        std::string completeName = "material." + name;
        completeName += number;

        shader.setInt(completeName, i);
        _textures[i].use(i);
    }
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, int(_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::setup()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(std::uint32_t) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

}
