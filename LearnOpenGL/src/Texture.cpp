#include "Texture.h"

Texture::~Texture()
{
	ClearTexture();
}

bool Texture::LoadFromFile(const std::string& filepath)
{
	int width, height, bitDepth;
	uint8_t* data = stbi_load(filepath.c_str(), &width, &height, &bitDepth, 0);
	if (!data)
	{
		std::cerr << "Unable to load texture: " << filepath << std::endl;
		return false;
	}

	m_Width = width;
	m_Height = height;
	m_BitDepth = bitDepth;

	GLenum internalFormat = GL_RED, format = GL_RED;
	switch (bitDepth)
	{
	case 3:
		internalFormat = GL_RGB8, format = GL_RGB;
		break;
	case 4:
		internalFormat = GL_RGBA8, format = GL_RGBA;
		break;
	}

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return true;
}

void Texture::UseTexture(uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &m_Texture);
}
