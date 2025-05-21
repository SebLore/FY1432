#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

constexpr float c_YAW = -90.0f; // points towards negative Z
constexpr float c_PITCH = 0.0f; // pitch
constexpr float c_SPEED = 2.5f; // travel speed
constexpr float c_SENSITIVITY = 0.1f; // mouse sensitivity
constexpr float c_ZOOM = 45.0f; // FOV

struct CameraProperties
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = c_YAW;
	float pitch = c_PITCH;
	float speed = c_SPEED;
	float sensitivity = c_SENSITIVITY;
	float zoom = c_ZOOM;

	float aspect = 1.0f;
	float nearZ = 0.1f;
	float farZ = 100.0f;

	glm::mat4x4 viewMatrix = glm::mat4(1.0f);
	glm::mat4x4 projMatrix = glm::mat4(1.0f);
	glm::mat4x4 viewProjectionMatrix = glm::mat4(1.0f);

	void UpdateViewMatrix()
	{
		viewMatrix = glm::lookAt(position, position + forward, up);
	}
	void UpdateProjectionMatrix()
	{
		projMatrix = glm::perspective(glm::radians(zoom), aspect, nearZ, farZ);
	}
};
class Camera
{
public:
	Camera();
    Camera(float width, float height, float nearZ, float farZ);
    ~Camera() = default;
    
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
	float GetYaw()const;
	float GetPitch()const;
	float GetZoom()const;

	void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3 &position);
    void SetUp(const glm::vec3 &up);
    void SetRight(const glm::vec3 &right);
    void SetForward(const glm::vec3 &forward);
	void SetAspectRatio(float aspect);
	void SetZoomFov(float zoom);

    void Translate(glm::vec3 translation);
    void Rotate(glm::vec3 rotation);
    void Rotate(float yaw, float pitch);
    void Zoom(float zoom);

	void MoveForward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);

	void Update(float delta = 0.0f);

	// TODO: implement these functions
	// void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	// void ProcessMouseScroll(float yoffset);
private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateCameraVectors();

    // camera view space attributes
    glm::vec3 m_position = glm::vec3(0, 0, 0);
    glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(0, 0, 1);

	// camera world space attributes
    glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

	// euler angles, default to looking down the -Z axis
	float m_yaw = -90.0f;
    float m_pitch = 0.0f;

	// projection
    float m_zoom = 45.0f; // FOV Y
	float m_aspect = 1.0f;
	float m_nearZ = 0.1f;
	float m_farZ = 100.0f;

	// camera settings
    float m_speed = 2.5f;
	float m_sensitivity = 0.1f;

	glm::mat4x4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4x4 m_projMatrix = glm::mat4(1.0f);
};
