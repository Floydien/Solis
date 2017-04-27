#include "vulkandevice.h"
#include "vulkanfunctionloader.h"
#include "vulkancommandpool.h"
#include "vulkanqueue.h"
#include <algorithm>


VulkanDevice::VulkanDevice(std::shared_ptr<VulkanContext> ctx, PhysicalDeviceInfos &physicalDevice, const std::vector<VulkanQueueCreateInfo> &queueCreateInfos) :
		context(std::move(ctx)),
		physicalDevice(physicalDevice) {

	VkPhysicalDeviceFeatures features = {};
	std::vector<const char *> extensions;
	extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	std::vector<VkDeviceQueueCreateInfo> qcis;
	for(auto &qci : queueCreateInfos) {
		qcis.emplace_back(VkDeviceQueueCreateInfo{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			nullptr,
			0,
			qci.queueFamilyIndex,
			qci.queueCount,
			qci.queuePriorities.data()
		});
	}

	VkDeviceCreateInfo dci = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		nullptr,
		0,
		(uint32_t)qcis.size(),
		qcis.data(),
		/*	Device layers are depricated as of 1.0.13.
			They must now be the same as the instance layers for backwards compatibility */
		(uint32_t)context->layers.size(),
		context->layers.data(),
		(uint32_t)extensions.size(),
		extensions.data(),
		&features
	};

	context->vk.createDevice(physicalDevice.device, &dci, nullptr, &device);
	vk = getDeviceFunctions(context->vk, device);

	auto cmdPoolIter = commandPools.before_begin();
	auto queueIter = queues.before_begin();
	for(auto &qci : queueCreateInfos) {
		for(uint32_t i = 0; i < qci.queueCount; i++) {
			queues.emplace_after(queueIter, std::make_shared<VulkanQueue>(*this, qci.queueFamilyIndex, i));
		}

		commandPools.emplace_after(cmdPoolIter, std::make_shared<VulkanCommandPool>(*this, qci.queueFamilyIndex, false, false));
		cmdPoolIter++;
	}

}

/* Finds the queue from the family with the specified id*/
std::shared_ptr<VulkanQueue> VulkanDevice::getQueue(uint32_t family, uint32_t id) const {
	auto it = std::find_if(queues.begin(), queues.end(), [&](const std::shared_ptr<VulkanQueue> &queue) { 
		return queue->family() == family && queue->id() == id; 
	});
	if(it != queues.end()) {
		return *it;
	} else {
		return std::shared_ptr<VulkanQueue>(VK_NULL_HANDLE);
	}
}

std::shared_ptr<VulkanCommandPool> VulkanDevice::getCommandPool(uint32_t family) const {
	auto it = std::find_if(commandPools.begin(), commandPools.end(), [&](std::shared_ptr<VulkanCommandPool> cmdPool) { 
		return cmdPool->family() == family; 
	});
	if(it != commandPools.end()) {
		return (*it);
	} else {
		return std::shared_ptr<VulkanCommandPool>(nullptr);
	}
}

VulkanDevice::~VulkanDevice() {
	for(auto &queue: queues) {
    	queue->wait();
	}
    vk.deviceWaitIdle(device);

    /* Clear command pools before destroying the device */
    while(commandPools.begin() != commandPools.end()) {
    	commandPools.pop_front();
    }

	vk.destroyDevice(device, nullptr);
}