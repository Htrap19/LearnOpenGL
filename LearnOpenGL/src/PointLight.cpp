#include "PointLight.h"

#include "OmniShadowMap.h"

#include <glm/gtc/matrix_transform.hpp>

PointLight::PointLight(uint32_t shadowMapWidth, uint32_t shadowMapHeight,
					   float nearPlane, float farPlane,
					   const glm::vec3& color, float ambientIntensity, float diffuseIntensity,
					   const glm::vec3& position,
					   float constant, float linear, float exponent)
	: Light(color, ambientIntensity, diffuseIntensity),
	m_Position(position),
	m_Constant(constant),
	m_Linear(linear),
	m_Exponent(exponent),
	m_FarPlane(farPlane),
	m_NearPlane(nearPlane)
{
	m_ShadowMap = new OmniShadowMap();
	m_ShadowMap->Init(shadowMapWidth, shadowMapHeight);

	float aspect = (float)shadowMapWidth / (float)shadowMapHeight;
	m_LightProjection = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);
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

std::array<glm::mat4, 6> PointLight::CalcLightTransforms() const
{
	std::array<glm::mat4, 6> lightMatrices = {};

	// +x, -x
	lightMatrices[0] = m_LightProjection * glm::lookAt(m_Position, m_Position + glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightMatrices[1] = m_LightProjection * glm::lookAt(m_Position, m_Position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	// +y, -y
	lightMatrices[2] = m_LightProjection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f,  1.0f));
	lightMatrices[3] = m_LightProjection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// +z, -z
	lightMatrices[4] = m_LightProjection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	lightMatrices[5] = m_LightProjection * glm::lookAt(m_Position, m_Position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	return std::move(lightMatrices);
}
