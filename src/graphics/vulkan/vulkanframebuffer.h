#pragma once
#include "vulkanrenderpass.h"

class VulkanFramebuffer {
public:
	VulkanFramebuffer(std::shared_ptr<VulkanRenderPass>, const std::vector<VkImageView> &, VkExtent2D, uint32_t);
	~VulkanFramebuffer();

	std::shared_ptr<VulkanDevice> device;
	VkFramebuffer framebuffer;
	std::vector<VkImageView> attachments;
	VkExtent2D dimensions;
	uint32_t layers;
};