#include <learnopengl/gridfloor.hpp>
#include <learnopengl/shader.hpp>
#include <learnopengl/camera.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace learnopengl {

GridFloor::~GridFloor()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_EBO);
    glDeleteBuffers(1, &_VBO);
}

void GridFloor::draw(const Camera& camera) const
{
    _shader->use();

    const auto& view = camera.viewMatrix();
    const auto& projection = camera.projectionMatrix();

    const auto viewProjection = projection * view;
    const auto inverseViewProjection = glm::inverse(viewProjection);

    _shader->setMat4("viewMatrix", glm::value_ptr(view));
    _shader->setMat4("viewProjectionMatrix", glm::value_ptr(viewProjection));
    _shader->setMat4("inverseViewProjectionMatrix", glm::value_ptr(inverseViewProjection));
    _shader->setFloat("nearPlane", camera.near());
    _shader->setFloat("farPlane", camera.far());

    _shader->setVec4("xAxisColor", _xAxisColor.r, _xAxisColor.g, _xAxisColor.b, _drawXAxis ? 1.f : 0.f);
    _shader->setVec4("yAxisColor", _yAxisColor.r, _yAxisColor.g, _yAxisColor.b, _drawYAxis ? 1.f : 0.f);
    _shader->setVec4("zAxisColor", _zAxisColor.r, _zAxisColor.g, _zAxisColor.b, _drawZAxis ? 1.f : 0.f);
    _shader->setVec4("lineColor", _lineColor.r, _lineColor.g, _lineColor.b, 1.f);
    _shader->setVec4("backgroundColor", _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, 1.f);

    _shader->setFloat("lineWidth", _lineWidth);
    _shader->setFloat("lineScale", _lineScale);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void GridFloor::setup()
{
    // Vertices of triangle shape in 3d. (x, y, z)
    // clang-format off
    float vertices[] = {
         1.f,  1.f, // top right
         1.f, -1.f, // bottom right
        -1.f, -1.f, // bottom left
        -1.f,  1.f, // top left
    };

    std::uint32_t indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
    // clang-format on

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    _shader = std::make_unique<Shader>("resources/shaders/gridfloor.vs", "resources/shaders/gridfloor.fs");
}

}
