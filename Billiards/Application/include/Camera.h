#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

constexpr float c_YAW = -90.0f; // points towards negative Z
constexpr float c_PITCH = 0.0f; // pitch
constexpr float c_SPEED = 2.5f; // travel speed
constexpr float c_SENSITIVITY = 0.1f; // mouse sense
constexpr float c_ZOOM = 45.0f; // FOV

class Camera
{
public:
    Camera();
    ~Camera();

    Camera(const Camera& camera);
    Camera& operator=(const Camera& camera);
    Camera(Camera && camera)noexcept;
    Camera& operator=(Camera && camera)noexcept;

    glm::mat4x4 GetViewMatrix()const;
    glm::mat4x4 GetProjectionMatrix()const;
    glm::mat4x4 GetViewProjectionMatrix()const;
    glm::vec3 GetPosition()const;
    glm::vec3 GetUp()const;
    glm::vec3 GetRight()const;
    glm::vec3 GetForward()const;

    void SetPosition(glm::vec3 position);
    void SetUp(glm::vec3 up);
    void SetRight(glm::vec3 right);
    void SetForward(glm::vec3 forward);

    void Translate(glm::vec3 translation);
    void Rotate(glm::vec3 rotation);
    void Rotate(float yaw, float pitch);
    void Zoom(float zoom);

private:
    glm::vec3 m_position = glm::vec3(0, 0, 0);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(0, 0, 1);
    glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float m_yaw = -90.0f;
    float m_pitch = 0.0f;
};
