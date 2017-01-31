#pragma once
#include "vulkandevice.h"
#include "image.h"

class VulkanTexture {
public:
	VulkanTexture(std::shared_ptr<VulkanDevice>, std::shared_ptr<VulkanQueue>, const Image &);
	~VulkanTexture();

	std::shared_ptr<VulkanDevice> device;
	VkImage image;
	VkImageView imageView;
	VkDeviceMemory memory;

private:
	void createImage(const Image &, VkFormat, VkImageTiling, VkImageUsageFlags, VkMemoryPropertyFlags, VkImage &, VkDeviceMemory &);
	void transitionImageLayout(VkImage, VkFormat, VkImageLayout, VkImageLayout, std::shared_ptr<VulkanQueue>);
	void copyImage(VkImage, uint32_t, uint32_t, std::shared_ptr<VulkanQueue> );
};