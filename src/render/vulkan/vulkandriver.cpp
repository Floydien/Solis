#include "vulkandriver.h"


VulkanDriver::VulkanDriver(SolisDevice *device) : 
		VideoDriver(device),
		swapchain(context) {
	context.initContext(true);
	swapchain.init(device);
	
}

VulkanDriver::~VulkanDriver() {

}