#pragma once
#include "vulkancontext.h"
#include "vulkanswapchain.h"

class VulkanRenderPass {
public:
	VulkanRenderPass() {};
	~VulkanRenderPass();

	void initRenderPass(const VulkanContext &, const VulkanSwapchain &);

	void createGraphicsPipeline(const std::string &);

private:

	void createCommandPool(const VulkanContext &, const VulkanSwapchain &);
	void createRenderPass(const VulkanContext &, const VulkanSwapchain &);
	void createFrameBuffers(const VulkanContext &, const VulkanSwapchain &);

	std::vector<char> loadShaderText(const std::string &);


	vk::RenderPass renderPass;
	std::vector<vk::Framebuffer> framebuffers;
	vk::CommandPool commandPool;
	std::vector<vk::CommandBuffer> commandBuffers;
	
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline graphicsPipeline;
};