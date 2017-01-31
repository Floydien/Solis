#include "vulkandescriptorpool.h"

VulkanDescriptorPool::VulkanDescriptorPool(std::shared_ptr<VulkanDevice> _device, std::vector<VkDescriptorPoolSize> poolSizes, uint32_t maxSets) :
		device(std::move(_device)) {

	VkDescriptorPoolCreateInfo poolInfo  = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        nullptr,
        0,
        maxSets,
        (uint32_t)poolSizes.size(),
        poolSizes.data()
    };

    device->vk.createDescriptorPool(device->device, &poolInfo, nullptr, &descriptorPool);
}

VulkanDescriptorPool::~VulkanDescriptorPool() {
	std::lock_guard<std::mutex> guard(descriptorMutex);

    device->vk.destroyDescriptorPool(device->device, descriptorPool, nullptr);
}


std::vector<VkDescriptorSet> VulkanDescriptorPool::alloc(std::vector<VkDescriptorSetLayout> layouts) {
	std::lock_guard<std::mutex> guard(descriptorMutex);

    VkDescriptorSetAllocateInfo allocInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        nullptr,
        descriptorPool,
        (uint32_t)layouts.size(),
        layouts.data()
    };

    std::vector<VkDescriptorSet> descriptorSets(layouts.size());
    device->vk.allocateDescriptorSets(device->device, &allocInfo, descriptorSets.data());

    return descriptorSets;
}

void VulkanDescriptorPool::free(std::vector<VkDescriptorSet> &sets) {
	std::lock_guard<std::mutex> guard(descriptorMutex);

	device->vk.freeDescriptorSets(device->device, descriptorPool, sets.size(), sets.data());
}