#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader() = default;
	~Shader();

	void CreateFromString(const std::string& vertexSource, 
						  const std::string& fragmentSource);
	void CreateFromFile(const std::string& vertexFile,
						const std::string& fragmentFile);
	void Cleanup();
	void Use();

	void SetUniformMat4(const std::string& name, const glm::mat4& value);

private:
	void CreateProgram(const std::string& vertexSource,
					   const std::string& fragmentSource);
	GLuint CreateShader(const std::string& source, GLenum type);
	GLuint GetUniformLocation(const std::string& name);

	static std::string ReadFile(const std::string& file);

private:
	GLuint m_Program = 0;
	std::unordered_map<std::string, GLuint> m_UniformLocations;
};

