#pragma once
#include <glm/glm.hpp>
#include "texture.h"
// #include "shader.h"
#include <string>
#include <map>

class Material {
public:
	Material(Texture *, const glm::vec4 &color = glm::vec4(1.0), float intensity = 1.0f, float specularity = 32.0f);

	void bind() const;

	inline float getFloat(const std::string &name) const { return floatValues.at(name); };
	inline const glm::vec4 &getColor() const { return color; };
	inline Texture *getTexture() const { return mainTexture; };

private:
	std::map<const std::string, float> floatValues;
	glm::vec4 color;
	Texture *mainTexture;
};