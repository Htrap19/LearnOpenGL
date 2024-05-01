#pragma once

#include <GL/glew.h>

#include <iostream>
#include <string>

#include "Common.h"

class Texture
{
public:
	Texture() = default;
	~Texture();

	bool LoadFromFile(const std::string& filepath);
	void UseTexture(uint32_t slot = 0);
	void Clear();

	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }
	inline uint32_t GetBitDepth() const { return m_BitDepth; }

private:
	uint32_t m_Width, m_Height, m_BitDepth;
	std::string m_Filename;
	GLuint m_Texture;
};

