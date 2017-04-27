#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <functional>

static std::function<PFN_vkVoidFunction(VkInstance, const char *)> getInstanceProcAddr;

struct EntryFunctions {
	std::function<VkResult(const VkInstanceCreateInfo *, const VkAllocationCallbacks *, VkInstance *)> 
		createInstance;

	std::function<VkResult(const char *, uint32_t *, VkExtensionProperties *)> 
		enumerateInstanceExtensionProperties;
		
	std::function<VkResult(uint32_t *, VkLayerProperties *)> 
		enumerateInstanceLayerProperties;
};

struct InstanceFunctions {
	std::function<void(VkInstance, const VkAllocationCallbacks *)> 				destroyInstance;
	std::function<VkResult(VkInstance, uint32_t *, VkPhysicalDevice *)> 		enumeratePhysicalDevices;
	std::function<VkResult(VkPhysicalDevice, const char*, uint32_t *, VkExtensionProperties *)> enumerateDeviceExtensionProperties;
	std::function<VkResult(VkPhysicalDevice, uint32_t *, VkLayerProperties *)> 	enumerateDeviceLayerProperties;
	std::function<void(VkPhysicalDevice, VkPhysicalDeviceFeatures *)> 			getPhysicalDeviceFeatures;
	std::function<void(VkPhysicalDevice, VkPhysicalDeviceProperties *)> 		getPhysicalDeviceProperties;
	std::function<void(VkPhysicalDevice, VkFormat, VkFormatProperties *)> 		getPhysicalDeviceFormatProperties;
	std::function<VkResult(VkPhysicalDevice, VkFormat, VkImageType, VkImageTiling, VkImageUsageFlags, VkImageCreateFlags, VkImageFormatProperties *)> 	getPhysicalDeviceImageFormatProperties;
	std::function<void(VkPhysicalDevice, VkPhysicalDeviceMemoryProperties *)> 	getPhysicalDeviceMemoryProperties;
	std::function<void(VkPhysicalDevice, uint32_t *, VkQueueFamilyProperties *)>getPhysicalDeviceQueueFamilyProperties;
	std::function<void(VkPhysicalDevice, VkFormat, VkImageType, VkSampleCountFlagBits, VkImageUsageFlags, VkImageTiling, uint32_t *, VkSparseImageFormatProperties *)> 	getPhysicalDeviceSparseImageFormatProperties;
	std::function<VkResult(VkPhysicalDevice, const VkDeviceCreateInfo *, const VkAllocationCallbacks *, VkDevice *)> createDevice;
	std::function<PFN_vkVoidFunction(VkDevice, const char *)> 					getDeviceProcAddr;
	/* Debug Callback EXT*/
	std::function<VkResult(VkInstance, const VkDebugReportCallbackCreateInfoEXT *, const VkAllocationCallbacks*, VkDebugReportCallbackEXT *)> createDebugReportCallbackEXT;
	std::function<void(VkInstance, VkDebugReportCallbackEXT, const VkAllocationCallbacks *)> destroyDebugReportCallbackEXT;
	std::function<void(VkInstance, VkDebugReportFlagsEXT, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char *, const char *)> debugReportMessageEXT;

	/* Display Surface Presentation KHR*/
	//Use GLFW for this

	/* KHR Extensions*/
	std::function<VkResult(VkPhysicalDevice, VkDisplayKHR, const VkDisplayModeCreateInfoKHR *, const VkAllocationCallbacks *, VkDisplayModeKHR *)> createDisplayModeKHR;
	std::function<VkResult(VkInstance instance, const VkDisplaySurfaceCreateInfoKHR *, const VkAllocationCallbacks *, VkSurfaceKHR *)> createDisplayPlaneSurfaceKHR;
	std::function<VkResult(VkPhysicalDevice, VkDisplayKHR, uint32_t *, VkDisplayModePropertiesKHR *)> getDisplayModePropertiesKHR;
	std::function<VkResult(VkPhysicalDevice, VkDisplayModeKHR, uint32_t, VkDisplayPlaneCapabilitiesKHR *)> getDisplayPlaneCapabilitiesKHR;
	std::function<VkResult(VkPhysicalDevice, uint32_t, uint32_t *, VkDisplayKHR *)> getDisplayPlaneSupportedDisplaysKHR;
	std::function<VkResult(VkPhysicalDevice, uint32_t *, VkDisplayPropertiesKHR *)> getPhysicalDeviceDisplayPropertiesKHR;
	std::function<VkResult(VkPhysicalDevice, uint32_t *, VkDisplayPlanePropertiesKHR *)> getPhysicalDeviceDisplayPlanePropertiesKHR;

