#pragma once
#include <memory>
#include <forward_list>
#include "vulkancontext.h"

class VulkanCommandPool;
class VulkanQueue;

struct VulkanQueueCreateInfo {
	uint32_t queueFamilyIndex;
	uint32_t queueCount;
	std::vector<float> queuePriorities;
};

class VulkanDevice {
public:
	VulkanDevice(std::shared_ptr<VulkanContext> context, PhysicalDeviceInfos &physicalDevice, const std::vector<VulkanQueueCreateInfo> &queueCreateInfos);
	~VulkanDevice();

	std::shared_ptr<VulkanQueue> getQueue(uint32_t family, uint32_t id) const;
	std::shared_ptr<VulkanCommandPool> getCommandPool(uint32_t family) const;

	std::shared_ptr<VulkanContext> context;
	PhysicalDeviceInfos physicalDevice;
	DeviceFuntions vk;
	VkDevice device;
	std::forward_list<std::shared_ptr<VulkanQueue>> queues;
	std::forward_list<std::shared_ptr<VulkanCommandPool>> commandPools;
};