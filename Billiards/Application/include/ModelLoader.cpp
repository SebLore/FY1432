#include "ModelLoader.h"
#include "Model.h"
#include "Mesh.h"

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <utility> // std::move

using namespace Assimp;

std::unique_ptr<Model> ModelLoader::LoadModel(const std::string& path)
{   
    Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcessPreset_TargetRealtime_Quality | aiProcess_ValidateDataStructure
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::MODELLOADER::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    std::vector<Mesh> meshes;

    ProcessNode(scene->mRootNode, scene, meshes, directory);

    std::cout << "Model loaded with ModeLoader: " << path << "with" << meshes.size() << " meshes.\n";

    return std::make_unique<Model>(std::move(meshes), directory);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& outMeshes,
                              const std::string& directory)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        outMeshes.push_back(ProcessMesh(mesh, scene, directory));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, outMeshes, directory);
    }
}

Mesh ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory)
{
    std::vector<Vertex> vertices;
    std::vector<GLsizei> indices;
    vertices.reserve(mesh->mNumVertices);
    indices.reserve(mesh->mNumFaces * 3);

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.texCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Mesh(vertices.data(), indices.data(), mesh->mNumVertices, mesh->mNumFaces * 3);
}
