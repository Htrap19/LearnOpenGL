#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "Common.h"

class Skybox
{
public:
	Skybox(std::array<std::string, 6> facePaths);

	void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
	uint32_t m_Texture = 0;
	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Shader> m_Shader;
};

