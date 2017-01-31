#include "vulkanpipelinelayout.h"

VulkanPipelineLayout::VulkanPipelineLayout(std::shared_ptr<VulkanDevice> _device, std::vector<VkDescriptorSetLayout> descriptorLayoutList) :
		device(std::move(_device)),
		descriptorLayouts(std::move(descriptorLayoutList)) {


    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        nullptr,
        0,
        (uint32_t)descriptorLayouts.size(),
        descriptorLayouts.data(),
        0,
        nullptr
    };

    device->vk.createPipelineLayout(device->device, &pipelineLayoutInfo, nullptr, &layout);
}

VulkanPipelineLayout::~VulkanPipelineLayout() {
    device->vk.destroyPipelineLayout(device->device, layout, nullptr);
}