#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight() = default;
	PointLight(const glm::vec3& color, float ambientIntensity, float diffuseIntensity,
			   const glm::vec3& position,
			   float constant, float linear, float exponent);

	void UseLight(const std::string& colorUniform,
				  const std::string& ambientIntensityUniform,
				  const std::string& diffuseIntensityUniform,
				  const std::string& positionUniform,
				  const std::string& constantUniform,
				  const std::string& linearUniform,
				  const std::string& exponentUniform,
				  Shader& shader) const;

protected:
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Constant = 1.0f, m_Linear = 0.0f, m_Exponent = 0.0f;
};

