#pragma once
#include "vulkandevice.h"
#include <mutex>

class VulkanQueue {
public:
	VulkanQueue(VulkanDevice &device, uint32_t queueFamilyIndex, uint32_t queueId);
	VulkanQueue(const VulkanQueue &) = delete;

	void submit(std::vector<VkSubmitInfo> submissions, VkFence fence);
	void present(VkPresentInfoKHR &info);
	void wait();

	inline uint32_t family() const { return queueFamily; };
	inline uint32_t id() const { return queueId; };

private:
	VulkanDevice &device;
	VkQueue queue;
	uint32_t queueFamily;
	uint32_t queueId;
	std::mutex queueGuard;

};