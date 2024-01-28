#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static uint32_t s_Width = 800;
static uint32_t s_Height = 600;

static GLuint s_VAO, s_VBO, s_Shader;

// Vertex Shader
static const char* vShader = "											\n\
#version 330															\n\
																		\n\
layout(location = 0) in vec3 a_Pos;										\n\
																		\n\
void main()																\n\
{																		\n\
	gl_Position = vec4(0.4 * a_Pos.x, 0.4 * a_Pos.y, a_Pos.z, 1.0);		\n\
}";

// Fragment Shader
static const char* fShader = "											\n\
#version 330															\n\
																		\n\
out vec4 o_Color;														\n\
																		\n\
void main()																\n\
{																		\n\
	o_Color = vec4(0.2, 0.1, 0.3, 1.0);									\n\
}";

static void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, // Bottom left
		 1.0f, -1.0f, 0.0f, // Bottom right
		 0.0f,  1.0f, 0.0f  // Middle top
	};

	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);

	glGenBuffers(1, &s_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vertex buffer

	glBindVertexArray(0); // unbind vertex array
}

static void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1] = { shaderCode };
	const GLint codeLength[1] = { strlen(shaderCode) };

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error compiling " << 
			(shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") 
			<< " shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(shaderProgram, shader);
}

static void CompileShader()
{
	s_Shader = glCreateProgram();

	if (!s_Shader)
	{
		std::cerr << "Unable to create shader!." << std::endl;
		return;
	}

	AddShader(s_Shader, vShader, GL_VERTEX_SHADER);
	AddShader(s_Shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(s_Shader);
	glGetProgramiv(s_Shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(s_Shader, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error linking program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(s_Shader);
	glGetProgramiv(s_Shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(s_Shader, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error validating program: " << eLog << std::endl;
		return;
	}
}

int main(int argc, char* argv[])
{
	if (!glfwInit())
	{
		std::cerr << "Unable to initialize GLFW!." << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(s_Width, s_Height, "Test Window", nullptr, nullptr);
	if (!mainWindow)
	{
		std::cerr << "Unable to create GLFW window!." << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(mainWindow);

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW!." << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShader();

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(s_Shader);

		glBindVertexArray(s_VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}