	std::function<void(VkInstance, VkSurfaceKHR, const VkAllocationCallbacks *)> destroySurfaceKHR;
	std::function<VkResult(VkPhysicalDevice, uint32_t, VkSurfaceKHR, VkBool32 *)> getPhysicalDeviceSurfaceSupportKHR;
	std::function<VkResult(VkPhysicalDevice, VkSurfaceKHR, VkSurfaceCapabilitiesKHR *)> getPhysicalDeviceSurfaceCapabilitiesKHR;
	std::function<VkResult(VkPhysicalDevice, VkSurfaceKHR, uint32_t *, VkSurfaceFormatKHR *)> getPhysicalDeviceSurfaceFormatsKHR;
	std::function<VkResult(VkPhysicalDevice, VkSurfaceKHR, uint32_t *, VkPresentModeKHR *)> getPhysicalDeviceSurfacePresentModesKHR;
};

struct DeviceFuntions {
	std::function<void(VkDevice, const VkAllocationCallbacks *)> 				destroyDevice;
	std::function<void(VkDevice, uint32_t, uint32_t, VkQueue *)> 				getDeviceQueue;
    std::function<VkResult(VkQueue, uint32_t, const VkSubmitInfo *,VkFence)> 	queueSubmit;
    std::function<VkResult(VkQueue)> 											queueWaitIdle;
    std::function<VkResult(VkDevice)> 											deviceWaitIdle;

    /* Memory Functions*/ 
    std::function<VkResult(VkDevice, const VkMemoryAllocateInfo *, const VkAllocationCallbacks *, VkDeviceMemory *)> allocateMemory;
    std::function<void(VkDevice, VkDeviceMemory, const VkAllocationCallbacks *)> freeMemory;
    std::function<VkResult(VkDevice, VkDeviceMemory, VkDeviceSize, VkDeviceSize, VkMemoryMapFlags, void **)> mapMemory;
    std::function<void(VkDevice, VkDeviceMemory)> unmapMemory;
    std::function<VkResult(VkDevice, uint32_t, const VkMappedMemoryRange *)> flushMappedMemoryRanges;
    std::function<VkResult(VkDevice, uint32_t, const VkMappedMemoryRange *)> invalidateMappedMemoryRanges;
    std::function<void(VkDevice, VkDeviceMemory, VkDeviceSize *)> getDeviceMemoryCommitment;
    std::function<VkResult(VkDevice, VkBuffer, VkDeviceMemory, VkDeviceSize)> bindBufferMemory;
    std::function<VkResult(VkDevice, VkImage, VkDeviceMemory, VkDeviceSize)> bindImageMemory;
    std::function<void(VkDevice, VkBuffer, VkMemoryRequirements *)> getBufferMemoryRequirements;
    std::function<void(VkDevice, VkImage, VkMemoryRequirements *)> getImageMemoryRequirements;
    std::function<void(VkDevice, VkImage, uint32_t *, VkSparseImageMemoryRequirements *)> getImageSparseMemoryRequirements;
    
	/* Synchornisation functions*/

    std::function<VkResult(VkDevice, const VkFenceCreateInfo *, const VkAllocationCallbacks *, VkFence *)>  createFence;
    std::function<void(VkDevice, VkFence, const VkAllocationCallbacks *)>                                   destroyFence;
    std::function<VkResult(VkDevice, uint32_t, const VkFence *)>                                            resetFences;
    std::function<VkResult(VkDevice, VkFence)>                                                              getFenceStatus;
    std::function<VkResult(VkDevice, uint32_t, const VkFence *, VkBool32, uint64_t)>                        waitForFences;
	std::function<VkResult(VkDevice, const VkSemaphoreCreateInfo *, const VkAllocationCallbacks *, VkSemaphore *)> createSemaphore;
 	std::function<void(VkDevice, VkSemaphore, const VkAllocationCallbacks *)> 	                            destroySemaphore;

 	/* Buffer Functions */
 	std::function<VkResult(VkDevice, const VkBufferCreateInfo *, const VkAllocationCallbacks *, VkBuffer *)> createBuffer;
    std::function<void(VkDevice, VkBuffer, const VkAllocationCallbacks *)> destroyBuffer;
    std::function<VkResult(VkDevice, const VkBufferViewCreateInfo *, const VkAllocationCallbacks *, VkBufferView *)> createBufferView;
    std::function<void(VkDevice, VkBufferView, const VkAllocationCallbacks *)> destroyBufferView;
   

