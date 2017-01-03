#pragma once
#include <string>
#include <array>
#include "image.h"


class Texture {
public:
	Texture(const std::string &) {};
	virtual ~Texture() = default;

	virtual void bind(uint32_t) const {};
};