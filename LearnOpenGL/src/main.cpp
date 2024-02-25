#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

std::vector<Mesh> s_MeshList;
std::vector<Shader> s_ShaderList;

static void CalculateAverageNormals(GLuint* indices, GLsizei numberOfIndices,
									GLfloat* vertices, GLsizei numberOfVertices,
									GLuint stride, GLuint normalOffset)
{
	for (size_t i = 0; i < numberOfIndices; i += 3)
	{
		GLuint index0 = indices[i]     * stride;
		GLuint index1 = indices[i + 1] * stride;
		GLuint index2 = indices[i + 2] * stride;

		glm::vec3 v1(vertices[index1] - vertices[index0], vertices[index1 + 1] - vertices[index0 + 1], vertices[index1 + 2] - vertices[index0 + 2]);
		glm::vec3 v2(vertices[index2] - vertices[index0], vertices[index2 + 1] - vertices[index0 + 1], vertices[index2 + 2] - vertices[index0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		index0 += normalOffset, index1 += normalOffset, index2 += normalOffset;
		vertices[index0] += normal.x, vertices[index0 + 1] += normal.y, vertices[index0 + 2] += normal.z;
		vertices[index1] += normal.x, vertices[index1 + 1] += normal.y, vertices[index1 + 2] += normal.z;
		vertices[index2] += normal.x, vertices[index2 + 1] += normal.y, vertices[index2 + 2] += normal.z;
	}

	for (size_t i = 0; i < (numberOfVertices / stride); i++)
	{
		GLuint offset = (i * stride) + normalOffset;
		glm::vec3 vec(vertices[offset], vertices[offset + 1], vertices[offset + 2]);
		vec = glm::normalize(vec);
		vertices[offset] = vec.x, vertices[offset + 1] = vec.y, vertices[offset + 2] = vec.z;
	}
}

static void CreateObjects()
{
	GLuint indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2,
	};
	GLsizei numOfIndices = sizeof(indices) / sizeof(indices[0]);

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Bottom left
		 0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Bottom right
		 0.0f,  1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f  // Middle top
	};
	GLsizei numOfVertices = sizeof(vertices) / sizeof(vertices[0]);

	CalculateAverageNormals(indices, numOfIndices, vertices, numOfVertices, 8, 5);

	Mesh& firstPyramid = s_MeshList.emplace_back();
	firstPyramid.Create(std::vector<GLfloat>(vertices, vertices + numOfVertices), 
						std::vector<GLuint>(indices, indices + numOfIndices));

	Mesh& secondPyramid = s_MeshList.emplace_back();
	secondPyramid.Create(std::vector<GLfloat>(vertices, vertices + numOfVertices),
						 std::vector<GLuint>(indices, indices + numOfIndices));
}

void CreateShader()
{
	auto& shader = s_ShaderList.emplace_back();
	shader.CreateFromFile("resources/shaders/shader.vert.glsl",
						  "resources/shaders/shader.frag.glsl");
}

int main(int argc, char* argv[])
{
	Window window;
	window.Initialize();

	CreateObjects();
	CreateShader();

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	Texture redBrickTexture;
	redBrickTexture.LoadFromFile("resources/textures/brick.png");
	Texture dirtTexture;
	dirtTexture.LoadFromFile("resources/textures/dirt.png");

	Light mainLight(glm::vec3{ 1.0f, 1.0f, 1.0f }, 0.2f, glm::vec3{ 2.0f, -1.0f, -2.0f }, 1.0f);

	glm::mat4 projection = glm::perspective(45.0f, (window.GetBufferWidth() / window.GetBufferHeight()), 0.1f, 100.0f);

	double lastTime = 0.0f;
	double deltaTime = 0.0f;

	while (!window.ShouldClose())
	{
		glfwPollEvents();

		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		camera.KeyControls(window, deltaTime);
		camera.MouseControls(window);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto& shader = s_ShaderList[0];
		shader.Use();
		shader.SetUniformMat4("u_Projection", projection);
		shader.SetUniformMat4("u_View", camera.CalculateViewMatrix());
		mainLight.UseLight("u_DirectionalLight.color",
						   "u_DirectionalLight.ambientIntensity",
						   "u_DirectionalLight.direction",
						   "u_DirectionalLight.diffuseIntensity",
						   shader);

		// Render first pyramid
		shader.SetUniformMat4("u_Model", model);
		redBrickTexture.UseTexture();
		s_MeshList[0].Render();

		// Render second pyramid
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		shader.SetUniformMat4("u_Model", model);
		dirtTexture.UseTexture();
		s_MeshList[1].Render();

		glUseProgram(0);

		window.SwapBuffers();
	}

	return 0;
}