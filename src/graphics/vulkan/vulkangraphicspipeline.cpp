#include "vulkangraphicspipeline.h"
#include "vertex.h"

VulkanGraphicsPipeline::VulkanGraphicsPipeline(std::shared_ptr<VulkanDevice> _device, std::shared_ptr<VulkanShader> shader, VkViewport viewport, VkRect2D scissor, std::shared_ptr<VulkanPipelineLayout> _layout, std::shared_ptr<VulkanRenderPass> _renderPass) :
		device(std::move(_device)),
		renderPass(std::move(_renderPass)),
        layout(std::move(_layout)) {
	
	std::vector<VkPipelineShaderStageCreateInfo> ssci;
    for (auto &_shader : shader->shaders) {
        ssci.emplace_back(VkPipelineShaderStageCreateInfo {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            0,
            _shader.stage,
            _shader.module,
            _shader.entryPoint.c_str(),
            nullptr,
        });
    }

	VkVertexInputBindingDescription bindingDescription = {
		0,
		sizeof(Vertex),
		VK_VERTEX_INPUT_RATE_VERTEX
	};

	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {
        /* Position */
		VkVertexInputAttributeDescription {
        	0,
        	0,
        	VK_FORMAT_R32G32B32_SFLOAT,
        	0
		}, 
        /* UV-Coords */
		VkVertexInputAttributeDescription {
        	1,
        	0,
        	VK_FORMAT_R32G32_SFLOAT,
        	sizeof(float) * 3
		},
        /* Normals */
		VkVertexInputAttributeDescription {
        	2,
        	0,
        	VK_FORMAT_R32G32B32_SFLOAT,
        	sizeof(float) * 5
		},
	};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		nullptr,
		0,
		1,
		&bindingDescription,
		attributeDescriptions.size(),
		attributeDescriptions.data()
	};

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
    	VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    	nullptr,
    	0,
    	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    	VK_FALSE
    };

    VkPipelineViewportStateCreateInfo viewportState = {
    	VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    	nullptr,
    	0,
    	1,
    	&viewport,
    	1,
    	&scissor
    };

    VkPipelineRasterizationStateCreateInfo rasterizer = {
    	VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    	nullptr,
    	0,
    	VK_FALSE,
    	VK_FALSE,
    	VK_POLYGON_MODE_FILL,
    	VK_CULL_MODE_BACK_BIT,
    	VK_FRONT_FACE_CLOCKWISE,
    	VK_FALSE,
    	0.0f,
    	0.0f,
    	0.0f,
    	1.0f
    };

    VkPipelineMultisampleStateCreateInfo multisampling = {
    	VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    	nullptr,
    	0,
    	VK_SAMPLE_COUNT_1_BIT,
    	VK_FALSE,
    	0.0,
    	nullptr,
    	VK_FALSE,
    	VK_FALSE
	};

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {
    	VK_FALSE,
    	VK_BLEND_FACTOR_ZERO,
    	VK_BLEND_FACTOR_ONE,
    	VK_BLEND_OP_ADD,
    	VK_BLEND_FACTOR_ZERO,
    	VK_BLEND_FACTOR_ONE,
    	VK_BLEND_OP_ADD,
    	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };		

    VkPipelineColorBlendStateCreateInfo colorBlending = {
    	VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    	nullptr,
    	0,
    	VK_FALSE,
    	VK_LOGIC_OP_COPY,
    	1,
    	&colorBlendAttachment,
    	{0.0f, 0.0f, 0.0f, 0.0f}
    };

    // VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
    // 	VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    // 	nullptr,
    // 	0,
    // 	0,
    // 	nullptr,
    // 	0,
    // 	nullptr
    // };

    // VkPipelineLayout pipelineLayout;
	// pipelineLayout = std::shared_ptr<VkPipelineLayout>(new VkPipelineLayout);

    // device->vk.createPipelineLayout(device->device, &pipelineLayoutInfo, nullptr, pipelineLayout);

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
    	VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    	nullptr,
    	0,
    	(uint32_t)ssci.size(),
    	ssci.data(),
    	&vertexInputInfo,
    	&inputAssembly,
    	nullptr,
    	&viewportState,
    	&rasterizer,
    	&multisampling,
    	nullptr,
    	&colorBlending,
    	nullptr,
    	layout->layout,
    	renderPass->renderPass,
    	0,
    	VK_NULL_HANDLE,
    	0
    };

    device->vk.createGraphicsPipelines(device->device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline);
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
    device->vk.destroyPipeline(device->device, pipeline, nullptr);
}