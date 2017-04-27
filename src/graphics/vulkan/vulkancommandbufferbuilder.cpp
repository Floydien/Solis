#include "vulkancommandbufferbuilder.h"
#include "vulkancommandpool.h"

VulkanCommandBufferBuilder::VulkanCommandBufferBuilder(std::shared_ptr<VulkanDevice> _device, VkCommandBufferLevel level, uint32_t familyIndex) :
		device(std::move(_device)),
        commandBufferLevel(level),
        queueFamily(familyIndex) {


    auto buffers = device->getCommandPool(familyIndex)->allocateCommandBuffers(false, 1);
    commandBuffer = buffers[0];

	VkCommandBufferBeginInfo cmdBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		nullptr,
		VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		nullptr
	};

	device->vk.beginCommandBuffer(commandBuffer, &cmdBeginInfo);
}

VulkanCommandBufferBuilder::~VulkanCommandBufferBuilder() {
    device->getCommandPool(queueFamily)->freeCommandBuffers({commandBuffer});
}

VulkanCommandBufferBuilder *VulkanCommandBufferBuilder::drawInline(std::shared_ptr<VulkanRenderPass> renderPass, 
        std::shared_ptr<VulkanFramebuffer> framebuffer, VkClearValue clearValue){

    VkRenderPassBeginInfo renderPassBegin = {
        VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        nullptr,
        renderPass->renderPass,
        framebuffer->framebuffer,
        {{0, 0}, framebuffer->dimensions},
        1,
        &clearValue
    };

    device->vk.cmdBeginRenderPass(commandBuffer, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

    return this;
}

VulkanCommandBufferBuilder *VulkanCommandBufferBuilder::bindGfxPipeline(std::shared_ptr<VulkanGraphicsPipeline> gfxPipeline) {
    if(gfxPipeline) {
        currentGraphicsPipeline = gfxPipeline;
        device->vk.cmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, currentGraphicsPipeline->pipeline);
    }
    return this;
};


VulkanCommandBufferBuilder *VulkanCommandBufferBuilder::drawIndexed(std::unique_ptr<VulkanBuffer> &vertexBuffer, std::unique_ptr<VulkanBuffer> &indexBuffer, uint32_t count, VkDescriptorSet descriptorSet) {
    if(currentGraphicsPipeline) {
        VkDeviceSize offset[] = {0};
        device->vk.cmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer->buffer, offset);
        device->vk.cmdBindIndexBuffer(commandBuffer, indexBuffer->buffer, 0, VK_INDEX_TYPE_UINT32);
        device->vk.cmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, currentGraphicsPipeline->layout->layout, 0, 1, &descriptorSet, 0, nullptr);
        device->vk.cmdDrawIndexed(commandBuffer, count, 1, 0, 0, 0);
    }
    return this;
}

VulkanCommandBufferBuilder *VulkanCommandBufferBuilder::drawEnd() {
    device->vk.cmdEndRenderPass(commandBuffer);
    return this;
}

void VulkanCommandBufferBuilder::build() {
        device->vk.endCommandBuffer(commandBuffer);
}