#include "vulkanrenderpass.h"

VulkanRenderPass::VulkanRenderPass(std::shared_ptr<VulkanDevice> _device, VkFormat format) :
		device(std::move(_device)) {

	std::vector<VkAttachmentDescription> 	attachmentDescriptions;
	std::vector<VkSubpassDescription>		subpasses;
	std::vector<VkSubpassDependency>		dependencies;

	attachmentDescriptions.emplace_back(VkAttachmentDescription {
		0,
		format,
		VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		VK_ATTACHMENT_STORE_OP_DONT_CARE,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	});

	VkAttachmentReference attachmentRef = {
		0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	subpasses.emplace_back(VkSubpassDescription{
		0,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0,
		nullptr,
		1,
		&attachmentRef,
		nullptr,
		nullptr,
		0,
		nullptr
	});
	
	dependencies.emplace_back(VkSubpassDependency{
		VK_SUBPASS_EXTERNAL,
		0,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		0,
		VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
		0
	});


	VkRenderPassCreateInfo rpci = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		nullptr,
		0,
		(uint32_t)attachmentDescriptions.size(),
		attachmentDescriptions.data(),
		(uint32_t)subpasses.size(),
		subpasses.data(),
		(uint32_t)dependencies.size(),
		dependencies.data()
	};

	device->vk.createRenderPass(device->device, &rpci, nullptr, &renderPass);
}

VulkanRenderPass::~VulkanRenderPass() {
	device->vk.destroyRenderPass(device->device, renderPass, nullptr);
}
