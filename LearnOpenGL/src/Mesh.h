#pragma once

#include <GL/glew.h>

#include <vector>

class Mesh
{
public:
	Mesh() = default;
	~Mesh();

	void Create(const std::vector<GLfloat>& vertices,
				const std::vector<GLuint>& indices);
	void Render();
	void Cleanup();

private:
	GLuint m_VAO = 0, m_VBO = 0, m_IBO = 0;
	GLsizei m_IndexCount = 0;
};

