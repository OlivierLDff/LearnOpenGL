#ifndef __LEARNOPENGL_MODEL_HPP__
#define __LEARNOPENGL_MODEL_HPP__

#include <learnopengl/mesh.hpp>

#include <memory>

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;

namespace learnopengl {

class Model
{
public:
    Model(const std::string& filePath, bool verticalFlipTextures = false);

public:
    void draw(const Shader& shader);

private:
    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene) const;
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, int type, const std::string& typeName) const;

    std::vector<std::unique_ptr<Mesh>> _meshes;
    std::string _directory;
    bool _verticalFlipTextures = false;
};

}

#endif
