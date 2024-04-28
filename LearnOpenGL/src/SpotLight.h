#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight() = default;
	SpotLight(uint32_t shadowMapWidth, uint32_t shadowMapHeight,
			  float nearPlane, float farPlane,
			  const glm::vec3& color, float ambientIntensity, float diffuseIntensity,
			  const glm::vec3& position,
			  const glm::vec3& direction,
			  float constant, float linear, float exponent,
			  float edge);

	void UseLight(const std::string& colorUniform,
				  const std::string& ambientIntensityUniform,
				  const std::string& diffuseIntensityUniform,
				  const std::string& positionUniform,
				  const std::string& directionUniform,
				  const std::string& constantUniform,
				  const std::string& linearUniform,
				  const std::string& exponentUniform,
				  const std::string& edgeUniform,
				  Shader& shader) const;

	inline void SetFlash(const glm::vec3& position, const glm::vec3& direction)
	{ m_Position = position, m_Direction = direction; }

private:
	glm::vec3 m_Direction = { 0.0f, -1.0f, 0.0f };
	float m_Edge = 0.0f, m_EdgeToRadians = 0.0f;
};

