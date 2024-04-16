#include "Light.h"

Light::Light(uint32_t shadowMapWidth,
			 uint32_t shadowMapHeight,
			 const glm::vec3& color,
			 float ambientIntensity,
			 float diffuseIntensity)
	: m_Color(color),
	m_AmbientIntensity(ambientIntensity),
	m_DiffuseIntensity(diffuseIntensity)
{
	m_ShadowMap.Init(shadowMapWidth, shadowMapHeight);
}

void Light::UseLight(const std::string& colorUniform, 
					 const std::string& ambientIntensityUniform,
					 const std::string& diffuseIntensityUniform,
					 Shader& shader) const
{
	shader.SetUniformVec3(colorUniform, m_Color);
	shader.SetUniformF(ambientIntensityUniform, m_AmbientIntensity);
	shader.SetUniformF(diffuseIntensityUniform, m_DiffuseIntensity);
}
