#include "Mesh.h"

Mesh::~Mesh()
{
	Cleanup();
}

void Mesh::Create(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
{
	m_IndexCount = indices.size();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	size_t stride = sizeof(vertices[0]) * 8;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(vertices[0]) * 5));

	glBindVertexArray(0);						// unbind vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);			// unbind vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	// unbind index buffer
}

void Mesh::Render()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Cleanup()
{
	glDeleteBuffers(1, &m_IBO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);

	m_IBO = 0;
	m_VBO = 0;
	m_VAO = 0;
}

