#include <iostream>
#include <memory>

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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

std::vector<std::shared_ptr<Mesh>> s_MeshList;
std::vector<Shader> s_ShaderList;
Shader directionShadowMapShader;

Texture redBrickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial(1.0f, 32);
Material dullMaterial(0.3f, 4);

Model uh60;

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
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Bottom left
		 0.0f, -1.0f,  1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f, // Bottom right
		 0.0f,  1.0f,  0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f  // Middle top
	};
	GLsizei numOfVertices = sizeof(vertices) / sizeof(vertices[0]);

	CalculateAverageNormals(indices, numOfIndices, vertices, numOfVertices, 8, 5);

	auto firstPyramid = std::make_shared<Mesh>();
	firstPyramid->Create(std::vector<GLfloat>(vertices, vertices + numOfVertices), 
						std::vector<GLuint>(indices, indices + numOfIndices));
	s_MeshList.push_back(firstPyramid);

	auto secondPyramid = std::make_shared<Mesh>();
	secondPyramid->Create(std::vector<GLfloat>(vertices, vertices + numOfVertices),
						 std::vector<GLuint>(indices, indices + numOfIndices));
	s_MeshList.push_back(secondPyramid);

	GLfloat floorVertices[] = {
		-50.0f, 0.0f, -50.0f,		 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 50.0f, 0.0f, -50.0f,		50.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		-50.0f, 0.0f,  50.0f,		 0.0f, 50.0f,	0.0f, -1.0f, 0.0f,
		 50.0f, 0.0f,  50.0f,		50.0f, 50.0f,	0.0f, -1.0f, 0.0f
	};
	GLsizei numOfFloorVertices = sizeof(floorVertices) / sizeof(floorVertices[0]);

	GLuint floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};
	GLsizei numOfFloorIndices = sizeof(floorIndices) / sizeof(floorIndices[0]);

	auto floor = std::make_shared<Mesh>();
	floor->Create(std::vector<GLfloat>(floorVertices, floorVertices + numOfFloorVertices),
				 std::vector<GLuint>(floorIndices, floorIndices + numOfFloorIndices));
	s_MeshList.push_back(floor);
}

void CreateShader()
{
	auto& shader = s_ShaderList.emplace_back();
	shader.CreateFromFile("resources/shaders/shader.vert.glsl",
						  "resources/shaders/shader.frag.glsl");

	directionShadowMapShader.CreateFromFile("resources/shaders/directional_shadowmap.vert.glsl",
											"resources/shaders/directional_shadowmap.frag.glsl");
}

void RenderScene(Window& window,
				 DirectionalLight& mainLight,
				 std::vector<PointLight>& pointLights,
				 std::vector<SpotLight>& spotLights,
				 Camera& camera,
				 Shader& shader,
				 const glm::mat4& projection)
{
	// Render first pyramid
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	shader.SetUniformMat4("u_Model", model);
	redBrickTexture.UseTexture();
	shinyMaterial.UseMaterial("u_Material.specularIntensity",
							  "u_Material.shininess",
							  shader);
	s_MeshList[0]->Render();

	// Render second pyramid
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, -2.5f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	shader.SetUniformMat4("u_Model", model);
	dirtTexture.UseTexture();
	dullMaterial.UseMaterial("u_Material.specularIntensity",
							 "u_Material.shininess",
							 shader);
	s_MeshList[1]->Render();

	// Render floor
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
	shader.SetUniformMat4("u_Model", model);
	dirtTexture.UseTexture();
	shinyMaterial.UseMaterial("u_Material.specularIntensity",
							  "u_Material.shininess",
							  shader);
	s_MeshList[2]->Render();

	model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, -4.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f));
	shader.SetUniformMat4("u_Model", model);
	shinyMaterial.UseMaterial("u_Material.specularIntensity",
							  "u_Material.shininess",
							  shader);
	uh60.Render();
}

