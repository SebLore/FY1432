#include "Model.h"




Model::Model(std::vector<Mesh>&& meshes, std::string directory)
	: m_meshes(std::move(meshes)), m_directory(std::move(directory)),
	m_transform(Transform())
{

}

void Model::Draw() const
{
	for (const auto& mesh : m_meshes)
		mesh.Draw();
}

void Model::SetPosition(float x, float y, float z)
{
	m_transform.position = glm::vec3(x, y, z);
}

void Model::SetPosition(glm::vec3 position)
{
	m_transform.position = position;
}

void Model::UpdateTransform()
{
	m_transform.Update();
}

void Model::SetScale(float scale)
{
	m_transform.scale = glm::vec3(scale);
}

void Model::SetScale(float x, float y, float z)
{
	m_transform.scale = glm::vec3(x, y, z);
}

const glm::mat4x4 &Model::GetModelMatrix()const 
{
	return m_transform.modelMatrix;
}





