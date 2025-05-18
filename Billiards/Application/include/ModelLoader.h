// ModelLoader.h
// uses assimp to load Model.cpp files into memory
#pragma once
#include <memory>
#include <string>

#include "Model.h"

// forward declarations using assimp.h
struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum class aiTextureType;

class ModelLoader
{
public:
    ModelLoader() = delete;
    ~ModelLoader() = delete;
    
    static std::unique_ptr<Model> LoadModel(const std::string &path);
private:
    static void ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& outMeshes,
                            const std::string& directory);
    static Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string &directory);
};
