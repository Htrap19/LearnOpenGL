#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"

class Camera
{
public:
	Camera() = default;
	Camera(const glm::vec3& position, GLfloat yaw, GLfloat pitch, GLfloat movementSpeed, GLfloat turningSpeed);

	void KeyControls(Window& window, GLfloat deltaTime);
	void MouseControls(Window& window);

	glm::mat4 CalculateViewMatrix() const;
	inline glm::vec3 GetPosition() const { return m_Position; }

private:
	void Update();
	
private:
	glm::vec3 m_Position, m_Front, m_Right, m_Up;

	GLfloat m_Yaw = 0.0f, m_Pitch = 0.0f;
	GLfloat m_MovementSpeed, m_TurningSpeed;
};

