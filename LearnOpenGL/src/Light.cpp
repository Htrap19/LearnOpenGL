#include "Light.h"

Light::Light(const glm::vec3& color, float ambientIntensity)
	: m_Color(color),
	m_AmbientIntensity(ambientIntensity)
{
}

void Light::UseLight(const std::string& colorUniform, 
					 const std::string& ambientIntensityUniform, 
					 Shader& shader) const
{
	shader.SetUniformVec3(colorUniform, m_Color);
	shader.SetUniformF(ambientIntensityUniform, m_AmbientIntensity);
}
