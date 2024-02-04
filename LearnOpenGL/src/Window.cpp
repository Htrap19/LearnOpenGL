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

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, m_BufferWidth, m_BufferHeight);
}

void Window::Cleanup()
{
	glfwDestroyWindow(m_Window);
	s_WindowCount--;
	if (s_WindowCount <= 0)
		glfwTerminate();
}
