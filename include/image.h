#pragma once
#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "referencecounter.h"

class Image 
{
public:
	Image(const std::string &);
	~Image();

	inline const std::vector<uint8_t> &getData() const { return imageData->data; };
	inline int32_t getWidth() const { return imageData->width; }; 
	inline int32_t getHeight() const { return imageData->height; };
	inline int32_t getChannels() const { return imageData->channels; };

private:
	class ImageData : public ReferenceCounter {
	public:
		ImageData(int32_t width, int32_t height, int32_t channels, const std::vector<uint8_t> &data):
			width(width),
			height(height),
			channels(channels),
			data(std::move(data)) {};

		int32_t width;
		int32_t height;
		int32_t channels;
		std::vector<uint8_t> data;
	};

	//RGBA format
	std::string name;
	std::shared_ptr<ImageData> imageData;

	static std::map<const std::string, std::shared_ptr<ImageData>> imageDataMap;
};