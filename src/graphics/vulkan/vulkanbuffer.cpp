#include "vulkanbuffer.h"
#include "vulkancommandpool.h"

VulkanBuffer::VulkanBuffer(std::shared_ptr<VulkanDevice> _device, VkDeviceSize _size, VkBufferUsageFlags _bufferFlags, VkMemoryPropertyFlags _memoryFlags) :
		device(std::move(_device)),
		size(_size),
		bufferFlags(_bufferFlags),
		memoryFlags(_memoryFlags) {

    VkBufferCreateInfo bufferInfo = {
        VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        nullptr,
        0,
        size,
        bufferFlags,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        nullptr
    };

    device->vk.createBuffer(device->device, &bufferInfo, nullptr, &buffer);

    VkMemoryRequirements memRequirements;
    device->vk.getBufferMemoryRequirements(device->device, buffer, &memRequirements);

    uint32_t index;
    auto &memoryProperties = device->physicalDevice.memoryProperties;
    for (index = 0; index < memoryProperties.memoryTypeCount; index++) {
        if((memRequirements.memoryTypeBits & ( 1 << index )) && (memoryProperties.memoryTypes[index].propertyFlags & memoryFlags)) {
            break;
        }
    }

    VkMemoryAllocateInfo allocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memRequirements.size,
        index
    };

    device->vk.allocateMemory(device->device, &allocInfo, nullptr, &memory);

    device->vk.bindBufferMemory(device->device, buffer, memory, 0);
}

VulkanBuffer::~VulkanBuffer() {
    device->vk.freeMemory(device->device, memory, nullptr);
    device->vk.destroyBuffer(device->device, buffer, nullptr);

}

void VulkanBuffer::copyFrom(VulkanBuffer &other, std::shared_ptr<VulkanQueue> queue) {
    
    auto buffers = device->getCommandPool(queue->family())->allocateCommandBuffers(false, 1);

    VkCommandBufferBeginInfo cbbi = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        nullptr,
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        nullptr
    };
    device->vk.beginCommandBuffer(buffers[0], &cbbi);

    VkBufferCopy bufferCopy = {
        0,
        0,
        size
    };
    device->vk.cmdCopyBuffer(buffers[0], other.buffer, buffer, 1, &bufferCopy);
    device->vk.endCommandBuffer(buffers[0]);


    VkSubmitInfo submitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr,
        0,
        nullptr,
        0,
        1,
        &buffers[0],
        0,
        nullptr
    }; 

    queue->submit({submitInfo}, VK_NULL_HANDLE);
    queue->wait();

    device->getCommandPool(queue->family())->freeCommandBuffers(buffers);
}
