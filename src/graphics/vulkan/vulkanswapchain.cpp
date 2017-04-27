/* EDIT DIS */

#include "vulkanswapchain.h"


VulkanSwapchain::VulkanSwapchain(SolisDevice *solisDevice, 
		std::shared_ptr<VulkanContext> context, 
		std::shared_ptr<VulkanDevice>  vulkanDevice) :
		context(context),
		device(vulkanDevice) {

	surface = std::shared_ptr<VkSurfaceKHR>(new VkSurfaceKHR);
	glfwCreateWindowSurface(context->instance, solisDevice->getWindow(), nullptr, surface.get());

	VkSurfaceCapabilitiesKHR capabilities;
	context->vk.getPhysicalDeviceSurfaceCapabilitiesKHR(device->physicalDevice.device, *surface, &capabilities);

	uint32_t formatCount;
	context->vk.getPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice.device, *surface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	context->vk.getPhysicalDeviceSurfaceFormatsKHR(device->physicalDevice.device, *surface, &formatCount, formats.data());

	imageFormat = formats[0].format;

	uint32_t presentModeCount;
	context->vk.getPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice.device, *surface, &presentModeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	context->vk.getPhysicalDeviceSurfacePresentModesKHR(device->physicalDevice.device, *surface, &presentModeCount, presentModes.data());

	uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    uint32_t familyIndex[] = {0, 2};

    dimensions = {(uint32_t)solisDevice->getWidth(), (uint32_t)solisDevice->getHeight()};

	VkSwapchainCreateInfoKHR sci = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		nullptr,
		0,
		*surface,
		imageCount,
		imageFormat,
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR /* This is the only colorspace aviable */,
		dimensions,
		1,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		/*FOR MY CARD*/
		VK_SHARING_MODE_CONCURRENT,
		2,
		familyIndex,
		///////
		capabilities.currentTransform,
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		presentModes[0],
		true,
		VK_NULL_HANDLE
	};

	uint32_t supported;
	context->vk.getPhysicalDeviceSurfaceSupportKHR(context->physicalDevices[0].device, 0, *surface, &supported);
	context->vk.getPhysicalDeviceSurfaceSupportKHR(context->physicalDevices[0].device, 1, *surface, &supported);
	context->vk.getPhysicalDeviceSurfaceSupportKHR(context->physicalDevices[0].device, 2, *surface, &supported);

	device->vk.createSwapchainKHR(device->device, &sci, nullptr, &swapchain);

	/* GET SWAPCHAIN IMAGES */
	uint32_t swapchainImageCount;
	device->vk.getSwapchainImagesKHR(device->device, swapchain, &swapchainImageCount, nullptr);
    images.resize(swapchainImageCount);
    device->vk.getSwapchainImagesKHR(device->device, swapchain, &swapchainImageCount, images.data());

    /* CREATE IMAGE VIEWS */
    imageViews.resize(images.size());
    for(size_t i = 0; i < imageViews.size(); i++) {
    	VkImageViewCreateInfo ivci = {
    		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    		nullptr,
    		0,
    		images[i],
    		VK_IMAGE_VIEW_TYPE_2D,
    		imageFormat,
    		{ VK_COMPONENT_SWIZZLE_IDENTITY, 
    			VK_COMPONENT_SWIZZLE_IDENTITY, 
    			VK_COMPONENT_SWIZZLE_IDENTITY, 
    			VK_COMPONENT_SWIZZLE_IDENTITY },
    		{ VK_IMAGE_ASPECT_COLOR_BIT,
    			0, 1, 0, 1 }
    	};

    	device->vk.createImageView(device->device, &ivci, nullptr, &imageViews[i]);
    }

}

VulkanSwapchain::~VulkanSwapchain() {
	for(auto &imageView : imageViews) {
		device->vk.destroyImageView(device->device, imageView, nullptr);
	}

	device->vk.destroySwapchainKHR(device->device, swapchain, nullptr);
	context->vk.destroySurfaceKHR(context->instance, *surface, nullptr);
}