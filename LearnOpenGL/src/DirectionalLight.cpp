#include "DirectionalLight.h"

#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight(uint32_t shadowMapWidth, uint32_t shadowMapHeight,
								   const glm::vec3& color, float ambientIntensity, float diffuseIntensity, 
								   const glm::vec3& direction)
	: Light(shadowMapWidth, shadowMapHeight, color, ambientIntensity, diffuseIntensity),
	m_Direction(direction)
{
	m_LightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(const std::string& colorUniform,
								const std::string& ambientIntensityUniform,
								const std::string& diffuseIntensityUniform,
								const std::string& directionUniform,
								Shader& shader) const
{
	Light::UseLight(colorUniform, ambientIntensityUniform, diffuseIntensityUniform, shader);
	shader.SetUniformVec3(directionUniform, m_Direction);
}

glm::mat4 DirectionalLight::CalcLightTransform() const
{
	return m_LightProjection * glm::lookAt(-m_Direction,
										   glm::vec3(0.0f, 0.0f, 0.0f),
										   glm::vec3(0.0f, 1.0f, 0.0f));
}
