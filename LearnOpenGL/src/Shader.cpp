#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Common.h"

Shader::~Shader()
{
	Cleanup();
}

void Shader::CreateFromString(const std::string& vertexSource, 
						      const std::string& fragmentSource)
{
	CreateProgram(vertexSource, "", fragmentSource);
}

void Shader::CreateFromFile(const std::string& vertexFile, const std::string& fragmentFile)
{
	std::string vertexSource = ReadFile(vertexFile);
	std::string fragmentSource = ReadFile(fragmentFile);

	CreateProgram(vertexSource, "", fragmentSource);
}

void Shader::CreateFromString(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource)
{
	CreateProgram(vertexSource, geometrySource, fragmentSource);
}

void Shader::CreateFromFile(const std::string& vertexFile, const std::string& geometryFile, const std::string& fragmentFile)
{
	std::string vertexSource = ReadFile(vertexFile);
	std::string geometrySource = ReadFile(geometryFile);
	std::string fragmentSource = ReadFile(fragmentFile);

	CreateProgram(vertexSource, geometrySource, fragmentSource);
}

void Shader::Cleanup()
{
	glDeleteProgram(m_Program);
}

void Shader::Use()
{
	glUseProgram(m_Program);
}

void Shader::SetUniformI(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformF(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value)
{
	glUniform3f(GetUniformLocation(name), value.r, value.g, value.b);
}

void Shader::SetUniformVec4(const std::string& name, const glm::vec4& value)
{
	glUniform4f(GetUniformLocation(name), value.r, value.g, value.b, value.a);
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformMat4v(const std::string& name, const std::vector<glm::mat4>& values)
{
	glUniformMatrix4fv(GetUniformLocation(name), values.size(), GL_FALSE, &values[0][0][0]);
}

void Shader::SetDirectionalLight(const DirectionalLight& light)
{
	light.UseLight("u_DirectionalLight.base.color",
				   "u_DirectionalLight.base.ambientIntensity",
				   "u_DirectionalLight.base.diffuseIntensity",
				   "u_DirectionalLight.direction",
				   *this);
}

void Shader::SetPointLights(const std::vector<std::shared_ptr<PointLight>>& lights)
{
	size_t lightCount = lights.size();
	if (lightCount > MAX_POINTLIGHTS)
		lightCount = MAX_POINTLIGHTS;

	SetUniformI("u_PointLightCount", lightCount);
	std::string pointLightUniform;
	pointLightUniform.resize(100);
	for (size_t i = 0; i < lightCount; i++)
	{
		pointLightUniform = "u_PointLights[" + std::to_string(i) + "]";

		lights[i]->UseLight(pointLightUniform + ".base.color",
						    pointLightUniform + ".base.ambientIntensity",
						    pointLightUniform + ".base.diffuseIntensity",
						    pointLightUniform + ".position",
						    pointLightUniform + ".constant",
						    pointLightUniform + ".linear",
						    pointLightUniform + ".exponent",
						    *this);
	}
}

void Shader::SetSpotLights(const std::vector<std::shared_ptr<SpotLight>>& lights)
{
	size_t lightCount = lights.size();
	if (lightCount > MAX_SPOTLIGHTS)
		lightCount = MAX_SPOTLIGHTS;

	SetUniformI("u_SpotLightCount", lightCount);
	std::string spotLightUniform;
	spotLightUniform.resize(100);
	for (size_t i = 0; i < lightCount; i++)
	{
		spotLightUniform = "u_SpotLights[" + std::to_string(i) + "]";

		lights[i]->UseLight(spotLightUniform + ".base.base.color",
						    spotLightUniform + ".base.base.ambientIntensity",
						    spotLightUniform + ".base.base.diffuseIntensity",
						    spotLightUniform + ".base.position",
						    spotLightUniform + ".direction",
						    spotLightUniform + ".base.constant",
						    spotLightUniform + ".base.linear",
						    spotLightUniform + ".base.exponent",
						    spotLightUniform + ".edge",
						    *this);
	}
}

void Shader::CreateProgram(const std::string& vertexSource, 
						   const std::string& geometrySource,
						   const std::string& fragmentSource)
{
	m_Program = glCreateProgram();
	if (!m_Program)
	{
		std::cerr << "Unable to create shader program!" << std::endl;
		return;
	}

	GLuint vShader = 0, fShader = 0, gShader = 0;
	vShader = CreateShader(vertexSource, GL_VERTEX_SHADER);
	if (vShader != 0)
		glAttachShader(m_Program, vShader);

	if (!geometrySource.empty())
	{
		gShader = CreateShader(geometrySource, GL_GEOMETRY_SHADER);
		if (gShader != 0)
			glAttachShader(m_Program, gShader);
	}

	if (!fragmentSource.empty())
	{
		fShader = CreateShader(fragmentSource, GL_FRAGMENT_SHADER);
		if (fShader != 0)
			glAttachShader(m_Program, fShader);
	}

	glLinkProgram(m_Program);
	
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetProgramiv(m_Program, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_Program, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error linking program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(m_Program);
	glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_Program, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error validating program: " << eLog << std::endl;
		return;
	}

	glDeleteShader(vShader);
	if (gShader)
		glDeleteShader(gShader);
	if (fShader)
		glDeleteShader(fShader);
}

GLuint Shader::CreateShader(const std::string& source, GLenum type)
{
	GLuint shader = glCreateShader(type);

	const GLchar* code[1] = { source.c_str() };
	const GLint codeLength[1] = { source.size() };

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
		std::cerr << "Error compiling " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
			" shader: " << eLog << std::endl;
		return 0;
	}

	return shader;
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		m_UniformLocations[name] = glGetUniformLocation(m_Program, name.c_str());

	return m_UniformLocations[name];
}

std::string Shader::ReadFile(const std::string& file)
{
	std::ifstream fileStream(file, std::ios::in | std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cerr << "Unable to open " << file << " file!" << std::endl;
		return "";
	}

	std::string content;
	fileStream.seekg(0, std::ios::end);
	size_t size = fileStream.tellg();
	if (size != -1)
	{
		content.resize(size);
		fileStream.seekg(0, std::ios::beg);
		fileStream.read(&content[0], (std::streamsize)size);
	}
	else
		std::cerr << "Failed to read file: " << file << std::endl;
	
	return content;
}
