#include "Model.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


Model::Model(std::vector<Mesh>&& meshes, std::string directory)
	: m_meshes(std::move(meshes)), m_directory(std::move(directory)),
	m_position(glm::vec3(0.0f, 0.0f, 0.0f)), m_rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_transform(glm::mat4(1.0f))
{

}

void Model::Draw() const
{
	for (const auto& mesh : m_meshes)
		mesh.Draw();
}

void Model::SetPosition(float x, float y, float z)
{
	m_position = glm::vec3(x, y, z);
}

void Model::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void Model::UpdateTransform()
{
	// create a world matrix from the local transform and push it to the vertex shader
	m_transform = glm::translate(glm::mat4(1.0f), m_position) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(glm::mat4(1.0f), m_scale);
}

void Model::SetScale(float scale)
{
	m_scale = glm::vec3(scale);
}

void Model::SetScale(float x, float y, float z)
{
	m_scale = glm::vec3(x, y, z);
}

const glm::mat4x4 &Model::GetTransform()const 
{
	return m_transform;
}





