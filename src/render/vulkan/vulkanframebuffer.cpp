#include "vulkanframebuffer.h"


VulkanFramebuffer::VulkanFramebuffer(std::shared_ptr<VulkanRenderPass> renderPass, const std::vector<VkImageView> &_attachments, VkExtent2D _dimensions, uint32_t _layers) :
		device(renderPass->device),
		attachments(_attachments),
		dimensions(_dimensions),
		layers(_layers) {

	VkFramebufferCreateInfo fbci = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		nullptr,
		0,
		renderPass->renderPass,
		(uint32_t)attachments.size(),
		attachments.data(),
		dimensions.width,
		dimensions.height,
		layers
	};

	device->vk.createFramebuffer(device->device, &fbci, nullptr, &framebuffer);
}

VulkanFramebuffer::~VulkanFramebuffer() {
    device->vk.destroyFramebuffer(device->device, framebuffer, nullptr);
}