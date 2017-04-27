#include "vulkanfunctionloader.h"
#include <glfw/glfw3.h>

#define INSTANCE_PROC_ADDR(i, fn) (PFN_##fn) getInstanceProcAddr(i, #fn)

EntryFunctions getEntryFunctions() {
	EntryFunctions out;
	getInstanceProcAddr =
		(PFN_vkGetInstanceProcAddr) glfwGetInstanceProcAddress(nullptr, "vkGetInstanceProcAddr");


	out.createInstance 							= INSTANCE_PROC_ADDR(nullptr, vkCreateInstance);
	out.enumerateInstanceExtensionProperties 	= INSTANCE_PROC_ADDR(nullptr, vkEnumerateInstanceExtensionProperties);
	out.enumerateInstanceLayerProperties 		= INSTANCE_PROC_ADDR(nullptr, vkEnumerateInstanceLayerProperties);

	return out;
}

InstanceFunctions getInstanceFunctions(const VkInstance &inst) {
	InstanceFunctions out;

	out.destroyInstance 						= INSTANCE_PROC_ADDR(inst, vkDestroyInstance);
	out.enumeratePhysicalDevices				= INSTANCE_PROC_ADDR(inst, vkEnumeratePhysicalDevices);
	out.enumerateDeviceExtensionProperties		= INSTANCE_PROC_ADDR(inst, vkEnumerateDeviceExtensionProperties);
	out.enumerateDeviceLayerProperties			= INSTANCE_PROC_ADDR(inst, vkEnumerateDeviceLayerProperties);
	out.getPhysicalDeviceFeatures 				= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceFeatures);
	out.getPhysicalDeviceProperties				= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceProperties);
	out.getPhysicalDeviceFormatProperties 		= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceFormatProperties);
	out.getPhysicalDeviceImageFormatProperties 	= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceImageFormatProperties);
	out.getPhysicalDeviceMemoryProperties		= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceMemoryProperties);
	out.getPhysicalDeviceQueueFamilyProperties	= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceQueueFamilyProperties);
	out.getPhysicalDeviceSparseImageFormatProperties = INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceSparseImageFormatProperties);
	out.createDevice							= INSTANCE_PROC_ADDR(inst, vkCreateDevice);
	out.getDeviceProcAddr						= INSTANCE_PROC_ADDR(inst, vkGetDeviceProcAddr);

	out.createDebugReportCallbackEXT			= INSTANCE_PROC_ADDR(inst, vkCreateDebugReportCallbackEXT);
	out.destroyDebugReportCallbackEXT			= INSTANCE_PROC_ADDR(inst, vkDestroyDebugReportCallbackEXT);
	out.debugReportMessageEXT					= INSTANCE_PROC_ADDR(inst, vkDebugReportMessageEXT);


	out.createDisplayModeKHR					= INSTANCE_PROC_ADDR(inst, vkCreateDisplayModeKHR);
	out.createDisplayPlaneSurfaceKHR			= INSTANCE_PROC_ADDR(inst, vkCreateDisplayPlaneSurfaceKHR);
	out.getDisplayModePropertiesKHR 			= INSTANCE_PROC_ADDR(inst, vkGetDisplayModePropertiesKHR);
	out.getDisplayPlaneCapabilitiesKHR			= INSTANCE_PROC_ADDR(inst, vkGetDisplayPlaneCapabilitiesKHR);
	out.getDisplayPlaneSupportedDisplaysKHR		= INSTANCE_PROC_ADDR(inst, vkGetDisplayPlaneSupportedDisplaysKHR);
	out.getPhysicalDeviceDisplayPropertiesKHR 	= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceDisplayPropertiesKHR);
	out.getPhysicalDeviceDisplayPlanePropertiesKHR 	= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceDisplayPlanePropertiesKHR);

	out.destroySurfaceKHR						= INSTANCE_PROC_ADDR(inst, vkDestroySurfaceKHR);
	out.getPhysicalDeviceSurfaceSupportKHR		= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceSurfaceSupportKHR);
	out.getPhysicalDeviceSurfaceCapabilitiesKHR	= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	out.getPhysicalDeviceSurfaceFormatsKHR		= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceSurfaceFormatsKHR);
	out.getPhysicalDeviceSurfacePresentModesKHR	= INSTANCE_PROC_ADDR(inst, vkGetPhysicalDeviceSurfacePresentModesKHR);

	return out;
}

