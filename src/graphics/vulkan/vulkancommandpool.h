#pragma once
#include "vulkandevice.h"
#include <mutex>

class VulkanCommandPool {
public:
	VulkanCommandPool(VulkanDevice &device, uint32_t queueFamilyIndex, bool transistent, bool resetCommandBuffer);
	VulkanCommandPool(const VulkanCommandPool &) = delete;
	~VulkanCommandPool();

	std::vector<VkCommandBuffer> allocateCommandBuffers(bool secondary, uint32_t count);
	void freeCommandBuffers(std::vector<VkCommandBuffer> buffers);

	inline uint32_t family() const { return queueFamily; };

private:
	VulkanDevice &device;
	VkCommandPool commandPool;
	std::mutex cmdPoolMutex;
	uint32_t queueFamily;
};