	/* Image Functions */
	std::function<VkResult(VkDevice device, const VkImageCreateInfo *, const VkAllocationCallbacks *, VkImage *)>	createImage;
    std::function<void(VkDevice, VkImage, const VkAllocationCallbacks *)> 		destroyImage;
    std::function<void(VkDevice, VkImage, const VkImageSubresource *, VkSubresourceLayout *)> getImageSubresourceLayout;
	std::function<VkResult(VkDevice device, const VkImageViewCreateInfo *, const VkAllocationCallbacks *, VkImageView *)> createImageView;
    std::function<void(VkDevice, VkImageView imageView, const VkAllocationCallbacks *)>	destroyImageView;

    /* Pipeline functions*/
	std::function<VkResult(VkDevice, const VkShaderModuleCreateInfo *, const VkAllocationCallbacks *, VkShaderModule *)>	createShaderModule;
    std::function<void(VkDevice, VkShaderModule, const VkAllocationCallbacks *)>	destroyShaderModule;
    std::function<VkResult(VkDevice, const VkPipelineCacheCreateInfo *, const VkAllocationCallbacks *, VkPipelineCache *)>	createPipelineCache;
    std::function<void(VkDevice, VkPipelineCache, const VkAllocationCallbacks *)>	destroyPipelineCache;
    std::function<VkResult(VkDevice, VkPipelineCache, size_t *, void *)>			getPipelineCacheData;
    std::function<VkResult(VkDevice, VkPipelineCache, uint32_t, const VkPipelineCache *)>	mergePipelineCaches;
    std::function<VkResult(VkDevice, VkPipelineCache, uint32_t, const VkGraphicsPipelineCreateInfo *, const VkAllocationCallbacks *, VkPipeline *)>	createGraphicsPipelines;
    std::function<VkResult(VkDevice, VkPipelineCache, uint32_t, const VkComputePipelineCreateInfo *, const VkAllocationCallbacks *, VkPipeline *)>	createComputePipelines;
    std::function<void(VkDevice, VkPipeline, const VkAllocationCallbacks *)>		destroyPipeline;
    std::function<VkResult(VkDevice, const VkPipelineLayoutCreateInfo *, const VkAllocationCallbacks *, VkPipelineLayout *)> createPipelineLayout;
    std::function<void(VkDevice, VkPipelineLayout, const VkAllocationCallbacks *)>	destroyPipelineLayout;

    /* Sampler functions*/
    std::function<VkResult(VkDevice, const VkSamplerCreateInfo *, const VkAllocationCallbacks *, VkSampler *)> createSampler;
    std::function<void(VkDevice, VkSampler, const VkAllocationCallbacks *)> destroySampler;
    
    /* Descriptor Set functions */
    std::function<VkResult(VkDevice, const VkDescriptorSetLayoutCreateInfo *, const VkAllocationCallbacks *, VkDescriptorSetLayout *)> createDescriptorSetLayout;
    std::function<void(VkDevice, VkDescriptorSetLayout, const VkAllocationCallbacks *)> destroyDescriptorSetLayout;
    std::function<VkResult(VkDevice, const VkDescriptorPoolCreateInfo *, const VkAllocationCallbacks *, VkDescriptorPool *)> createDescriptorPool;
    std::function<void(VkDevice, VkDescriptorPool, const VkAllocationCallbacks *)> destroyDescriptorPool;
    std::function<VkResult(VkDevice, VkDescriptorPool, VkDescriptorPoolResetFlags)> resetDescriptorPool;
    std::function<VkResult(VkDevice, const VkDescriptorSetAllocateInfo *, VkDescriptorSet *)> allocateDescriptorSets;
    std::function<VkResult(VkDevice, VkDescriptorPool, uint32_t, const VkDescriptorSet *)> freeDescriptorSets;
    std::function<void(VkDevice, uint32_t, const VkWriteDescriptorSet *, uint32_t, const VkCopyDescriptorSet *)> updateDescriptorSets;

   	/* Framebuffer functions */
	std::function<VkResult(VkDevice, const VkFramebufferCreateInfo *, const VkAllocationCallbacks *, VkFramebuffer *)> createFramebuffer;
	std::function<void(VkDevice, VkFramebuffer, const VkAllocationCallbacks *)> 	destroyFramebuffer;

    /* Renderpass functions*/
	std::function<VkResult(VkDevice, const VkRenderPassCreateInfo *, const VkAllocationCallbacks *, VkRenderPass *)> createRenderPass;
	std::function<void(VkDevice, VkRenderPass, const VkAllocationCallbacks *)> 	destroyRenderPass;
	std::function<void(VkDevice, VkRenderPass, VkExtent2D *)> 	getRenderAreaGranularity;

