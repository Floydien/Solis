#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include "vulkancontext.h"
#include "solisdevice.h"

typedef struct SwapchainImage {
	vk::Image image;
	vk::ImageView imageView;
} SwapchainImage;

class VulkanSwapchain {
public:	
	// VulkanSwapchain(const VulkanContext &context_) :
		// context(context_) {};
	VulkanSwapchain() {};
	~VulkanSwapchain();

	void init(SolisDevice *, const VulkanContext &);
	void destroy(const VulkanContext &);

	void present(const VulkanContext &, uint32_t, const vk::ArrayProxy<const vk::Semaphore> &);

	vk::SurfaceKHR surface;
	vk::Extent2D swapchainExtent;
	vk::SwapchainKHR swapchain;
	std::vector<SwapchainImage> images;
	vk::Format colorFormat;
	vk::ColorSpaceKHR colorSpace;
	
	vk::Queue presentQueue;
	uint32_t presentQueueIndex;

private:
	// const VulkanContext &context;

	void createSurface(SolisDevice *, const VulkanContext &context);
	void createSwapchain(const VulkanContext &context);

};