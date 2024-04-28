#include "Light.h"

Light::Light(const glm::vec3& color,
			 float ambientIntensity,
			 float diffuseIntensity)
	: m_Color(color),
	m_AmbientIntensity(ambientIntensity),
	m_DiffuseIntensity(diffuseIntensity)
{
}

Light::~Light()
{
	if (m_ShadowMap)
		delete m_ShadowMap;
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
