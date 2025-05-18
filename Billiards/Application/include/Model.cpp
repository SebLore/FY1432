#include "Model.h"
#include <utility>

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

void Model::SetScale(float scale)
{
    m_scale = glm::vec3(scale);
}

void Model::SetScale(float x, float y, float z)
{
    m_scale = glm::vec3(x, y, z);
}