DeviceFuntions getDeviceFunctions(const InstanceFunctions &ifun, const VkDevice &device) {
	#define DEVICE_PROC_ADDR(d, fn) (PFN_##fn) ifun.getDeviceProcAddr(d, #fn)

	DeviceFuntions out;
	out.destroyDevice 					= DEVICE_PROC_ADDR(device, vkDestroyDevice);
	out.getDeviceQueue 					= DEVICE_PROC_ADDR(device, vkGetDeviceQueue);
    out.queueSubmit 					= DEVICE_PROC_ADDR(device, vkQueueSubmit);
    out.queueWaitIdle 					= DEVICE_PROC_ADDR(device, vkQueueWaitIdle);
    out.deviceWaitIdle 					= DEVICE_PROC_ADDR(device, vkDeviceWaitIdle);

    /* Memory */
    out.allocateMemory 					= DEVICE_PROC_ADDR(device, vkAllocateMemory);
    out.freeMemory     					= DEVICE_PROC_ADDR(device, vkFreeMemory);
    out.mapMemory    					= DEVICE_PROC_ADDR(device, vkMapMemory);
    out.unmapMemory    					= DEVICE_PROC_ADDR(device, vkUnmapMemory);
    out.flushMappedMemoryRanges    		= DEVICE_PROC_ADDR(device, vkFlushMappedMemoryRanges);
    out.invalidateMappedMemoryRanges    = DEVICE_PROC_ADDR(device, vkInvalidateMappedMemoryRanges);
    out.getDeviceMemoryCommitment   	= DEVICE_PROC_ADDR(device, vkGetDeviceMemoryCommitment);
    out.bindBufferMemory    			= DEVICE_PROC_ADDR(device, vkBindBufferMemory);
    out.bindImageMemory    				= DEVICE_PROC_ADDR(device, vkBindImageMemory);
    out.getBufferMemoryRequirements    	= DEVICE_PROC_ADDR(device, vkGetBufferMemoryRequirements);
    out.getImageMemoryRequirements    	= DEVICE_PROC_ADDR(device, vkGetImageMemoryRequirements);
    out.getImageSparseMemoryRequirements= DEVICE_PROC_ADDR(device, vkGetImageSparseMemoryRequirements);

    /* Sync*/
    out.createFence                     = DEVICE_PROC_ADDR(device, vkCreateFence);
    out.destroyFence                    = DEVICE_PROC_ADDR(device, vkDestroyFence);
    out.resetFences                     = DEVICE_PROC_ADDR(device, vkResetFences);
    out.getFenceStatus                  = DEVICE_PROC_ADDR(device, vkGetFenceStatus);
    out.waitForFences                   = DEVICE_PROC_ADDR(device, vkWaitForFences);
 	out.createSemaphore					= DEVICE_PROC_ADDR(device, vkCreateSemaphore);
    out.destroySemaphore   				= DEVICE_PROC_ADDR(device, vkDestroySemaphore);

    /*Buffer*/
 	out.createBuffer 					= DEVICE_PROC_ADDR(device, vkCreateBuffer);
    out.destroyBuffer    				= DEVICE_PROC_ADDR(device, vkDestroyBuffer);
    out.createBufferView    			= DEVICE_PROC_ADDR(device, vkCreateBufferView);
    out.destroyBufferView    			= DEVICE_PROC_ADDR(device, vkDestroyBufferView);

    /*Image*/
	out.createImage  					= DEVICE_PROC_ADDR(device, vkCreateImage);
    out.destroyImage  					= DEVICE_PROC_ADDR(device, vkDestroyImage);
    out.getImageSubresourceLayout  		= DEVICE_PROC_ADDR(device, vkGetImageSubresourceLayout);
	out.createImageView  				= DEVICE_PROC_ADDR(device, vkCreateImageView);
    out.destroyImageView  				= DEVICE_PROC_ADDR(device, vkDestroyImageView);

	out.createShaderModule  			= DEVICE_PROC_ADDR(device, vkCreateShaderModule);
    out.destroyShaderModule 			= DEVICE_PROC_ADDR(device, vkDestroyShaderModule);
    out.createPipelineCache 			= DEVICE_PROC_ADDR(device, vkCreatePipelineCache);
    out.destroyPipelineCache 			= DEVICE_PROC_ADDR(device, vkDestroyPipelineCache);
    out.getPipelineCacheData 			= DEVICE_PROC_ADDR(device, vkGetPipelineCacheData);
    out.mergePipelineCaches 			= DEVICE_PROC_ADDR(device, vkMergePipelineCaches);
    out.createGraphicsPipelines 		= DEVICE_PROC_ADDR(device, vkCreateGraphicsPipelines);
    out.createComputePipelines 			= DEVICE_PROC_ADDR(device, vkCreateComputePipelines);
    out.destroyPipeline 				= DEVICE_PROC_ADDR(device, vkDestroyPipeline);
    out.createPipelineLayout 			= DEVICE_PROC_ADDR(device, vkCreatePipelineLayout);
    out.destroyPipelineLayout 			= DEVICE_PROC_ADDR(device, vkDestroyPipelineLayout);

    out.createSampler 					= DEVICE_PROC_ADDR(device, vkCreateSampler);
    out.destroySampler    				= DEVICE_PROC_ADDR(device, vkDestroySampler);

    out.createDescriptorSetLayout   	= DEVICE_PROC_ADDR(device, vkCreateDescriptorSetLayout);
    out.destroyDescriptorSetLayout    	= DEVICE_PROC_ADDR(device, vkDestroyDescriptorSetLayout);
    out.createDescriptorPool    		= DEVICE_PROC_ADDR(device, vkCreateDescriptorPool);
    out.destroyDescriptorPool    		= DEVICE_PROC_ADDR(device, vkDestroyDescriptorPool);
    out.resetDescriptorPool    			= DEVICE_PROC_ADDR(device, vkResetDescriptorPool);
    out.allocateDescriptorSets    		= DEVICE_PROC_ADDR(device, vkAllocateDescriptorSets);
    out.freeDescriptorSets    			= DEVICE_PROC_ADDR(device, vkFreeDescriptorSets);
    out.updateDescriptorSets    		= DEVICE_PROC_ADDR(device, vkUpdateDescriptorSets);

	out.createFramebuffer 				= DEVICE_PROC_ADDR(device, vkCreateFramebuffer);
	out.destroyFramebuffer				= DEVICE_PROC_ADDR(device, vkDestroyFramebuffer);

	out.createRenderPass 				= DEVICE_PROC_ADDR(device, vkCreateRenderPass);
    out.destroyRenderPass 				= DEVICE_PROC_ADDR(device, vkDestroyRenderPass);
	out.getRenderAreaGranularity		= DEVICE_PROC_ADDR(device, vkGetRenderAreaGranularity);

	out.createCommandPool				= DEVICE_PROC_ADDR(device, vkCreateCommandPool);
    out.destroyCommandPool    			= DEVICE_PROC_ADDR(device, vkDestroyCommandPool);
    out.resetCommandPool     			= DEVICE_PROC_ADDR(device, vkResetCommandPool);
    out.allocateCommandBuffers    		= DEVICE_PROC_ADDR(device, vkAllocateCommandBuffers);
    out.freeCommandBuffers     			= DEVICE_PROC_ADDR(device, vkFreeCommandBuffers);
    out.beginCommandBuffer				= DEVICE_PROC_ADDR(device, vkBeginCommandBuffer);
    out.endCommandBuffer     			= DEVICE_PROC_ADDR(device, vkEndCommandBuffer);
    out.resetCommandBuffer    			= DEVICE_PROC_ADDR(device, vkResetCommandBuffer);

    out.cmdBindPipeline					= DEVICE_PROC_ADDR(device, vkCmdBindPipeline);

    out.cmdBindDescriptorSets 			= DEVICE_PROC_ADDR(device, vkCmdBindDescriptorSets);
    out.cmdBindIndexBuffer 				= DEVICE_PROC_ADDR(device, vkCmdBindIndexBuffer);
    out.cmdBindVertexBuffers    		= DEVICE_PROC_ADDR(device, vkCmdBindVertexBuffers);
    out.cmdDraw    						= DEVICE_PROC_ADDR(device, vkCmdDraw);
    out.cmdDrawIndexed    				= DEVICE_PROC_ADDR(device, vkCmdDrawIndexed);
    out.cmdDrawIndirect 				= DEVICE_PROC_ADDR(device, vkCmdDrawIndirect);
    out.cmdDrawIndexedIndirect 			= DEVICE_PROC_ADDR(device, vkCmdDrawIndexedIndirect);
    out.cmdDispatch    					= DEVICE_PROC_ADDR(device, vkCmdDispatch);
    out.cmdDispatchIndirect    			= DEVICE_PROC_ADDR(device, vkCmdDispatchIndirect);
    out.cmdCopyBuffer    				= DEVICE_PROC_ADDR(device, vkCmdCopyBuffer);
    out.cmdCopyImage    				= DEVICE_PROC_ADDR(device, vkCmdCopyImage);
    out.cmdBlitImage    				= DEVICE_PROC_ADDR(device, vkCmdBlitImage);

	out.cmdPipelineBarrier				= DEVICE_PROC_ADDR(device, vkCmdPipelineBarrier);

    out.cmdBeginRenderPass   			= DEVICE_PROC_ADDR(device, vkCmdBeginRenderPass);
    out.cmdNextSubpass    				= DEVICE_PROC_ADDR(device, vkCmdNextSubpass);
    out.cmdEndRenderPass    			= DEVICE_PROC_ADDR(device, vkCmdEndRenderPass);
    out.cmdExecuteCommands    			= DEVICE_PROC_ADDR(device, vkCmdExecuteCommands);

	out.createSwapchainKHR 				= DEVICE_PROC_ADDR(device, vkCreateSwapchainKHR);
	out.createSharedSwapchainsKHR 		= DEVICE_PROC_ADDR(device, vkCreateSharedSwapchainsKHR);
	out.destroySwapchainKHR				= DEVICE_PROC_ADDR(device, vkDestroySwapchainKHR);
	out.getSwapchainImagesKHR			= DEVICE_PROC_ADDR(device, vkGetSwapchainImagesKHR);
	out.acquireNextImageKHR				= DEVICE_PROC_ADDR(device, vkAcquireNextImageKHR);
	out.queuePresentKHR					= DEVICE_PROC_ADDR(device, vkQueuePresentKHR);

	return out;
}