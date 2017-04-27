#pragma once
#include "vulkandevice.h"
#include "vulkanrenderpass.h"
#include "vulkanframebuffer.h"
#include "vulkangraphicspipeline.h"
#include "vulkandriver.h"


class VulkanCommandBufferBuilder {
public:
	VulkanCommandBufferBuilder(std::shared_ptr<VulkanDevice>, VkCommandBufferLevel, uint32_t);
	~VulkanCommandBufferBuilder();

	VulkanCommandBufferBuilder *drawInline(std::shared_ptr<VulkanRenderPass> , std::shared_ptr<VulkanFramebuffer>, VkClearValue);
	VulkanCommandBufferBuilder *bindGfxPipeline(std::shared_ptr<VulkanGraphicsPipeline>);
	VulkanCommandBufferBuilder *drawIndexed(std::unique_ptr<VulkanBuffer> &, std::unique_ptr<VulkanBuffer> &, uint32_t, VkDescriptorSet);
	VulkanCommandBufferBuilder *drawEnd();
	void build();


	std::shared_ptr<VulkanDevice> device;
	VkCommandBufferLevel commandBufferLevel;
	VkCommandBuffer commandBuffer;
	uint32_t queueFamily;

	/* The currently bound graphics pipeline */
	std::shared_ptr<VulkanGraphicsPipeline> currentGraphicsPipeline;
};