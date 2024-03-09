#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& color, float ambientIntensity, float diffuseIntensity, 
								   const glm::vec3& direction)
	: Light(color, ambientIntensity, diffuseIntensity),
	m_Direction(direction)
{
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
