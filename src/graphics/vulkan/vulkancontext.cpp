#include "vulkancontext.h"
#include <string.h>
VulkanContext::VulkanContext() {
	/* Init Instance */
	auto entryf = getEntryFunctions();

	uint32_t layercount = 0;
	entryf.enumerateInstanceLayerProperties(&layercount, nullptr);
	std::vector<VkLayerProperties> layerProperties(layercount);
	entryf.enumerateInstanceLayerProperties(&layercount, layerProperties.data());

	uint32_t extensioncount = 0;
	entryf.enumerateInstanceExtensionProperties(nullptr, &extensioncount, nullptr);
	extensions.resize(extensioncount);
	entryf.enumerateInstanceExtensionProperties(nullptr, &extensioncount, extensions.data());

	for(auto &layerProperty : layerProperties) {
		 // if (!memcmp(layerProperty.layerName, "VK_LAYER_LUNARG_standard_validation", 20) || 
		 // 		// !memcmp(layerProperty.layerName, "VK_LAYER_LUNARG_api_dump", 20) || 
		 // 		!memcmp(layerProperty.layerName, "VK_LAYER_LUNARG_core_validation", 20) ) {
		if (memcmp(layerProperty.layerName, "VK_LAYER_LUNARG_vktrace", 20) &&
			memcmp(layerProperty.layerName, "VK_LAYER_RENDERDOC_Capture", 20) &&
			memcmp(layerProperty.layerName, "VK_LAYER_VALVE_steam_overlay", 20) &&
			memcmp(layerProperty.layerName, "VK_LAYER_LUNARG_api_dump", 20) ) {
			layers.push_back(layerProperty.layerName);
		}
			// printf("%s: %s\n", layerProperty.layerName, layerProperty.description);
	}

	std::vector<const char *> extensionNames;
	for(auto &extension : extensions) {
		extensionNames.emplace_back(extension.extensionName);
		// printf("%s\n", extension.extensionName);
	}

	VkApplicationInfo appinfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		nullptr,
		"Solis",
		0,
		"Solis",
		0,
		VK_API_VERSION_1_0
	};

	VkInstanceCreateInfo ici = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		nullptr,
		0,
		&appinfo,
		(uint32_t)layers.size(),
		layers.data(),
		(uint32_t)extensionNames.size(),
		extensionNames.data()
	};

	entryf.createInstance(&ici, nullptr, &instance);
	vk = getInstanceFunctions(instance);

	/* Get Physical Devices */
	uint32_t devicecount = 0;
	vk.enumeratePhysicalDevices(instance, &devicecount, nullptr);

	std::vector<VkPhysicalDevice> devices(devicecount);
	vk.enumeratePhysicalDevices(instance, &devicecount, devices.data());

	for (auto &device : devices) {
		VkPhysicalDeviceProperties 				devicePorperties;
		std::vector<VkQueueFamilyProperties> 	queueFamilyProperties;
		VkPhysicalDeviceMemoryProperties 		memoryProperties;

		vk.getPhysicalDeviceProperties(device, &devicePorperties);
		
		uint32_t queueFamilyCount = 0;
		vk.getPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
		queueFamilyProperties.resize(queueFamilyCount);
		vk.getPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

		vk.getPhysicalDeviceMemoryProperties(device, &memoryProperties);

		physicalDevices.push_back({
			device,
			devicePorperties,
			queueFamilyProperties,
			memoryProperties,
		});
	}


	/*Debug*/
	VkDebugReportCallbackCreateInfoEXT drcci = {
		VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
		nullptr,
		VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
		[](VkDebugReportFlagsEXT /*flags*/, VkDebugReportObjectTypeEXT /*objType*/, 
				uint64_t /*obj*/, size_t /*location*/, int32_t /*code*/, 
				const char* /*layerPrefix*/, const char* msg, void* /*userData*/) -> VkBool32  {
			printf("Validation Layer: %s\n", msg);
			return VK_FALSE;
		},
		nullptr
	};
	vk.createDebugReportCallbackEXT(instance, &drcci, nullptr, &debugCallback);
}

VulkanContext::~VulkanContext() {
	vk.destroyDebugReportCallbackEXT(instance, debugCallback, nullptr);
	vk.destroyInstance(instance, nullptr);
}