#pragma once

#include "ShadowMap.h"

class OmniShadowMap : public ShadowMap
{
public:
	OmniShadowMap();
	~OmniShadowMap() = default;

	virtual bool Init(uint32_t width, uint32_t height) override;
	virtual void Use(GLuint textureUnit) override;
};