	/* COMMAND POOL / BUFFER*/
	std::function<VkResult(VkDevice, const VkCommandPoolCreateInfo *, const VkAllocationCallbacks *, VkCommandPool *)>	createCommandPool;
    std::function<void(VkDevice, VkCommandPool, const VkAllocationCallbacks *)>	destroyCommandPool;
    std::function<VkResult(VkDevice, VkCommandPool, VkCommandPoolResetFlags)> resetCommandPool;
    std::function<VkResult(VkDevice, const VkCommandBufferAllocateInfo *, VkCommandBuffer *)> allocateCommandBuffers;
    std::function<void(VkDevice, VkCommandPool, uint32_t, const VkCommandBuffer *)>	freeCommandBuffers;
    std::function<VkResult(VkCommandBuffer, const VkCommandBufferBeginInfo *)> beginCommandBuffer;
    std::function<VkResult(VkCommandBuffer)> endCommandBuffer;
    std::function<VkResult(VkCommandBuffer, VkCommandBufferResetFlags)> resetCommandBuffer;

    /* COMMANDS */
    std::function<void(VkCommandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipeline)> cmdBindPipeline;

    std::function<void(VkCommandBuffer, VkPipelineBindPoint, VkPipelineLayout, uint32_t, uint32_t, const VkDescriptorSet *, uint32_t, const uint32_t *)> cmdBindDescriptorSets;
    std::function<void(VkCommandBuffer, VkBuffer, VkDeviceSize, VkIndexType)> cmdBindIndexBuffer;
    std::function<void(VkCommandBuffer, uint32_t, uint32_t, const VkBuffer *, const VkDeviceSize *)> cmdBindVertexBuffers;
    std::function<void(VkCommandBuffer, uint32_t, uint32_t, uint32_t, uint32_t)> cmdDraw;
    std::function<void(VkCommandBuffer, uint32_t, uint32_t, uint32_t, int32_t, uint32_t)> cmdDrawIndexed;
    std::function<void(VkCommandBuffer, VkBuffer, VkDeviceSize, uint32_t, uint32_t)> cmdDrawIndirect;
    std::function<void(VkCommandBuffer, VkBuffer, VkDeviceSize, uint32_t, uint32_t)> cmdDrawIndexedIndirect;
    std::function<void(VkCommandBuffer, uint32_t, uint32_t, uint32_t)> cmdDispatch;
    std::function<void(VkCommandBuffer, VkBuffer, VkDeviceSize)> cmdDispatchIndirect;
    std::function<void(VkCommandBuffer, VkBuffer, VkBuffer, uint32_t, const VkBufferCopy *)> cmdCopyBuffer;
    std::function<void(VkCommandBuffer, VkImage, VkImageLayout, VkImage, VkImageLayout, uint32_t, const VkImageCopy *)>cmdCopyImage;
    std::function<void(VkCommandBuffer, VkImage, VkImageLayout, VkImage, VkImageLayout, uint32_t, const VkImageBlit *, VkFilter)>cmdBlitImage;
    
    std::function<void(VkCommandBuffer, VkPipelineStageFlags, VkPipelineStageFlags, VkDependencyFlags, uint32_t, const VkMemoryBarrier *, uint32_t, const VkBufferMemoryBarrier *, uint32_t, const VkImageMemoryBarrier *)> cmdPipelineBarrier;

    std::function<void(VkCommandBuffer, const VkRenderPassBeginInfo *, VkSubpassContents)> cmdBeginRenderPass;
    std::function<void(VkCommandBuffer, VkSubpassContents)> cmdNextSubpass;
    std::function<void(VkCommandBuffer)> cmdEndRenderPass;
    std::function<void(VkCommandBuffer, uint32_t, const VkCommandBuffer *)> cmdExecuteCommands;

	/* KHR Extension functions */
	std::function<VkResult(VkDevice, const VkSwapchainCreateInfoKHR *, const VkAllocationCallbacks *, VkSwapchainKHR *)> createSwapchainKHR;
	std::function<VkResult(VkDevice, uint32_t, const VkSwapchainCreateInfoKHR *, const VkAllocationCallbacks *, VkSwapchainKHR *)> createSharedSwapchainsKHR;
	std::function<void(VkDevice, VkSwapchainKHR, const VkAllocationCallbacks *)> 	destroySwapchainKHR;
	std::function<VkResult(VkDevice, VkSwapchainKHR, uint32_t *, VkImage *)> 		getSwapchainImagesKHR;
	std::function<VkResult(VkDevice, VkSwapchainKHR, uint64_t, VkSemaphore, VkFence, uint32_t *)> acquireNextImageKHR;
	std::function<VkResult(VkQueue, const VkPresentInfoKHR *)> 						queuePresentKHR;
};

EntryFunctions getEntryFunctions();
InstanceFunctions getInstanceFunctions(const VkInstance &);
DeviceFuntions getDeviceFunctions(const InstanceFunctions &, const VkDevice &);