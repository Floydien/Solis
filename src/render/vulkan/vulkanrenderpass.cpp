#include "vulkanrenderpass.h"
#include "vertex.h"
#include <cstddef>

VulkanRenderPass::~VulkanRenderPass() {

}

void VulkanRenderPass::initRenderPass(const VulkanContext &context, const VulkanSwapchain &swapchain) {
	createCommandPool(context, swapchain);
	createRenderPass(context, swapchain);
	createFrameBuffers(context, swapchain);
}

void VulkanRenderPass::createCommandPool(const VulkanContext &context, const VulkanSwapchain &swapchain) {
	vk::CommandPoolCreateInfo cmdPoolCreateInfo;
	cmdPoolCreateInfo.queueFamilyIndex = context.findQueue(vk::QueueFlagBits::eGraphics, swapchain.surface);
	commandPool = context.device.createCommandPool(cmdPoolCreateInfo);
}

void VulkanRenderPass::createRenderPass(const VulkanContext &context, const VulkanSwapchain &swapchain) {
	vk::AttachmentDescription colorAttachment;
	colorAttachment.format = swapchain.colorFormat;
	colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
	colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
	colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
	colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
	colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

	vk::AttachmentReference colorAttachmentReference;
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = vk::ImageLayout::eColorAttachmentOptimal;

	vk::SubpassDescription subpass;
	subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentReference;

	vk::SubpassDependency subpassDependency;
	subpassDependency.srcSubpass = 0;
	subpassDependency.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
	subpassDependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

	subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead;
	subpassDependency.dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;

	vk::RenderPassCreateInfo renderPassCreateInfo;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &colorAttachment;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

	renderPass = context.device.createRenderPass(renderPassCreateInfo);
}

void VulkanRenderPass::createFrameBuffers(const VulkanContext &context, const VulkanSwapchain &swapchain) {
	framebuffers.resize(swapchain.images.size());

	vk::FramebufferCreateInfo framebufferCreateInfo;
	framebufferCreateInfo.renderPass = renderPass;
	framebufferCreateInfo.attachmentCount = 1;
	framebufferCreateInfo.width = swapchain.swapchainExtent.width;
	framebufferCreateInfo.height = swapchain.swapchainExtent.height;
	framebufferCreateInfo.layers = 1;

	for(size_t i = 0; i < swapchain.images.size(); i++) {
		framebufferCreateInfo.pAttachments = &swapchain.images[i].imageView;

		framebuffers[i] = context.device.createFramebuffer(framebufferCreateInfo);
	}
}
/*
void VulkanRenderPass::createGraphicsPipeline(const std::string &filename) {
	auto vertexShaderCode = loadShaderText("res/shaders/vert.spv");
	auto fragmentShaderCode = loadShaderText("res/shaders/tri.frag.spv");

	vk::ShaderModuleCreateInfo shaderCreateInfo;
	std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages;

	// Create Vertex shader
	shaderCreateInfo.codeSize = vertexShaderCode.size();
	shaderCreateInfo.pCode = (uint32_t *) vertexShaderCode.data();
	auto vertexModule = context.device.createShaderModule(shaderCreateInfo);

	shaderStages[0].stage = vk::ShaderStageFlagBits::eVertex;
	shaderStages[0].module = vertexModule;
	shaderStages[0].pName = "main";

	//create Fragment Shader
	shaderCreateInfo.codeSize = fragmentShaderCode.size();
	shaderCreateInfo.pCode = (uint32_t *) fragmentShaderCode.data();
	auto fragmentModule = context.device.createShaderModule(shaderCreateInfo);

	shaderStages[1].stage = vk::ShaderStageFlagBits::eFragment;
	shaderStages[1].module = fragmentModule;
	shaderStages[1].pName = "main";

	//Fixed Pipeline
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState;
	inputAssemblyState.topology = vk::PrimitiveTopology::eTriangleList;

	vk::Viewport viewport;
	viewport.width = swapchain.swapchainExtent.width;
	viewport.height = swapchain.swapchainExtent.height;
	viewport.maxDepth = 1.0f;

	vk::Rect2D scissor;
	scissor.extent = swapchain.swapchainExtent;

	vk::PipelineViewportStateCreateInfo viewportState;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	vk::PipelineRasterizationStateCreateInfo rasterizationState;
	rasterizationState.lineWidth = 1.0f;
	rasterizationState.polygonMode = vk::PolygonMode::eFill;
	rasterizationState.cullMode = vk::CullModeFlagBits::eNone;
	rasterizationState.frontFace = vk::FrontFace::eClockwise;

	vk::PipelineMultisampleStateCreateInfo multisampleState;
	multisampleState.minSampleShading = 1.0f;

	vk::PipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

	vk::PipelineColorBlendStateCreateInfo colorBlendState;
	colorBlendState.logicOp = vk::LogicOp::eCopy;
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = &colorBlendAttachment;

	pipelineLayout = context.device.createPipelineLayout(vk::PipelineLayoutCreateInfo());

	bindingDescriptions.resize(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = vk::VertexInputRate::eVertex;

	attributeDescriptions.resize(2);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = vk::Format::eR32G32Sfloat;
	attributeDescriptions[1].offset = offsetof(Vertex, uvCoord);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = vk::Format::eR32G32B32Sfloat;
	attributeDescriptions[2].offset = offsetof(Vertex, normal);

	vertexInputState.vertexBindingDescriptionCount = (uint32_t) bindingDescriptions.size();
	vertexInputState.pVertexBindingDescriptions = bindingDescriptions.data();
	vertexInputState.vertexAttributeDescriptionCount = (uint32_t) attributeDescriptions.size();
	vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

	vk::GraphicsPipelineCreateInfo pipelineCreateInfo;
	pipelineCreateInfo.stageCount = shaderStages.size();
	pipelineCreateInfo.pStages = shaderStages.data();
	pipelineCreateInfo.pVertexInputState = &vertexInputState;
	pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineCreateInfo.pViewportState = &viewportState;
	pipelineCreateInfo.pRasterizationState = &rasterizationState;
	pipelineCreateInfo.pMultisampleState = &multisampleState;
	pipelineCreateInfo.pColorBlendState = &colorBlendState;
	pipelineCreateInfo.layout = pipelineLayout;
	pipelineCreateInfo.renderPass = renderPass;

	graphicsPipeline = context.device.createGraphicsPipeline(VK_NULL_HANDLE, pipelineCreateInfo);

	context.device.destroyShaderModule(vertexModule);
	context.device.destroyShaderModule(fragmentModule);
}*/

// std::vector<char> VulkanRenderPass::loadShaderText(const std::string& fileName) {
// 	std::ifstream file(fileName, std::ios_base::ate | std::ios_base::binary);
// 	if(!file.is_open()) {
// 		throw std::runtime_error("filed to open file!");
// 	}

// 	size_t filesize = (size_t) file.tellg();
// 	std::vector<char> buffer(filesize);

// 	file.seekg(0);
// 	file.read(buffer.data(), filesize);
// 	file.close();

// 	return buffer;
// }