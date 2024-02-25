#include "Light.h"

Light::Light(const glm::vec3& color, float ambientIntensity,
			 const glm::vec3& direction, float diffuseIntensity)
	: m_Color(color),
	m_AmbientIntensity(ambientIntensity),
	m_Direction(direction),
	m_DiffuseIntensity(diffuseIntensity)
{
}

void Light::UseLight(const std::string& colorUniform, 
					 const std::string& ambientIntensityUniform,
					 const std::string& directionUniform,
					 const std::string& diffuseIntensityUniform,
					 Shader& shader) const
{
	shader.SetUniformVec3(colorUniform, m_Color);
	shader.SetUniformF(ambientIntensityUniform, m_AmbientIntensity);
	shader.SetUniformVec3(directionUniform, m_Direction);
	shader.SetUniformF(diffuseIntensityUniform, m_DiffuseIntensity);
}
