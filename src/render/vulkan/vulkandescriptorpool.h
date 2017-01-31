#pragma once
#include "vulkandevice.h"
#include <mutex>

class VulkanDescriptorPool {
public:
	VulkanDescriptorPool(std::shared_ptr<VulkanDevice> device, std::vector<VkDescriptorPoolSize> poolSizes, uint32_t maxSets);
	~VulkanDescriptorPool();

	std::vector<VkDescriptorSet> alloc(std::vector<VkDescriptorSetLayout>);
	void free(std::vector<VkDescriptorSet> &);

private:
	std::shared_ptr<VulkanDevice> device;
	std::mutex descriptorMutex;
	VkDescriptorPool descriptorPool;
};