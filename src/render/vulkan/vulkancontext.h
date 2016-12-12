#pragma once
#include <vulkan/vulkan.hpp>

class VulkanContext {
public:
	~VulkanContext();

	void initContext(const bool);
	void destroyContext();

	void submit(const vk::ArrayProxy<const vk::CommandBuffer> &,
				const vk::ArrayProxy<const vk::Semaphore> & = {},
				const vk::ArrayProxy<const vk::PipelineStageFlags> & = {},
				const vk::ArrayProxy<const vk::Semaphore> & = {});
	uint32_t findQueue(vk::QueueFlagBits, const vk::SurfaceKHR = vk::SurfaceKHR()) const;
	uint32_t findMemoryType(uint32_t, const vk::MemoryPropertyFlags &) const;

	vk::Instance instance;
	vk::DebugReportCallbackEXT callback;
	vk::PhysicalDevice physicalDevice;
	vk::PhysicalDeviceFeatures deviceFeatures;
	vk::PhysicalDeviceMemoryProperties deviceMemoryProperties;
	vk::Device device;

	vk::Queue graphicsQueue;

private:
	void createInstance();
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	void enableDebugCallback();
	void pickPhysicalDevice();
	void createLogicalDevice();

	uint32_t queueIndex;

	bool enableValidation;

	const std::vector<const char *> VALIDATION_LAYERS = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
};