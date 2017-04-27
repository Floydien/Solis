#pragma once
#include "vulkanrenderpass.h"
#include "vulkanshader.h"
#include "vulkanpipelinelayout.h"

class VulkanGraphicsPipeline {
public:
	VulkanGraphicsPipeline(std::shared_ptr<VulkanDevice>, std::shared_ptr<VulkanShader>, VkViewport, VkRect2D, std::shared_ptr<VulkanPipelineLayout>, std::shared_ptr<VulkanRenderPass>);
	~VulkanGraphicsPipeline();

	std::shared_ptr<VulkanDevice> 		device;
	std::shared_ptr<VulkanRenderPass>	renderPass;
	VkPipeline 							pipeline;
	// VkPipelineLayout					layout;
	std::shared_ptr<VulkanPipelineLayout> 	layout;

};