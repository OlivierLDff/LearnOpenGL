#ifndef __LEARNOPENGL_MATERIAL_COLLECTION_HPP__
#define __LEARNOPENGL_MATERIAL_COLLECTION_HPP__

#include <learnopengl/phongmaterial.hpp>

namespace learnopengl {

class PhongMaterialCollection
{
public:
    static constexpr PhongMaterial emerald()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.0215f, 0.1745f, 0.0215f));
        material.setDiffuse(glm::vec3(0.07568f, 0.61424f, 0.07568f));
        material.setSpecular(glm::vec3(0.633f, 0.727811f, 0.633f));
        material.setShininess(0.6f * 128.f);
        return material;
    }

    static constexpr PhongMaterial pearl()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.25, 0.20725, 0.20725));
        material.setDiffuse(glm::vec3(1, 0.829, 0.829));
        material.setSpecular(glm::vec3(0.296648, 0.296648, 0.296648));
        material.setShininess(0.088f * 128.f);
        return material;
    }

    static constexpr PhongMaterial bronze()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.2125, 0.1275, 0.054));
        material.setDiffuse(glm::vec3(0.714, 0.4284, 0.18144));
        material.setSpecular(glm::vec3(0.393548, 0.271906, 0.166721));
        material.setShininess(0.2f * 128.f);
        return material;
    }

    static constexpr PhongMaterial gold()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.24725, 0.1995, 0.0745));
        material.setDiffuse(glm::vec3(0.75164, 0.60648, 0.22648));
        material.setSpecular(glm::vec3(0.628281, 0.555802, 0.366065));
        material.setShininess(0.4f * 128.f);
        return material;
    }

    static constexpr PhongMaterial cyanPlastic()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.0, 0.1, 0.06));
        material.setDiffuse(glm::vec3(0.0, 0.50980392, 0.50980392));
        material.setSpecular(glm::vec3(0.50196078, 0.50196078, 0.50196078));
        material.setShininess(0.25f * 128.f);
        return material;
    }

    static constexpr PhongMaterial redPlastic()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.0, 0.0, 0.0));
        material.setDiffuse(glm::vec3(0.5, 0.0, 0.0));
        material.setSpecular(glm::vec3(0.7, 0.6, 0.6));
        material.setShininess(0.25f * 128.f);
        return material;
    }

    static constexpr PhongMaterial greenRubber()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.0, 0.05, 0.0));
        material.setDiffuse(glm::vec3(0.4, 0.5, 0.4));
        material.setSpecular(glm::vec3(0.04, 0.7, 0.04));
        material.setShininess(0.078125f * 128.f);
        return material;
    }

    static constexpr PhongMaterial yellowRubber()
    {
        PhongMaterial material;
        material.setAmbient(glm::vec3(0.05, 0.05, 0.0));
        material.setDiffuse(glm::vec3(0.5, 0.5, 0.4));
        material.setSpecular(glm::vec3(0.7, 0.7, 0.04));
        material.setShininess(0.078125f * 128.f);
        return material;
    }
};

}

#endif
