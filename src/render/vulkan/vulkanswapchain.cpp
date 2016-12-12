#include "vulkanswapchain.h"

VulkanSwapchain::~VulkanSwapchain() {
}

void VulkanSwapchain::init(SolisDevice *device, const VulkanContext &context) {
	createSurface(device);
	presentQueueIndex = context.findQueue(vk::QueueFlagBits::eGraphics, surface);
	presentQueue = context.device.getQueue(presentQueueIndex, 0);
	createSwapchain();
}

void VulkanSwapchain::destroy(const VulkanContext &context) {
	for(size_t i = 0; i < images.size(); i++) {
		context.device.destroyImageView(images[i].imageView);
	}

	context.device.destroySwapchainKHR(swapchain);
	context.instance.destroySurfaceKHR(surface);
}

void VulkanSwapchain::present(uint32_t imageIndex, const vk::ArrayProxy<const vk::Semaphore> &waitSemaphores) {
	vk::PresentInfoKHR presentInfo;
	presentInfo.waitSemaphoreCount = waitSemaphores.size();
	presentInfo.pWaitSemaphores = waitSemaphores.data();
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain;
	presentInfo.pImageIndices = &imageIndex;

	presentQueue.presentKHR(presentInfo);
}

void VulkanSwapchain::createSurface(SolisDevice *device, const VulkanContext &context) {
	if(glfwCreateWindowSurface(context.instance, device->getWindow(), nullptr, (VkSurfaceKHR *)&surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");

	swapchainExtent = vk::Extent2D{(uint32_t)device->getWidth() / 2,(uint32_t) device->getHeight() / 2};
}

void VulkanSwapchain::createSwapchain(const VulkanContext &context) {
	if(!context.physicalDevice.getSurfaceSupportKHR(presentQueueIndex, surface))
		throw std::runtime_error("The device does not support the surface");

	//choose surface Format
	std::vector<vk::SurfaceFormatKHR> surfaceFormats = context.physicalDevice.getSurfaceFormatsKHR(surface);
	auto formatCount = surfaceFormats.size();


	if(formatCount == 1 && surfaceFormats[0].format == vk::Format::eUndefined) {
		colorFormat = vk::Format::eB8G8R8A8Unorm;
	} else {
		colorFormat = surfaceFormats[0].format;
	}
	colorSpace = surfaceFormats[0].colorSpace;

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = context.physicalDevice.getSurfaceCapabilitiesKHR(surface);
	std::vector<vk::PresentModeKHR> presentModes = context.physicalDevice.getSurfacePresentModesKHR(surface);
	size_t presentModeCount = presentModes.size();

	//choose Extent
	if(surfaceCapabilities.currentExtent.width != UINT32_MAX) {
		swapchainExtent = surfaceCapabilities.currentExtent;
	}

	//choose Present mode
	vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;
	for(size_t i = 0; i < presentModeCount; i++) {
		if(presentModes[i] == vk::PresentModeKHR::eMailbox) {
			swapchainPresentMode = vk::PresentModeKHR::eMailbox;
			break;
		}
		if(presentModes[i] == vk::PresentModeKHR::eImmediate) {
			swapchainPresentMode = vk::PresentModeKHR::eImmediate;
		}
	}

	uint32_t swapchainImageCount = surfaceCapabilities.minImageCount + 1;
	if(surfaceCapabilities.maxImageCount > 0 && swapchainImageCount > surfaceCapabilities.maxImageCount) {
		swapchainImageCount = surfaceCapabilities.maxImageCount;
	}

	//Creation of the Swapchain
	vk::SwapchainCreateInfoKHR swapchainCreateInfo;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = swapchainImageCount;
	swapchainCreateInfo.imageFormat = colorFormat;
	swapchainCreateInfo.imageColorSpace = colorSpace;
	swapchainCreateInfo.imageExtent = vk::Extent2D{ swapchainExtent.width, swapchainExtent.height };
	swapchainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.presentMode = swapchainPresentMode;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	swapchainCreateInfo.clipped = true;
	swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;

	swapchain = context.device.createSwapchainKHR(swapchainCreateInfo);

	//Create Swapchain Images
	vk::ImageViewCreateInfo colorAttachmentView;
	colorAttachmentView.format = colorFormat;
	colorAttachmentView.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	colorAttachmentView.subresourceRange.levelCount = 1;
	colorAttachmentView.subresourceRange.layerCount = 1;
	colorAttachmentView.viewType = vk::ImageViewType::e2D;

	auto swapchainImages = context.device.getSwapchainImagesKHR(swapchain);
	size_t imageCount = swapchainImages.size();

	images.resize(imageCount);
	for(size_t i = 0; i < imageCount; i++) {
		images[i].image = swapchainImages[i];
		colorAttachmentView.image = swapchainImages[i];
		images[i].imageView = context.device.createImageView(colorAttachmentView);
	}
}