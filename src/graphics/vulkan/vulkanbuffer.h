/**
*	TODO: Create a Buffer class for device local and cpu visible buffer
*/

#pragma once
#include "vulkandevice.h"
#include "vulkanqueue.h"
#include <cstring>

class VulkanBuffer {
public:
	VulkanBuffer(std::shared_ptr<VulkanDevice>, VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags);
	~VulkanBuffer();

	void copyFrom(VulkanBuffer &, std::shared_ptr<VulkanQueue>);


	std::shared_ptr<VulkanDevice> device;
	VkBuffer buffer;
	VkDeviceMemory memory;
	VkDeviceSize size;
	VkBufferUsageFlags bufferFlags;
	VkMemoryPropertyFlags memoryFlags;

	template <typename T> 
	void mapContent(const std::vector<T> &content) {
		if(memoryFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
		    void* data;
		    device->vk.mapMemory(device->device, memory, 0, size, 0, &data);
		        memcpy(data, content.data(), size);
		    device->vk.unmapMemory(device->device, memory);
		}
	};
};