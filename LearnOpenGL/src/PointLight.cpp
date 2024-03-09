#include "PointLight.h"

PointLight::PointLight(const glm::vec3& color, float ambientIntensity, float diffuseIntensity,
					   const glm::vec3& position,
					   float constant, float linear, float exponent)
	: Light(color, ambientIntensity, diffuseIntensity),
	m_Position(position),
	m_Constant(constant),
	m_Linear(linear),
	m_Exponent(exponent)
{
}

void PointLight::UseLight(const std::string& colorUniform,
						  const std::string& ambientIntensityUniform,
						  const std::string& diffuseIntensityUniform,
						  const std::string& positionUniform,
						  const std::string& constantUniform,
						  const std::string& linearUniform,
						  const std::string& exponentUniform,
						  Shader& shader) const
{
	Light::UseLight(colorUniform, ambientIntensityUniform, diffuseIntensityUniform, shader);
	shader.SetUniformVec3(positionUniform, m_Position);
	shader.SetUniformF(constantUniform, m_Constant);
	shader.SetUniformF(linearUniform, m_Linear);
	shader.SetUniformF(exponentUniform, m_Exponent);
}
