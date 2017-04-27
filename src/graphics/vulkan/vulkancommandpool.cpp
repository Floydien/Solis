#include "vulkancommandpool.h"

VulkanCommandPool::VulkanCommandPool(VulkanDevice &_device, uint32_t _queueFamilyIndex, bool transistent, bool resetCommandBuffer) :
		device(_device),
		queueFamily(_queueFamilyIndex) {

	VkCommandPoolCreateFlags flags;
	if(transistent) {
		flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	}
	if(resetCommandBuffer) {
		flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	}

    VkCommandPoolCreateInfo cpci = {
    	VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    	nullptr,
    	flags,
    	queueFamily
    };
    if(device.vk.createCommandPool(device.device, &cpci, nullptr, &commandPool) != VK_SUCCESS) {
    	printf("ERROR\n");
    }
}

VulkanCommandPool::~VulkanCommandPool() {
	std::lock_guard<std::mutex> guard(cmdPoolMutex);

    device.vk.destroyCommandPool(device.device, commandPool, nullptr);
}

std::vector<VkCommandBuffer> VulkanCommandPool::allocateCommandBuffers(bool secondary, uint32_t count) {
	std::lock_guard<std::mutex> guard(cmdPoolMutex);
	std::vector<VkCommandBuffer> out(count);

	VkCommandBufferLevel level = secondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	VkCommandBufferAllocateInfo cbai = {
    	VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    	nullptr,
    	commandPool,
    	level,
    	count
    };

    device.vk.allocateCommandBuffers(device.device, &cbai, out.data());

	return out;
}
void VulkanCommandPool::freeCommandBuffers(std::vector<VkCommandBuffer> buffers) {
	std::lock_guard<std::mutex> guard(cmdPoolMutex);
    device.vk.freeCommandBuffers(device.device, commandPool, buffers.size(), buffers.data());
}
