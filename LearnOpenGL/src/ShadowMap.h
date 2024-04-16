#pragma once

#include <GL/glew.h>

#include <iostream>

class ShadowMap
{
public:
	ShadowMap() = default;
	~ShadowMap();

	bool Init(uint32_t width, uint32_t height);
	void Write();
	void Use(GLuint textureUnit);
	void Clear();

	inline uint32_t GetTextureWidth() const { return m_Width; }
	inline uint32_t GetTextureHeight() const { return m_Height; }

private:
	GLuint m_FBO = 0, m_Texture = 0;
	uint32_t m_Width = 0, m_Height = 0;
};

