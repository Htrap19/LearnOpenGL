#pragma once

#include <GL/glew.h>

#include <string>

#include "Shader.h"

class Material
{
public:
	Material() = default;
	Material(float specularIntensity, float shininess);

	void UseMaterial(const std::string& specularIntensityUniform,
					 const std::string& shininessUniform,
					 Shader& shader);

private:
	float m_SpecularIntensity = 0.0f;
	float m_Shininess = 0.0f;
};

