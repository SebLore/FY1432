#include "ModelLoader.h"
#include "Model.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// stb_image requires a single implementation definition in one source file
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

std::unique_ptr<Model> ModelLoader::LoadModel(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality |
		aiProcess_LimitBoneWeights |
		aiProcess_SplitLargeMeshes |
		aiProcess_PreTransformVertices |
		aiProcess_ValidateDataStructure);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	std::string directory = path.substr(0, path.find_last_of("/\\"));
	std::vector<Mesh> meshes;
	meshes.reserve(scene->mNumMeshes);

	ProcessNode(scene->mRootNode, scene, meshes, directory);
	return std::make_unique<Model>(std::move(meshes), directory);
}


void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes, const std::string& directory)
{
	// Process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene, directory));
	}
	// Then process each of its children
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene, meshes, directory);
	}
}

Mesh ModelLoader::ProcessMesh(aiMesh* aMesh, const aiScene* scene, const std::string& directory)
{
	// Temporary storage
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// 1) Walk through each of the mesh vertices
	vertices.reserve(aMesh->mNumVertices);
	for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// position
		vertex.position = {
			aMesh->mVertices[i].x,
			aMesh->mVertices[i].y,
			aMesh->mVertices[i].z };
		// normals
		if (aMesh->HasNormals())
		{
			vertex.normal = {
				aMesh->mNormals[i].x,
				aMesh->mNormals[i].y,
				aMesh->mNormals[i].z };
		}
		// texture coordinates
		if (aMesh->mTextureCoords[0])
		{
			vertex.texCoord = {
				aMesh->mTextureCoords[0][i].x,
				aMesh->mTextureCoords[0][i].y };
		}
		else
		{
			vertex.texCoord = { 0.0f, 0.0f };
		}
		vertices.push_back(vertex);
	}

	// 2) Walk through each of the mesh faces (triangles) and retrieve indices
	indices.reserve(aMesh->mNumFaces * 3);
	for (unsigned int i = 0; i < aMesh->mNumFaces; ++i)
	{
		aiFace& face = aMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	// 3) Process material
	if (aMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[aMesh->mMaterialIndex];
		// lambdas to load different texture types
		auto loadTex = [&](aiTextureType type, const std::string& typeName)
			{
				for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
				{
					aiString str;
					material->GetTexture(type, i, &str);
					Texture tex;
					tex.id = TextureFromFile(str.C_Str(), directory);
					tex.type = typeName;
					tex.path = str.C_Str();
					textures.push_back(tex);
				}
			};
		loadTex(aiTextureType_DIFFUSE, "texture_diffuse");
		loadTex(aiTextureType_SPECULAR, "texture_specular");
		loadTex(aiTextureType_HEIGHT, "texture_normal");
		loadTex(aiTextureType_AMBIENT, "texture_height");
	}

	// 4) Create the mesh using the data and return
	return Mesh(
		vertices.data(),
		reinterpret_cast<const GLsizei*>(indices.data()),
		static_cast<GLsizei>(vertices.size()),
		static_cast<GLsizei>(indices.size()));
}

unsigned int ModelLoader::TextureFromFile(const char* filename, const std::string& directory)
{
	std::string filepath = directory + "/" + filename;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = (nrComponents == 1 ? GL_RED : (nrComponents == 3 ? GL_RGB : GL_RGBA));
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Texture failed to load at path: " << filepath << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}