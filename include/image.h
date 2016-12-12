#pragma once
#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include "referencecounter.h"

class Image : public ReferenceCounter {
public:
	Image(const std::string &);
	~Image();

	inline const std::vector<uint8_t> &getData() const { return imageData; };
	inline int32_t getWidth() const { return width; }; 
	inline int32_t getHeight() const { return height; };
	inline int32_t getChannels() const { return channels; };

private:
	//RGBA format
	std::vector<uint8_t> imageData;
	std::string name;
	int32_t width, height, channels;
};