#pragma once
#include "opengldriver.h"
#include "texture.h"
#include "image.h"

class OpenGLTexture : public Texture {
public:
	OpenGLTexture(const std::string &);
	~OpenGLTexture();

	void bind(uint32_t) const;
private:
	std::string name;
	uint32_t textureID;
};