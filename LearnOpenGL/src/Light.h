#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

#include "Shader.h"

class Light
{
public:
	Light() = default;
	Light(const glm::vec3& color, float ambientIntensity);

	void UseLight(const std::string& colorUniform, 
				  const std::string& ambientIntensityUniform,
				  Shader& shader) const;

private:
	glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };
	float m_AmbientIntensity = 1.0f;
};

