#include "Camera.h"

#include <iostream>

Camera::Camera()
{
    // initialize Camera with default values: up y+, right x+, forward z-
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_right = glm::vec3(1.0f, 0.0f, 0.0f);

    // default values
    m_yaw = c_YAW;
    m_pitch = c_PITCH;
    m_zoom = c_ZOOM;
    m_speed = c_SPEED;

    m_aspect = 1.0f;
    m_nearZ = 0.1f;
    m_farZ = 100.0f;

    // generate matrices
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
    m_projMatrix = glm::perspective(glm::radians(m_zoom), m_aspect, m_nearZ, m_farZ);
}

Camera::Camera(float width, float height, float nearZ, float farZ)
{
    // initialize Camera with default values: up y+, right x+, forward z-
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    // initialize camera angles to default values
    m_yaw = c_YAW;
    m_pitch = c_PITCH;
    m_zoom = c_ZOOM;
    m_speed = c_SPEED;
    m_aspect = width / height;
    m_nearZ = nearZ;
    m_farZ = farZ;

    // create the view matrix
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
    m_projMatrix = glm::perspective(glm::radians(m_zoom), m_aspect, m_nearZ, m_farZ);
}

Camera::Camera(const Camera& camera)
{
    m_position = camera.m_position;
    m_forward = camera.m_forward;
    m_up = camera.m_up;
    m_right = camera.m_right;
    m_world_up = camera.m_world_up;
    m_yaw = camera.m_yaw;
    m_pitch = camera.m_pitch;
    m_zoom = camera.m_zoom;
    m_speed = camera.m_speed;
    m_aspect = camera.m_aspect;
    m_nearZ = camera.m_nearZ;
    m_farZ = camera.m_farZ;

    m_viewMatrix = camera.m_viewMatrix;
    m_projMatrix = camera.m_projMatrix;
}

Camera& Camera::operator=(const Camera& camera)
{
    if (this != &camera)
    {
        m_position = camera.m_position;
        m_forward = camera.m_forward;
        m_up = camera.m_up;
        m_right = camera.m_right;
        m_world_up = camera.m_world_up;
        m_yaw = camera.m_yaw;
        m_pitch = camera.m_pitch;
        m_zoom = camera.m_zoom;
        m_speed = camera.m_speed;
        m_aspect = camera.m_aspect;
        m_nearZ = camera.m_nearZ;
        m_farZ = camera.m_farZ;
        m_viewMatrix = camera.m_viewMatrix;
        m_projMatrix = camera.m_projMatrix;
    }
    return *this;
}

// move operator
Camera::Camera(Camera&& camera) noexcept
    : m_position(camera.m_position), m_forward(camera.m_forward), m_up(camera.m_up), m_right(camera.m_right), 
      m_world_up(camera.m_world_up), m_yaw(camera.m_yaw), m_pitch(camera.m_pitch), m_speed(camera.m_speed),
      m_zoom(camera.m_zoom), m_aspect(camera.m_aspect), m_nearZ(camera.m_nearZ), m_farZ(camera.m_farZ),
      m_viewMatrix(camera.m_viewMatrix), m_projMatrix(camera.m_projMatrix)
{
    camera.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
    camera.m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    camera.m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.m_yaw = c_YAW;
    camera.m_pitch = c_PITCH;
    camera.m_zoom = c_ZOOM;
    camera.m_speed = c_SPEED;
    camera.m_aspect = 1.0f;
    camera.m_nearZ = 0.1f;
    camera.m_farZ = 100.0f;
    camera.m_viewMatrix = glm::mat4(1.0f);
    camera.m_projMatrix = glm::mat4(1.0f);
}

Camera& Camera::operator=(Camera&& camera) noexcept
{
    if (this != &camera)
    {
        m_position = camera.m_position;
        m_forward = camera.m_forward;
        m_up = camera.m_up;
        m_right = camera.m_right;
        m_world_up = camera.m_world_up;
        m_yaw = camera.m_yaw;
        m_pitch = camera.m_pitch;
        m_aspect = camera.m_aspect;
        m_nearZ = camera.m_nearZ;
        m_farZ = camera.m_farZ;
        m_viewMatrix = camera.m_viewMatrix;
        m_projMatrix = camera.m_projMatrix;

        // null initialize the other camera
        camera.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        camera.m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
        camera.m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera.m_right = glm::vec3(1.0f, 0.0f, 0.0f);
        camera.m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

        camera.m_yaw = c_YAW;
        camera.m_pitch = c_PITCH;
        camera.m_speed = c_SPEED;

        camera.m_zoom = c_ZOOM;
        camera.m_aspect = 1.0f;
        camera.m_nearZ = 0.1f;
        camera.m_farZ = 100.0f;

        camera.m_viewMatrix = glm::mat4(1.0f);
        camera.m_projMatrix = glm::mat4(1.0f);
    }

    return *this;
}

