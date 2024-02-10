#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	Window() = default;
	Window(const std::string& title, uint32_t width, uint32_t height);
	~Window();

	void Initialize();
	void Cleanup();
	inline bool ShouldClose() const { return glfwWindowShouldClose(m_Window); }
	void SwapBuffers() const { glfwSwapBuffers(m_Window); }

	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }
	inline GLfloat GetBufferWidth() const { return (GLfloat)m_BufferWidth; }
	inline GLfloat GetBufferHeight() const { return (GLfloat)m_BufferHeight; }

	bool IsKeyPressed(uint32_t key) const;

	GLfloat GetDeltaX();
	GLfloat GetDeltaY();

private:
	static void HandleKeyCallback(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleCursorCallback(GLFWwindow* window, double xPos, double yPos);

private:
	uint32_t m_Width = 1280, m_Height = 720;
	int m_BufferWidth = 0, m_BufferHeight = 0;
	std::string m_Title;

	bool m_Keys[GLFW_KEY_LAST + 1];
	GLfloat m_LastX, m_LastY, m_DeltaX, m_DeltaY;
	bool m_FirstMouseCallback = true;
	
	GLFWwindow* m_Window = nullptr;
};

