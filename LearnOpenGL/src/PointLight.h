#pragma once

#include "Light.h"

#include <array>

class PointLight : public Light
{
public:
	PointLight() = default;
	PointLight(uint32_t shadowMapWidth, uint32_t shadowMapHeight,
			   float nearPlane, float farPlane,
			   const glm::vec3& color, float ambientIntensity, float diffuseIntensity,
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

	inline glm::vec3 GetPosition() const { return m_Position; }
	inline float GetFarPlane() const { return m_FarPlane; }

	std::array<glm::mat4, 6> CalcLightTransforms() const;

protected:
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Constant = 1.0f, m_Linear = 0.0f, m_Exponent = 0.0f;
	float m_NearPlane = 0.0f, m_FarPlane = 0.0f;
};

