#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static uint32_t s_Width = 800;
static uint32_t s_Height = 600;

static GLuint s_VAO, s_VBO, s_IBO, s_Shader, s_UniformModel, s_UniformProjection;

// Vertex Shader
static const char* vShader = "														\n\
#version 330																		\n\
																					\n\
layout(location = 0) in vec3 a_Pos;													\n\
																					\n\
out vec4 v_VCol;																	\n\
																					\n\
uniform mat4 u_Model;																\n\
uniform mat4 u_Projection;															\n\
																					\n\
void main()																			\n\
{																					\n\
	gl_Position = u_Projection * u_Model * vec4(a_Pos, 1.0);						\n\
	v_VCol = vec4(clamp(a_Pos, 0.0, 1.0), 1.0);										\n\
}";

// Fragment Shader
static const char* fShader = "														\n\
#version 330																		\n\
																					\n\
in vec4 v_VCol;																		\n\
																					\n\
out vec4 o_Color;																	\n\
																					\n\
void main()																			\n\
{																					\n\
	o_Color = v_VCol;																\n\
}";

static void CreateTriangle()
{
	uint32_t indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, // Bottom left
		 0.0f, -1.0f, 1.0f, 
		 1.0f, -1.0f, 0.0f, // Bottom right
		 0.0f,  1.0f, 0.0f  // Middle top
	};

	glGenVertexArrays(1, &s_VAO);
	glBindVertexArray(s_VAO);

	glGenBuffers(1, &s_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &s_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, s_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind index buffer

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

	s_UniformModel = glGetUniformLocation(s_Shader, "u_Model");
	s_UniformProjection = glGetUniformLocation(s_Shader, "u_Projection");
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

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShader();

	bool direction = true; // True = Right, False = Left
	float xOffset = 0.0f;
	float xMaxOffset = 0.7f;
	float xOffsetIncrement = 0.005f;
	
	float curAngle = 0.0f;

	bool sizeDirection = true; // True = Stretch, False = Compress
	float curSize = 0.4f, maxSize = 0.8f, minSize = 0.1f;

	glm::mat4 projection = glm::perspective(45.0f, ((GLfloat)bufferWidth / (GLfloat)bufferHeight), 0.1f, 100.0f);

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

		xOffset += direction ? xOffsetIncrement : -xOffsetIncrement;
		
		if (std::abs(xOffset) >= xMaxOffset)
			direction = !direction;

		curAngle = curAngle >= 360.0f ? 0 : curAngle + 0.5f;

		curSize += sizeDirection ? 0.001f : -0.001f;

		if (curSize > maxSize || curSize < minSize)
			sizeDirection = !sizeDirection;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, glm::radians(curAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(s_Shader);

		glUniformMatrix4fv(s_UniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(s_UniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(s_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

	return 0;
}