#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Forward declaration
class DirectionalLight;
class PointLight;
class SpotLight;

class Shader
{
public:
	Shader() = default;
	~Shader();

	void CreateFromString(const std::string& vertexSource, 
						  const std::string& fragmentSource);
	void CreateFromFile(const std::string& vertexFile,
						const std::string& fragmentFile);

	void CreateFromString(const std::string& vertexSource,
						  const std::string& geometrySource,
						  const std::string& fragmentSource);
	void CreateFromFile(const std::string& vertexFile,
						const std::string& geometryFile,
						const std::string& fragmentFile);
	void Cleanup();
	void Use();

	void SetUniformI(const std::string& name, int value);
	void SetUniformF(const std::string& name, float value);
	void SetUniformVec3(const std::string& name, const glm::vec3& value);
	void SetUniformVec4(const std::string& name, const glm::vec4& value);
	void SetUniformMat4(const std::string& name, const glm::mat4& value);
	void SetUniformMat4v(const std::string& name, const std::vector<glm::mat4>& values);

	void SetDirectionalLight(const DirectionalLight& light);
	void SetPointLights(const std::vector<std::shared_ptr<PointLight>>& lights);
	void SetSpotLights(const std::vector<std::shared_ptr<SpotLight>>& lights);

private:
	void CreateProgram(const std::string& vertexSource,
					   const std::string& geometrySource,
					   const std::string& fragmentSource);
	GLuint CreateShader(const std::string& source, GLenum type);
	GLuint GetUniformLocation(const std::string& name);

	static std::string ReadFile(const std::string& file);

private:
	GLuint m_Program = 0;
	std::unordered_map<std::string, GLuint> m_UniformLocations;
};