void ShadowPass(Window& window,
				DirectionalLight& light,
				std::vector<PointLight>& pointLights,
				std::vector<SpotLight>& spotLights,
				Camera& camera)
{
	directionShadowMapShader.Use();
	auto& shadowMap = light.GetShadowMap();

	window.SetViewport(shadowMap.GetTextureWidth(), shadowMap.GetTextureHeight());

	shadowMap.Write();
	glClear(GL_DEPTH_BUFFER_BIT);
	directionShadowMapShader.SetUniformMat4("u_DirectionalLightTransform", light.CalcLightTransform());

	RenderScene(window, light, pointLights, spotLights, camera, directionShadowMapShader, glm::mat4(1.0f));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(Window& window,
				DirectionalLight& mainLight,
				std::vector<PointLight>& pointLights,
				std::vector<SpotLight>& spotLights,
				Camera& camera,
				const glm::mat4& projection)
{
	auto& shader = s_ShaderList[0];
	shader.Use();
	window.SetDefaultViewport();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.SetUniformMat4("u_Projection", projection);
	shader.SetUniformMat4("u_View", camera.CalculateViewMatrix());
	shader.SetUniformVec3("u_EyePosition", camera.GetPosition());
	mainLight.GetShadowMap().Use(1);
	shader.SetUniformI("u_ShadowMap", 1);
	shader.SetUniformMat4("u_DirectionalLightTransform", mainLight.CalcLightTransform());

	glm::vec3 lowerLight = camera.GetPosition();
	lowerLight.y -= 0.3f;
	//spotLights[1].SetFlash(lowerLight, camera.GetFront());

	shader.SetDirectionalLight(mainLight);
	shader.SetPointLights(pointLights);
	shader.SetSpotLights(spotLights);

	RenderScene(window, mainLight, pointLights, spotLights, camera, shader, projection);

	glUseProgram(0);
}

int main(int argc, char* argv[])
{
	Window window("LearnOpenGL", 1366, 768);
	window.Initialize();

	CreateObjects();
	CreateShader();

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	redBrickTexture.LoadFromFile("resources/textures/brick.png");
	dirtTexture.LoadFromFile("resources/textures/dirt.png");
	plainTexture.LoadFromFile("resources/textures/plain.png");

	DirectionalLight mainLight(4096, 4096,
							   glm::vec3{ 1.0f, 1.0f, 1.0f }, 
							   0.1f, 0.6f,
							   glm::vec3{ 0.0f, -7.0f, 2.0f });

	std::vector<PointLight> pointLights;
	pointLights.emplace_back(glm::vec3{ 0.0f, 0.0f, 1.0f },
							 0.1f, 1.0f,
							 glm::vec3{ 4.0f, 0.0f, 0.0f },
							 0.3f, 0.2f, 0.1f);
	pointLights.emplace_back(glm::vec3{ 0.0f, 1.0f, 0.0f },
							 0.1f, 1.0f,
							 glm::vec3{ -4.0f, 2.0f, 0.0f },
							 0.3f, 0.2f, 0.1f);

	std::vector<SpotLight> spotLights;
	spotLights.emplace_back(glm::vec3{ 1.0f, 1.0f, 1.0f },
							0.1f, 1.0f,
							glm::vec3{ 8.0f, 4.0f, 0.0f },
							glm::vec3{ 0.0f, -1.0f, 0.0f },
							0.3f, 0.2f, 0.1f,
							20.0f);
	spotLights.emplace_back(glm::vec3{ 1.0f, 1.0f, 1.0f },
							0.1f, 1.0f,
							glm::vec3{ 0.0f, 0.0f, 0.0f },
							glm::vec3{ 0.0f, -1.0f, 0.0f },
							0.3f, 0.2f, 0.1f,
							20.0f);
	
	uh60.Load("resources/models/uh60.obj");

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

		ShadowPass(window, mainLight, pointLights, spotLights, camera);
		RenderPass(window, mainLight, pointLights, spotLights, camera, projection);

		window.SwapBuffers();
	}

	return 0;
}