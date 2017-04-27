#pragma once
#include "vulkancontext.h"
#include "vulkandevice.h"
#include "solisdevice.h"

class VulkanSwapchain {
public:
	VulkanSwapchain(SolisDevice *, std::shared_ptr<VulkanContext>, std::shared_ptr<VulkanDevice>);
	~VulkanSwapchain();

	std::shared_ptr<VulkanContext> 	context;
	std::shared_ptr<VulkanDevice>  	device;
	std::shared_ptr<VkSurfaceKHR>	surface;
	VkSwapchainKHR					swapchain;
	std::vector<VkImage>			images;
	std::vector<VkImageView>		imageViews;
	VkFormat						imageFormat;
	VkExtent2D						dimensions;

};