glm::mat4x4 Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix() const
{
    return m_projMatrix;
}

glm::mat4x4 Camera::GetViewProjectionMatrix() const
{
    return m_projMatrix * m_viewMatrix;
}

glm::vec3 Camera::GetPosition() const
{
    return m_position;
}

glm::vec3 Camera::GetUp() const
{
    return m_up;
}

glm::vec3 Camera::GetRight() const
{
    return m_right;
}

glm::vec3 Camera::GetForward() const
{
    return m_forward;
}

float Camera::GetYaw() const
{
    return m_yaw;
}

float Camera::GetPitch() const
{
    return m_pitch;
}

float Camera::GetZoom() const
{
    return m_zoom;
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

void Camera::SetUp(const glm::vec3& up)
{
    m_up = up;
}

void Camera::SetRight(const glm::vec3& right)
{
    m_right = right;
}

void Camera::SetForward(const glm::vec3& forward)
{
    m_forward = forward;
}

void Camera::SetAspectRatio(float aspect)
{
    m_aspect = aspect;
    // regenerate proj matrix
    m_projMatrix = glm::perspective(glm::radians(m_zoom), m_aspect, m_nearZ, m_farZ);
}

void Camera::SetZoomFov(float zoom)
{
    m_zoom = zoom;
    m_projMatrix = glm::perspective(glm::radians(m_zoom), m_aspect, m_nearZ, m_farZ);
}

// translate in view space by x, y, z
void Camera::Translate(glm::vec3 translation)
{
    m_position += translation;
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

// rotate in view space
void Camera::Rotate(glm::vec3 rotation)
{
    Rotate(rotation.x, rotation.y);
    // TODO: implement actual quaternion/rotation matrix rotation later
}

// rotate by yaw and pitch factors
void Camera::Rotate(float yaw, float pitch)
{
    m_yaw += yaw;
    m_pitch += pitch;
    m_yaw = glm::mod(m_yaw, 360.0f); // limit to 2 rad
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f); // clamp pitch to not gimbal lock
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

// zoom in by changing the fov
void Camera::Zoom(float zoom)
{
    m_zoom += zoom;
    m_zoom = glm::clamp(m_zoom, 1.0f, 90.0f); // clamp zoom as to not go too far
    m_projMatrix = glm::perspective(glm::radians(m_zoom), m_aspect, 0.1f, 100.0f);
}

// move along the forward vector by a factor of distance
void Camera::MoveForward(float distance)
{
    m_position += m_forward * distance;

    // DEBUG: Print position
	std::cout << "Camera position: " << m_position.x << ", " << m_position.y << ", " << m_position.z << std::endl;

	// update the view matrix
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

// move along the right vector by a factor of distance
void Camera::MoveRight(float distance)
{
    m_position += m_right * distance;
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

// move along the Up vector by a factor of distance
void Camera::MoveUp(float distance)
{
    m_position += m_up * distance;
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::Update(float)
{
    // update the camera's matrices
	UpdateCameraVectors();
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

// void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
// {
// }
//
// void Camera::ProcessMouseScroll(float yoffset)
// {
// }

// update camera vectors
void Camera::UpdateCameraVectors()
{
    // calculate the new front vector
    glm::vec3 forward(0.f);
    forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    forward.y = sin(glm::radians(m_pitch));
    forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward = glm::normalize(forward);

	// recalculate right and up, normalizing them to ensure the vectors are always unit length
    m_right = glm::normalize(glm::cross(m_forward, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void Camera::UpdateViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::UpdateProjectionMatrix()
{
	m_projMatrix = glm::perspective(glm::radians(m_zoom), m_aspect, m_nearZ, m_farZ);
}

