#pragma once
#include <vector>
#include "vulkanfunctionloader.h"

struct PhysicalDeviceInfos {
	VkPhysicalDevice 						device;
	VkPhysicalDeviceProperties 				properties;
	std::vector<VkQueueFamilyProperties> 	queueFamilies;
	VkPhysicalDeviceMemoryProperties 		memoryProperties;
};

class VulkanContext {
public:
	VulkanContext();
	~VulkanContext();

	VkInstance 							instance;
	std::vector<PhysicalDeviceInfos>	physicalDevices;
	InstanceFunctions					vk;
	std::vector<VkExtensionProperties> 	extensions;
	std::vector<const char *> 			layers;
	VkDebugReportCallbackEXT			debugCallback;
};