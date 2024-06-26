#include "Window.h"

#include <iostream>

static size_t s_WindowCount = 0;

Window::Window(const std::string& title, uint32_t width, uint32_t height)
	: m_Title(title), 
	m_Width(width),
	m_Height(height)
{
}

Window::~Window()
{
	Cleanup();
}

void Window::Initialize()
{
	if (s_WindowCount <= 0)
	{
		if (!glfwInit())
		{
			std::cerr << "Unable to initialize GLFW!." << std::endl;
			glfwTerminate();
			return;
		}
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (!m_Window)
	{
		std::cerr << "Unable to create GLFW window!." << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);

	glfwGetFramebufferSize(m_Window, &m_BufferWidth, &m_BufferHeight);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW!." << std::endl;
		Cleanup();
		return;
	}

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1); // enable vsync

	// Initialize input callbacks
	glfwSetKeyCallback(m_Window, HandleKeyCallback);
	glfwSetCursorPosCallback(m_Window, HandleCursorCallback);

	glEnable(GL_DEPTH_TEST);

	SetDefaultViewport();
}

void Window::Cleanup()
{
	glfwDestroyWindow(m_Window);
	s_WindowCount--;
	if (s_WindowCount <= 0)
		glfwTerminate();
}

void Window::SetDefaultViewport()
{
	SetViewport(m_BufferWidth, m_BufferHeight);
}

void Window::SetViewport(uint32_t width, uint32_t height)
{
	glViewport(0, 0, width, height);
}

bool Window::IsKeyPressed(uint32_t key) const
{
	return m_Keys[key];
}

GLfloat Window::GetDeltaX()
{
	auto delta = m_DeltaX;
	m_DeltaX = 0.0;
	return delta;
}

GLfloat Window::GetDeltaY()
{
	auto delta = m_DeltaY;
	m_DeltaY = 0.0;
	return delta;
}

void Window::HandleKeyCallback(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GL_TRUE);

	win->m_Keys[key] = action == GLFW_PRESS || action == GLFW_REPEAT;
}

void Window::HandleCursorCallback(GLFWwindow* window, double xPos, double yPos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (win->m_FirstMouseCallback)
	{
		win->m_LastX = xPos;
		win->m_LastY = yPos;
		win->m_FirstMouseCallback = false;
	}

	win->m_DeltaX = xPos - win->m_LastX;
	win->m_DeltaY = win->m_LastY - yPos;

	win->m_LastX = xPos;
	win->m_LastY = yPos;
}
