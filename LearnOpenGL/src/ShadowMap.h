#pragma once

#include <GL/glew.h>

#include <iostream>

class ShadowMap
{
public:
	ShadowMap() = default;
	~ShadowMap();

	virtual bool Init(uint32_t width, uint32_t height);
	virtual void Write();
	virtual void Use(GLuint textureUnit);
	virtual void Clear();

	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }

protected:
	GLuint m_FBO = 0, m_Texture = 0;
	uint32_t m_Width = 0, m_Height = 0;
};

