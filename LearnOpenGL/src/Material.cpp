#include "Material.h"

Material::Material(float specularIntensity, float shininess)
	: m_SpecularIntensity(specularIntensity),
	m_Shininess(shininess)
{
}

void Material::UseMaterial(const std::string& specularIntensityUniform,
						   const std::string& shininessUniform, 
						   Shader& shader)
{
	shader.SetUniformF(specularIntensityUniform, m_SpecularIntensity);
	shader.SetUniformF(shininessUniform, m_Shininess);
}
