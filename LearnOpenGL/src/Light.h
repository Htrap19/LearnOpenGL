#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

#include "Shader.h"
#include "ShadowMap.h"

class Light
{
public:
	Light() = default;
	Light(uint32_t shadowMapWidth,
		  uint32_t shadowMapHeight,
		  const glm::vec3& color,
		  float ambientIntensity,
		  float diffuseIntensity);

	virtual glm::mat4 CalcLightTransform() const { return glm::mat4(1.0f); }
	inline ShadowMap& GetShadowMap() { return m_ShadowMap; }

protected:
	void UseLight(const std::string& colorUniform, 
				  const std::string& ambientIntensityUniform,
				  const std::string& diffuseIntensityUniform,
				  Shader& shader) const;

protected:
	glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };
	float m_AmbientIntensity = 1.0f;
	float m_DiffuseIntensity = 1.0f;

	glm::mat4 m_LightProjection = glm::mat4(1.0f);
	ShadowMap m_ShadowMap;
};

