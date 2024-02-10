#include "Camera.h"

static glm::vec3 s_WorldUp = { 0.0f, 1.0f, 0.0f };

Camera::Camera(const glm::vec3& position, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turningSpeed)
	: m_Position(position),
	m_Front{ 0.0f, 0.0f, 1.0f },
	m_Yaw(yaw),
	m_Pitch(pitch),
	m_MovementSpeed(movementSpeed),
	m_TurningSpeed(turningSpeed)
{
	Update();
}

void Camera::KeyControls(Window& window, GLfloat deltaTime)
{
	auto velocity = deltaTime * m_MovementSpeed;

	// WASD Controller
	if (window.IsKeyPressed(GLFW_KEY_W))
		m_Position += m_Front * velocity;
	if (window.IsKeyPressed(GLFW_KEY_S))
		m_Position -= m_Front * velocity;
	if (window.IsKeyPressed(GLFW_KEY_A))
		m_Position -= m_Right * velocity;
	if (window.IsKeyPressed(GLFW_KEY_D))
		m_Position += m_Right * velocity;

	if (window.IsKeyPressed(GLFW_KEY_SPACE))
		m_Position += s_WorldUp * velocity;
	if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		m_Position -= s_WorldUp * velocity;
}

void Camera::MouseControls(Window& window)
{
	auto deltaX = window.GetDeltaX() * m_TurningSpeed;
	auto deltaY = window.GetDeltaY() * m_TurningSpeed;

	m_Yaw += deltaX;
	m_Pitch += deltaY;

	m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

	Update();
}

glm::mat4 Camera::CalculateViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::Update()
{
	m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(m_Front);

	m_Right = glm::normalize(glm::cross(m_Front, s_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
