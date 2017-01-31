#include "vulkanqueue.h"

VulkanQueue::VulkanQueue(VulkanDevice &_device, uint32_t queueFamilyIndex, uint32_t id) : 
		device(_device),
		queueFamily(queueFamilyIndex),
		queueId(id) {

	device.vk.getDeviceQueue(device.device, queueFamily, queueId, &queue);
}

void VulkanQueue::submit(std::vector<VkSubmitInfo> submissions, VkFence fence) {
	std::lock_guard<std::mutex> guard(queueGuard);
    device.vk.queueSubmit(queue, (uint32_t)submissions.size(), submissions.data(), fence);
}

void VulkanQueue::present(VkPresentInfoKHR &info) {
	std::lock_guard<std::mutex> guard(queueGuard);
    device.vk.queuePresentKHR(queue, &info);
}

void VulkanQueue::wait() {
	std::lock_guard<std::mutex> guard(queueGuard);
    device.vk.queueWaitIdle(queue);
}
