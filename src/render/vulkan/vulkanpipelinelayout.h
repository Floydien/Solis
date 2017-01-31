#pragma once
#include "vulkandevice.h"

class VulkanPipelineLayout {
public:
	VulkanPipelineLayout(std::shared_ptr<VulkanDevice>, std::vector<VkDescriptorSetLayout>);
	~VulkanPipelineLayout();


	std::shared_ptr<VulkanDevice> device;
	VkPipelineLayout layout;
	std::vector<VkDescriptorSetLayout> descriptorLayouts;
};