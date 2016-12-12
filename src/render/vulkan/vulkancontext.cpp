#include "vulkancontext.h"
#include <GLFW/glfw3.h>

VulkanContext::~VulkanContext() {
	if(!instance)
		destroyContext();
}

void VulkanContext::submit(const vk::ArrayProxy<const vk::CommandBuffer> &commandBuffers,
				const vk::ArrayProxy<const vk::Semaphore> &waitSemaphores,
				const vk::ArrayProxy<const vk::PipelineStageFlags> &pipelineFlags,
				const vk::ArrayProxy<const vk::Semaphore> &signalSemaphores) {

	vk::SubmitInfo submitInfo;
	submitInfo.waitSemaphoreCount = waitSemaphores.size();
	submitInfo.pWaitSemaphores = waitSemaphores.data();
	submitInfo.pWaitDstStageMask = pipelineFlags.data();
	submitInfo.commandBufferCount = commandBuffers.size();
	submitInfo.pCommandBuffers = commandBuffers.data();
	submitInfo.signalSemaphoreCount = signalSemaphores.size();
	submitInfo.pSignalSemaphores = signalSemaphores.data();

	graphicsQueue.submit(submitInfo, VK_NULL_HANDLE);
}

void VulkanContext::initContext(const bool enableValidation_ = false) {
	enableValidation = enableValidation_;
	createInstance();
	enableDebugCallback();
	pickPhysicalDevice();
	createLogicalDevice();
}

void VulkanContext::destroyContext() {
	graphicsQueue.waitIdle();
	device.waitIdle();

	device.destroy();
	//Destroy Debug Callback
	if(enableValidation) {
	    auto func = (PFN_vkDestroyDebugReportCallbackEXT)
    		vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    	if (func != nullptr) {
        	func(instance, callback, nullptr);
    	}
	}

	instance.destroy();	
}

void VulkanContext::createInstance() {
	auto extensions = getRequiredExtensions();

	vk::ApplicationInfo appinfo;
	appinfo.pApplicationName = "Solis";
	appinfo.pEngineName = "Solis";
	appinfo.apiVersion = VK_API_VERSION_1_0;

	vk::InstanceCreateInfo ici;
	ici.pApplicationInfo = &appinfo;
    ici.enabledExtensionCount = extensions.size();
    ici.ppEnabledExtensionNames = extensions.data();
    if(enableValidation) {
    	ici.enabledLayerCount = VALIDATION_LAYERS.size();
    	ici.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	} else {
		ici.enabledLayerCount = 0;
	}

	instance = vk::createInstance(ici);
}

std::vector<const char*> VulkanContext::getRequiredExtensions() {
	std::vector<const char *> extensions;

	unsigned int glfwExtensionCount = 0;
	const char **glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for(unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}

	if(enableValidation)
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	return extensions;
}

bool VulkanContext::checkValidationLayerSupport() {
	auto layers = vk::enumerateInstanceLayerProperties();

	for(const char *name : VALIDATION_LAYERS) {
		bool layerFound = false;

		for(const auto &layerProperties : layers) {
			if(strcmp(name, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if(!layerFound)
			return false;
	}

	return true;
}

void VulkanContext::enableDebugCallback() {
	if(!enableValidation)
		return;

	//Debug information and function
	vk::DebugReportCallbackCreateInfoEXT cci( vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning,
	(PFN_vkDebugReportCallbackEXT) [](
    	VkDebugReportFlagsEXT		,
    	VkDebugReportObjectTypeEXT  ,
    	uint64_t                    ,
    	size_t                      ,
    	int32_t                     ,
    	const char*                 ,
    	const char*                 pMessage,
    	void*                       ) -> VkBool32 { 
			fprintf(stderr, "Validation layer: %s\n", pMessage);
    		return VK_FALSE;
    });

	auto func = (PFN_vkCreateDebugReportCallbackEXT) 
    	instance.getProcAddr("vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, (VkDebugReportCallbackCreateInfoEXT*)&cci, nullptr, (VkDebugReportCallbackEXT*)&callback);
    } else {
    	printf("Error\n");
    }
}

void VulkanContext::pickPhysicalDevice() {
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	physicalDevice = physicalDevices[0];

	deviceFeatures = physicalDevice.getFeatures();
	deviceMemoryProperties = physicalDevice.getMemoryProperties();
}

void VulkanContext::createLogicalDevice() {
	queueIndex = findQueue(vk::QueueFlagBits::eGraphics);
	std::array<float, 1> queuePriorities = { 0.0f };

	vk::DeviceQueueCreateInfo queueCreateInfo;
	queueCreateInfo.queueFamilyIndex = queueIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = queuePriorities.data();

	vk::DeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = DEVICE_EXTENSIONS.size();
	deviceCreateInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

	if(enableValidation) {
		deviceCreateInfo.enabledLayerCount = VALIDATION_LAYERS.size();
		deviceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}

	device = physicalDevice.createDevice(deviceCreateInfo);
	//one queue for presenting and one for rendering
	graphicsQueue = device.getQueue(queueIndex, 0);
}

uint32_t VulkanContext::findQueue(vk::QueueFlagBits flags, const vk::SurfaceKHR surface) const {
	std::vector<vk::QueueFamilyProperties> queueProps = physicalDevice.getQueueFamilyProperties();
	for(size_t i = 0; i < queueProps.size(); i++) {
		if(queueProps[i].queueFlags & flags) {
			if(surface && !physicalDevice.getSurfaceSupportKHR(i, surface)) {
				continue;
			}
			return i;
		}
	}
	throw std::runtime_error("Failed to find a queue that matches the flags " + vk::to_string(flags));
}

uint32_t VulkanContext::findMemoryType(uint32_t typeBits, const vk::MemoryPropertyFlags &propertyFlags) const {
	for (uint32_t i = 0; i < 32; i++) {
		if((typeBits & 1) == 1) {
			if((deviceMemoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
				return i;
			}
		}
		typeBits >>= 1;
	}
	throw std::runtime_error("Memory Type could not be found");
}