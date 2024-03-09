#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight() = default;
	DirectionalLight(const glm::vec3& color, float ambientIntensity, float diffuseIntensity,
					 const glm::vec3& direction);

	void UseLight(const std::string& colorUniform,
				  const std::string& ambientIntensityUniform,
				  const std::string& diffuseIntensityUniform,
				  const std::string& directionUniform,
				  Shader& shader) const;

private:
	glm::vec3 m_Direction = { 0.0f, -1.0f, 0.0f };
};

