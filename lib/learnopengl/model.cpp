#include <learnopengl/model.hpp>
#include <learnopengl/fileinfo.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

namespace learnopengl {

Model::Model(const std::string& filePath, bool verticalFlipTextures) : _verticalFlipTextures(verticalFlipTextures)
{
    const auto absolutePath = FileInfo(filePath).absolutePath();
    loadModel(absolutePath);
}

void Model::draw(const Shader& shader)
{
    for(const auto& mesh: _meshes) { mesh->draw(shader); }
}

void Model::loadModel(const std::string& path)
{
    std::cout << "Load model " << path << std::endl;
    _directory = path.substr(0, path.find_last_of('/'));
    std::cout << "Folder is " << _directory << std::endl;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.emplace_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) { processNode(node->mChildren[i], scene); }
}

std::unique_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) const
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<std::uint32_t> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        const auto& vertex = mesh->mVertices[i];
        const auto& normal = mesh->mNormals[i];
        const auto& texCoord =
            mesh->mTextureCoords[0] ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0, 0);

        // process vertex positions, normals and texture coordinates
        vertices.emplace_back(Mesh::Vertex({glm::vec3(vertex.x, vertex.y, vertex.z), glm::vec3(normal.x, normal.y, normal.z), texCoord}));
    }

    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        auto& face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }

    // process material
  
    if(mesh->mMaterialIndex < scene->mNumMaterials)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    else
    {
        textures.push_back(Texture("resources/textures/black.png",
            {
                .name = "texture_diffuse",
            }));

        textures.push_back(Texture("resources/textures/black.png",
            {
                .name = "texture_specular",
            }));
    }

    return std::make_unique<Mesh>(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, int type, const std::string& typeName) const
{
    const auto textureCount = mat->GetTextureCount(aiTextureType(type));
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < textureCount; i++)
    {
        aiString str;
        mat->GetTexture(aiTextureType(type), i, &str);

        textures.emplace_back(Texture(std::string(_directory + "/" + std::string(str.C_Str())),
            {
                .verticalFlip = _verticalFlipTextures,
                .name = typeName,
            }));
    }

    // Make sure every spectrum has texture
    if(!textureCount)
    {
        textures.push_back(Texture("resources/textures/black.png",
            {
                .name = typeName,
            }));
    }
    return textures;
}

}
