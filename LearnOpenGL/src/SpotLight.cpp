#include "SpotLight.h"

SpotLight::SpotLight(uint32_t shadowMapWidth, uint32_t shadowMapHeight,
					 float nearPlane, float farPlane,
					 const glm::vec3& color, float ambientIntensity, float diffuseIntensity, 
					 const glm::vec3& position, const glm::vec3& direction, 
					 float constant, float linear, float exponent, 
					 float edge)
	: PointLight(shadowMapWidth, shadowMapHeight, 
				 nearPlane, farPlane, 
				 color, ambientIntensity, diffuseIntensity, 
				 position, 
				 constant, linear, exponent),
	m_Direction(direction),
	m_Edge(edge),
	m_EdgeToRadians(cos(glm::radians(m_Edge)))
{
}

void SpotLight::UseLight(const std::string& colorUniform, 
						 const std::string& ambientIntensityUniform,
						 const std::string& diffuseIntensityUniform,
						 const std::string& positionUniform,
						 const std::string& directionUniform,
						 const std::string& constantUniform,
						 const std::string& linearUniform,
						 const std::string& exponentUniform,
						 const std::string& edgeUniform,
						 Shader& shader) const
{
	PointLight::UseLight(colorUniform,
						 ambientIntensityUniform,
						 diffuseIntensityUniform,
						 positionUniform,
						 constantUniform,
						 linearUniform,
						 exponentUniform,
						 shader);

	shader.SetUniformVec3(directionUniform, m_Direction);
	shader.SetUniformF(edgeUniform, m_EdgeToRadians);
}
