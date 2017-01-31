#pragma once
#include "vulkandevice.h"

class VulkanRenderPass {
public:
	VulkanRenderPass(std::shared_ptr<VulkanDevice>, VkFormat);
	~VulkanRenderPass();

	std::shared_ptr<VulkanDevice> 	device;
	VkRenderPass 					renderPass;
};