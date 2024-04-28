#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap()
	: ShadowMap()
{
}

bool OmniShadowMap::Init(uint32_t width, uint32_t height)
{
	m_Width = width;
	m_Height = height;

	glGenFramebuffers(1, &m_FBO);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

	for (uint32_t face = 0; face < 6; face++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_Texture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Failed to create omni shadow map: " << status << std::endl;
		return false;
	}

	return true;
}

void OmniShadowMap::Use(GLuint textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
}
