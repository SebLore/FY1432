#include "Camera.h"

Camera::Camera(float width, float height)
{
	// initialize camera with default values: up y+, right x+, forward z-
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right = glm::vec3(1.0f, 0.0f, 0.0f);

	// initialize camera angles to default values
	m_yaw = c_YAW;
	m_pitch = c_PITCH;
	m_zoom = c_ZOOM;
	m_speed = c_SPEED;

	// create the view matrix
	m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
	m_projMatrix = glm::perspective(glm::radians(m_zoom), width / height, 0.1f, 100.0f);

}
