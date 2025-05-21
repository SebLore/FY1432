#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Mesh.h"


struct Transform
{
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	glm::mat4x4 modelMatrix = { 1.0f };
    // recalculate the mat4
    void Update() {
		modelMatrix = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), scale);
    
    }

};

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

    const glm::mat4x4& GetModelMatrix()const;
	Transform& GetTransform() { return m_transform; }
	void Update(float delta = 0.0f) { m_transform.Update(); } // update the transform matrix per frame
    // no copying
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;
private:
    void UpdateTransform();

    // geometry data
    std::vector<Mesh> m_meshes;
    std::string m_directory;

    // transform
	Transform m_transform;
    //glm::vec3 m_position;
    //glm::vec3 m_rotation;
    //glm::vec3 m_scale;
    //glm::mat4x4 m_transform;
};
