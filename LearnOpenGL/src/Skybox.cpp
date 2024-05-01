#include "Skybox.h"

#include <iostream>
#include <array>

Skybox::Skybox(std::array<std::string, 6> facePaths)
{
	m_Shader = std::make_shared<Shader>();
	m_Shader->CreateFromFile("resources/shaders/skybox.vert.glsl",
							 "resources/shaders/skybox.frag.glsl");

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

	for (uint32_t i = 0; i < 6; i++)
	{
		int width, height, bitDepth;
		stbi_uc* data = stbi_load(facePaths[i].c_str(), &width, &height, &bitDepth, 0);
		if (!data)
		{
			std::cerr << "Unable to load texture: " << facePaths[i].c_str() << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	uint32_t skyboxIndices[] = {
		// front
		0, 1, 2,
		2, 1, 3,
		// right
		2, 3, 5,
		5, 3, 7,
		// back
		5, 7, 4,
		4, 7, 6,
		// left
		4, 6, 0,
		0, 6, 1,
		// top
		4, 0, 5,
		5, 0, 2,
		// bottom
		1, 6, 3,
		3, 6, 7
	};

	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		-1.0f,  1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	m_Mesh = std::make_shared<Mesh>();
	m_Mesh->Create(std::vector<GLfloat>(skyboxVertices, skyboxVertices + 64),
				   std::vector<GLuint>(skyboxIndices, skyboxIndices + 36));
}

void Skybox::Draw(const glm::mat4& view, const glm::mat4& projection)
{
	glDepthMask(GL_FALSE);

	m_Shader->Use();
	m_Shader->SetUniformMat4("u_Projection", projection);
	m_Shader->SetUniformMat4("u_View", glm::mat4(glm::mat3(view)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

	m_Mesh->Render();

	glDepthMask(GL_TRUE);
}
