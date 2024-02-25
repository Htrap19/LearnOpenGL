#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

#include "Shader.h"

class Light
{
public:
	Light() = default;
	Light(const glm::vec3& color, float ambientIntensity,
		  const glm::vec3& direction, float diffuseIntensity);

	void UseLight(const std::string& colorUniform, 
				  const std::string& ambientIntensityUniform,
				  const std::string& directionUniform,
				  const std::string& diffuseIntensityUniform,
				  Shader& shader) const;

private:
	glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };
	float m_AmbientIntensity = 1.0f;

	glm::vec3 m_Direction = { 0.0f, -1.0f, 0.0f };
	float m_DiffuseIntensity = 1.0f;
};

