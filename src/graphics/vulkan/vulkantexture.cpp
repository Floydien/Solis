#include "vulkantexture.h"
#include "vulkanqueue.h"
#include "vulkancommandpool.h"
#include <cstring>

VulkanTexture::VulkanTexture(std::shared_ptr<VulkanDevice> _device, std::shared_ptr<VulkanQueue> queue, const Image &img) :
		device(std::move(_device)) {

    VkImage stagingImage;
    VkDeviceMemory stagingImageMemory;

    createImage(img, 
        VK_FORMAT_R8G8B8A8_UNORM, 
        VK_IMAGE_TILING_LINEAR, 
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingImage, stagingImageMemory);

    VkImageSubresource subresource = {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        0
    };
    VkSubresourceLayout stagingImageLayout;
    device->vk.getImageSubresourceLayout(device->device, stagingImage, &subresource, &stagingImageLayout);

    void* data;
    device->vk.mapMemory(device->device, stagingImageMemory, 0, img.getData().size(), 0, &data);
    if(stagingImageLayout.rowPitch == (uint32_t)img.getWidth() * 4) {
        memcpy(data, img.getData().data(), img.getData().size());
    } else {
        uint8_t *dataBytes = reinterpret_cast<uint8_t *>(data);

        for(int32_t i = 0; i < img.getHeight(); i++) {
            memcpy(
                &dataBytes[i * stagingImageLayout.rowPitch],
                &img.getData().at(i * img.getWidth() * 4),
                img.getWidth() * 4);
        }
    }
    device->vk.unmapMemory(device->device, stagingImageMemory);

	createImage(img, 
	    VK_FORMAT_R8G8B8A8_UNORM, 
	    VK_IMAGE_TILING_OPTIMAL, 
	    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
	    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
	    image, memory);


    transitionImageLayout(stagingImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, queue);
    transitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, queue);

    copyImage(stagingImage, (uint32_t)img.getWidth(), (uint32_t)img.getHeight(), queue);
    transitionImageLayout(image, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, queue);

    VkImageViewCreateInfo viewImage = {
        VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        nullptr,
        0,
        image,
        VK_IMAGE_VIEW_TYPE_2D,
        VK_FORMAT_R8G8B8A8_UNORM,
        {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        }, {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0,
            1,
            0,
            1
        }
    };

    device->vk.createImageView(device->device, &viewImage, nullptr, &imageView);


    device->vk.destroyImage(device->device, stagingImage, nullptr);
    device->vk.freeMemory(device->device, stagingImageMemory, nullptr);
}

VulkanTexture::~VulkanTexture() {
    device->vk.destroyImageView(device->device, imageView, nullptr);
    device->vk.destroyImage(device->device, image, nullptr);
    device->vk.freeMemory(device->device, memory, nullptr);

}

void VulkanTexture::createImage(const Image &imageData, VkFormat format, VkImageTiling tiling, 
		VkImageUsageFlags usage, VkMemoryPropertyFlags memoryPropertyFlags, 
		VkImage &image, VkDeviceMemory &imageMemory) {

	VkImageCreateInfo imageci = {
        VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        nullptr,
        0,
        VK_IMAGE_TYPE_2D,
        format,
        { (uint32_t)imageData.getWidth(),
            (uint32_t)imageData.getHeight(),
            1 },
        1,
        1,
        VK_SAMPLE_COUNT_1_BIT,
        tiling,
        usage,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        nullptr,
        VK_IMAGE_LAYOUT_PREINITIALIZED
    };

    device->vk.createImage(device->device, &imageci, nullptr, &image);

    VkMemoryRequirements memRequirements;
    device->vk.getImageMemoryRequirements(device->device, image, &memRequirements);

    uint32_t index;
    auto &memoryProperties = device->physicalDevice.memoryProperties;
    for (index = 0; index < memoryProperties.memoryTypeCount; index++) {
        if((memRequirements.memoryTypeBits & ( 1 << index )) && (memoryProperties.memoryTypes[index].propertyFlags & memoryPropertyFlags )) {
            break;
        }
    }

    VkMemoryAllocateInfo mallocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memRequirements.size,
        index
    };
    device->vk.allocateMemory(device->device, &mallocInfo, nullptr, &imageMemory);

    device->vk.bindImageMemory(device->device, image, imageMemory, 0);
}

void VulkanTexture::transitionImageLayout(VkImage image, VkFormat /*format*/, VkImageLayout oldLayout, VkImageLayout newLayout, std::shared_ptr<VulkanQueue> queue) {
	    /* BEGIN COMMANDBUFFER */

    VkCommandBufferBeginInfo cmdBeginInfo = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        nullptr,
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        nullptr
    };

    auto buffers = device->getCommandPool(queue->family())->allocateCommandBuffers(false, 1);
    device->vk.beginCommandBuffer(buffers[0], &cmdBeginInfo);

    /* IMAGE TRANSITION */
    VkAccessFlags srcAccess;
    VkAccessFlags dstAccess;

    if(oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        srcAccess = VK_ACCESS_HOST_WRITE_BIT;
        dstAccess = VK_ACCESS_TRANSFER_READ_BIT;
    } else if(oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        srcAccess = VK_ACCESS_HOST_WRITE_BIT;
        dstAccess = VK_ACCESS_TRANSFER_WRITE_BIT;
    } else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL  && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        srcAccess = VK_ACCESS_TRANSFER_WRITE_BIT;
        dstAccess = VK_ACCESS_SHADER_READ_BIT;
    }

    VkImageMemoryBarrier imageBarrier = {
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr,
        srcAccess,
        dstAccess,
        oldLayout,
        newLayout,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        image,
        {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0,
            1,
            0,
            1
        }
    };

    device->vk.cmdPipelineBarrier(
        buffers[0],
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &imageBarrier);

    /* SUBMIT COMMAND */
    device->vk.endCommandBuffer(buffers[0]);
    VkSubmitInfo submitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr,
        0,
        nullptr,
        nullptr,
        (uint32_t)buffers.size(),
        buffers.data(),
        0,
        nullptr
    };

    queue->submit({submitInfo}, VK_NULL_HANDLE);
    queue->wait();

    device->getCommandPool(queue->family())->freeCommandBuffers(buffers);
}

void VulkanTexture::copyImage(VkImage stagingImage, uint32_t width, uint32_t height, std::shared_ptr<VulkanQueue> queue) {
    
    VkCommandBufferBeginInfo cmdBeginInfo = {
        VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        nullptr,
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        nullptr
    };
    auto buffers = device->getCommandPool(queue->family())->allocateCommandBuffers(false, 1);
    device->vk.beginCommandBuffer(buffers[0], &cmdBeginInfo);


    /* COPY IMAGE*/
    VkImageSubresourceLayers subresource = {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        0,
        1
    };

    VkImageCopy imageCopy = {
        subresource,
        {0, 0, 0,},
        subresource,
        {0, 0, 0,},
        {width, height, 1}
    };

    device->vk.cmdCopyImage(buffers[0],
        stagingImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &imageCopy);


    /* SUBMIT COMMAND */
    device->vk.endCommandBuffer(buffers[0]);
    VkSubmitInfo submitInfo = {
        VK_STRUCTURE_TYPE_SUBMIT_INFO,
        nullptr,
        0,
        nullptr,
        nullptr,
        (uint32_t)buffers.size(),
        buffers.data(),
        0,
        nullptr
    };

    queue->submit({submitInfo}, VK_NULL_HANDLE);
    queue->wait();

    device->getCommandPool(queue->family())->freeCommandBuffers(buffers);
}