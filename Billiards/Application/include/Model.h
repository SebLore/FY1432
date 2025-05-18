#pragma once

#include <string>

#include <glm/glm.hpp>

#include "Mesh.h"


class Model
{
public:
    Model(std::vector<Mesh>&&meshes, std::string directory);
    ~Model() = default;
    
    // move constructor and operator
    Model(Model && model) noexcept = default;
    Model &operator=(Model &&other) noexcept = default;    
    
    void Draw()const;

    void SetScale(float scale);
    void SetScale(float x, float y, float z);
    void SetRotation(float angle, float x, float y, float z);
    void SetPosition(float x, float y, float z);
    void SetPosition(glm::vec3 position);

    // no copying
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
private:
    void UpdateTransform();

    // geometry data
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    // transform
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4x4 m_transform;
};
