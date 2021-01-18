#ifndef __LEARNOPENGL_GRID_FLOOR_HPP__
#define __LEARNOPENGL_GRID_FLOOR_HPP__

#include <cstdint>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

namespace learnopengl {

class Shader;
class Camera;

class GridFloor
{
public:
    GridFloor() { setup(); }
    ~GridFloor();

public:
    void draw(const Camera& camera) const;

    [[nodiscard]] glm::vec3 lineColor() const { return _lineColor; }
    void setLineColor(const glm::vec3& lineColor) { _lineColor = lineColor; }

    [[nodiscard]] glm::vec3 backgroundColor() const { return _backgroundColor; }
    void setBackgroundColor(const glm::vec3& backgroundColor) { _backgroundColor = backgroundColor; }

    [[nodiscard]] glm::vec3 xAxisColor() const { return _xAxisColor; }
    void setXAxisColor(const glm::vec3& xAxisColor) { _xAxisColor = xAxisColor; }

    [[nodiscard]] glm::vec3 yAxisColor() const { return _yAxisColor; }
    void setYAxisColor(const glm::vec3& yAxisColor) { _yAxisColor = yAxisColor; }

    [[nodiscard]] glm::vec3 zAxisColor() const { return _zAxisColor; }
    void setZAxisColor(const glm::vec3& zAxisColor) { _zAxisColor = zAxisColor; }

    [[nodiscard]] bool drawXAxis() const { return _drawXAxis; }
    void setDrawXAxis(bool drawXAxis) { _drawXAxis = drawXAxis; }

    [[nodiscard]] bool drawYAxis() const { return _drawYAxis; }
    void setDrawYAxis(bool drawYAxis) { _drawYAxis = drawYAxis; }

    [[nodiscard]] bool drawZAxis() const { return _drawZAxis; }
    void setDrawZAxis(bool drawZAxis) { _drawZAxis = drawZAxis; }

    [[nodiscard]] float lineWidth() const { return _lineWidth; }
    void setLineWidth(float lineWidth) { _lineWidth = lineWidth; }

    [[nodiscard]] float lineScale() const { return _lineScale; }
    void setLineScale(float lineScale) { _lineScale = lineScale; }

private:
    void setup();

    std::uint32_t _VAO = 0;
    std::uint32_t _VBO = 0;
    std::uint32_t _EBO = 0;

    std::unique_ptr<Shader> _shader;

    glm::vec3 _lineColor = glm::vec3(0.3f, 0.3f, 0.3f);
    glm::vec3 _backgroundColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 _xAxisColor = glm::vec3(244.f, 67.f, 54.f) / 255.f;
    glm::vec3 _yAxisColor = glm::vec3(139.f, 195.f, 74.f) / 255.f;
    glm::vec3 _zAxisColor = glm::vec3(63.f, 81.f, 181.f) / 255.f;

    bool _drawXAxis = true;
    bool _drawYAxis = false;
    bool _drawZAxis = true;

    float _lineWidth = 1.2f;
    float _lineScale = 1.f;
};

}

#endif
