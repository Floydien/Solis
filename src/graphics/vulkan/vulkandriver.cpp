#include "camera.h"
#include "vulkandriver.h"
#include "vulkancontext.h"
#include "vulkandevice.h"
#include "vulkanswapchain.h"
#include "vulkanrenderpass.h"
#include "vulkanshader.h"
#include "vulkangraphicspipeline.h"
#include "vulkanframebuffer.h"
#include "vulkancommandbufferbuilder.h"
#include "vulkantexture.h"
#include "vulkanqueue.h"
#include "vulkandescriptorpool.h"
#include "image.h"
#include <glm/gtx/transform.hpp>
#include "mesh.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};


VulkanDriver::VulkanDriver(SolisDevice *solisDevice) : VideoDriver(solisDevice) {

    std::shared_ptr<VulkanContext> context;
    std::shared_ptr<VulkanDevice> device;

    std::shared_ptr<VulkanQueue> graphicsQueue;
    std::shared_ptr<VulkanQueue> presentQueue;
    std::shared_ptr<VulkanQueue> transferQueue;

    std::shared_ptr<VulkanSwapchain> swapchain;
    std::shared_ptr<VulkanRenderPass> renderPass;
    std::shared_ptr<VulkanShader> shader;

    std::shared_ptr<VulkanDescriptorPool> descriptorPool;
    std::shared_ptr<VulkanPipelineLayout> pipelineLayout;
    std::shared_ptr<VulkanGraphicsPipeline> graphicsPipeline;

    Camera camera(70.0f, (float)solisDevice->getWidth()/(float)solisDevice->getHeight(), 0.1f, 1000.0f, glm::vec3());
    camera.setSpeed(0.4f);

	context = std::make_shared<VulkanContext>();

    /* TODO: automate this */
    std::vector<VulkanQueueCreateInfo> queues = {
        VulkanQueueCreateInfo{0, 1, {1.0f}}, /* Graphics Queue */
        VulkanQueueCreateInfo{1, 2, {0.9f, 0.8f}}, /* Present / Transfer Queues */
        VulkanQueueCreateInfo{2, 1, {0.8f}}  /* Transfer Queue */
    };

    /* Create logical device with the first graphics card found */
	device = std::make_shared<VulkanDevice>(context, context->physicalDevices[0], queues);

    graphicsQueue = device->getQueue(0, 0);
    presentQueue  = device->getQueue(1, 0);
    transferQueue = device->getQueue(1, 1);

	swapchain = std::make_shared<VulkanSwapchain>(solisDevice, context, device);

	renderPass = std::make_shared<VulkanRenderPass>(device, swapchain->imageFormat);
    shader = std::make_shared<VulkanShader>(device,  "res/shaders/tex");

    /* CREATE VERTEX BUFFER */
    std::vector<Vertex> vertices;
    vertices.emplace_back(glm::vec3(0.0, -0.5, -1.0), glm::vec2(1.0, 0.0));
    vertices.emplace_back(glm::vec3(0.5,  0.5, -1.0), glm::vec2(0.0, 1.0));
    vertices.emplace_back(glm::vec3(-0.5, 0.5, -1.0), glm::vec2(0.2, 0.5));
    std::vector<uint32_t> indices = { 0, 1, 2 };

    Mesh rarity("rarity_all-in-one.obj");
    VkDeviceSize verticesSize = rarity.getVertexBuffer()->getVertices().size() * sizeof(Vertex);
    VulkanBuffer vertexStaging(device, verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vertexStaging.mapContent(rarity.getVertexBuffer()->getVertices());

    auto vertexBufferLink = std::make_unique<VulkanBuffer>(device, verticesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vertexBufferLink->copyFrom(vertexStaging, transferQueue);


    VkDeviceSize indicesSize = rarity.getVertexBuffer()->getVertexIndices().size() * sizeof(uint32_t);
    VulkanBuffer indexStaging(device, indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    indexStaging.mapContent(rarity.getVertexBuffer()->getVertexIndices());

    auto indexBufferLink = std::make_unique<VulkanBuffer>(device, verticesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    indexBufferLink->copyFrom(indexStaging, transferQueue);

    /* CREATE TEXTURE */
    Image img("solis.png");
    VulkanTexture texture(device, transferQueue, img);


    /* CREATE TEXTURESAMPLER */
    VkSamplerCreateInfo samplerInfo = {
        VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        nullptr,
        0,
        VK_FILTER_LINEAR, 
        VK_FILTER_LINEAR,
        VK_SAMPLER_MIPMAP_MODE_LINEAR,
        VK_SAMPLER_ADDRESS_MODE_REPEAT,
        VK_SAMPLER_ADDRESS_MODE_REPEAT,
        VK_SAMPLER_ADDRESS_MODE_REPEAT,
        0.0f,
        VK_TRUE,
        16.0f,
        VK_FALSE,
        VK_COMPARE_OP_ALWAYS,
        0.0f, 0.0f,
        VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        VK_FALSE
    };

    VkSampler textureSampler;
    device->vk.createSampler(device->device, &samplerInfo, nullptr, &textureSampler);

    /* DESCRIPTOR SET */
    std::vector<VkDescriptorSetLayoutBinding> descriptorSetLayoutBindings;

    /* UNIFORM BUFFER LAYOUT*/
    descriptorSetLayoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
        0,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        1,
        VK_SHADER_STAGE_VERTEX_BIT,
        nullptr

    });

    /* SAMPLER LAYOUT*/
    descriptorSetLayoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
        1,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        1,
        VK_SHADER_STAGE_FRAGMENT_BIT,
        nullptr

    });

    VkDescriptorSetLayoutCreateInfo layoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        nullptr,
        0,
        (uint32_t)descriptorSetLayoutBindings.size(),
        descriptorSetLayoutBindings.data()
    };

    
    // VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;

    device->vk.createDescriptorSetLayout(device->device, &layoutInfo, nullptr, &descriptorSetLayout);

    VkDeviceSize uniformBufferSize = sizeof(UniformBufferObject);
    VulkanBuffer uniformStagingBuffer(device, uniformBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    VulkanBuffer uniformBuffer(device, uniformBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    UniformBufferObject ubo;
    ubo.view = glm::mat4();
    ubo.proj = glm::perspective(glm::radians(70.0f), (float)1280/720, 0.1f, 100.0f);

    ubo.model = glm::rotate(glm::radians(90.0f), glm::vec3(0, 0, 1));

    std::vector<UniformBufferObject> vubo = {ubo};
    uniformStagingBuffer.mapContent(vubo); 
    uniformBuffer.copyFrom(uniformStagingBuffer, transferQueue);

    /* DESCRIPTOR POOL */
    std::vector<VkDescriptorPoolSize> poolSizes;
    /* UNIFORM BUFFER */
    poolSizes.emplace_back(VkDescriptorPoolSize {
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        1
    });
    /* SAMPLER */
    poolSizes.emplace_back(VkDescriptorPoolSize {
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        1
    });

    descriptorPool = std::make_shared<VulkanDescriptorPool>(device, poolSizes, 1);
    auto descriptorSets = descriptorPool->alloc({descriptorSetLayout});


    VkDescriptorBufferInfo bufferDscInfo = {
        uniformBuffer.buffer,
        0,
        sizeof(UniformBufferObject)
    };

    VkDescriptorImageInfo imageDscInfo = {
        textureSampler,
        texture.imageView,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    std::vector<VkWriteDescriptorSet> descriptorWrites;
    /* UNIFORM BUFFER */
    descriptorWrites.emplace_back(VkWriteDescriptorSet {
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        nullptr,
        descriptorSets[0],
        0,
        0,
        1,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        nullptr,
        &bufferDscInfo,
        nullptr

    });
    /* SAMPLER */
    descriptorWrites.emplace_back(VkWriteDescriptorSet {
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        nullptr,
        descriptorSets[0],
        1,
        0,
        1,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        &imageDscInfo,
        nullptr,
        nullptr

    });

    device->vk.updateDescriptorSets(device->device, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);


    /* PIPELINE */
    std::vector<VkDescriptorSetLayout> layouts = {descriptorSetLayout};
    pipelineLayout = std::make_shared<VulkanPipelineLayout>(device, layouts);

    graphicsPipeline = std::shared_ptr<VulkanGraphicsPipeline>(new VulkanGraphicsPipeline( 
        device, 
        shader, 
        { 0.0f, 0.0f,
            (float) swapchain->dimensions.width,
            (float) swapchain->dimensions.height,
            0.0f, 1.0f},
        { {0, 0}, swapchain->dimensions}, 
        pipelineLayout,
        renderPass));


    std::vector<std::shared_ptr<VulkanFramebuffer>> framebuffers;
    for(auto image : swapchain->imageViews) {
        std::vector<VkImageView> attachments = { image };
        framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(renderPass, attachments, swapchain->dimensions, 1));
    }


    /* CREATE COMMAND BUFFER */
    std::vector<std::unique_ptr<VulkanCommandBufferBuilder>> commands;
    for(size_t i = 0; i < swapchain->imageViews.size(); i++) {

        VkClearValue clearValue = {0.0f, 0.0f, 0.0f, 1.0f};

        commands.emplace_back(std::make_unique<VulkanCommandBufferBuilder>(device, VK_COMMAND_BUFFER_LEVEL_PRIMARY, graphicsQueue->family()));
        commands.back()->
            drawInline(renderPass, framebuffers[i], clearValue)->
            bindGfxPipeline(graphicsPipeline)->
            drawIndexed(vertexBufferLink, indexBufferLink, rarity.getVertexBuffer()->getVertexIndices().size(), descriptorSets[0])->
            drawEnd()->
            build();
    }

    /* CREATE SEMAPHORES */
    VkSemaphoreCreateInfo sci = {
    	VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    	nullptr,
    	0
    };
    std::vector<VkSemaphore> semaphores(2);

    device->vk.createSemaphore(device->device, &sci, nullptr, &semaphores[0]);
    device->vk.createSemaphore(device->device, &sci, nullptr, &semaphores[1]);

    /* CREATE FENCE */
    VkFenceCreateInfo fci = {
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        nullptr,
        0
    };
    VkFence fence;
    device->vk.createFence(device->device, &fci, nullptr, &fence);

    /* DRAW */
    auto draw = [&]() {
        uint32_t currentImageIndex;
        device->vk.acquireNextImageKHR(device->device, swapchain->swapchain, 
            std::numeric_limits<uint64_t>::max(), semaphores[0], VK_NULL_HANDLE, &currentImageIndex);


    	VkPipelineStageFlags pipelineStages[] =  {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    	VkSubmitInfo submit = {
    		VK_STRUCTURE_TYPE_SUBMIT_INFO,
    		nullptr,
    		1,
    		&semaphores[0],
    		pipelineStages,
    		1,
    		&commands[currentImageIndex]->commandBuffer,
    		1,
    		&semaphores[1]
    	};

        graphicsQueue->submit({submit}, fence);


    	VkPresentInfoKHR presentInfo = {
    		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    		nullptr,
    		1,
    		&semaphores[1],
    		1,
    		&swapchain->swapchain,
    		&currentImageIndex,
    		nullptr
    	};

        presentQueue->present(presentInfo);
        presentQueue->wait();
        device->vk.waitForFences(device->device, 1, &fence, true, std::numeric_limits<uint64_t>::max());
        device->vk.resetFences(device->device, 1, &fence);
    };

    float i = 0.0f;
    while (solisDevice->run()) { 
        i += 0.01;

        camera.input(1.0f/3600.0f * 60.0f, solisDevice);
        vubo[0].view = camera.getView();
        vubo[0].model = glm::rotate(glm::radians(i), glm::vec3(0, 0, 1));


        uniformStagingBuffer.mapContent(vubo);
        uniformBuffer.copyFrom(uniformStagingBuffer, transferQueue);
    	draw();
    }

    graphicsQueue->wait();
    device->vk.deviceWaitIdle(device->device);

    device->vk.destroySampler(device->device, textureSampler, nullptr);

    /* DESTROY SEMAPHORES*/
    device->vk.destroySemaphore(device->device, semaphores[0], nullptr);
    device->vk.destroySemaphore(device->device, semaphores[1], nullptr);

    device->vk.destroyDescriptorSetLayout(device->device, descriptorSetLayout, nullptr);
    device->vk.destroyFence(device->device, fence, nullptr);
}

VulkanDriver::~VulkanDriver() {
}


VulkanDriver::VBLink *VulkanDriver::getBufferLink(std::shared_ptr<const VertexBuffer>) const    { 
    return nullptr; 
};

VulkanDriver::VBLink *VulkanDriver::createBuffer(std::shared_ptr<const VertexBuffer> vbuffer) {     
    if(!vbuffer) {
        return nullptr;
    }
    return nullptr;
    // auto it = vertexBufferMap.find(vbuffer);
    // if(it != vertexBufferMap.end()) {
    //     return it->second;
    // }
    
    // VkDeviceSize verticesSize = vbuffer->getVertices().size() * sizeof(Vertex);
    // VulkanBuffer vertexStaging(device, verticesSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    // vertexStaging.mapContent(vbuffer->getVertices());

    // auto vertexBufferLink = std::make_unique<VulkanBuffer>(device, verticesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    // vertexBufferLink->copyFrom(vertexStaging, transferQueue);


    // VkDeviceSize indicesSize = vbuffer->getVertexIndices().size() * sizeof(uint32_t);
    // VulkanBuffer indexStaging(device, indicesSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    // indexStaging.mapContent(vbuffer->getVertexIndices());

    // auto indexBufferLink = std::make_unique<VulkanBuffer>(device, verticesSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    // indexBufferLink->copyFrom(indexStaging, transferQueue);

    // VBLinkVk *vblink = new VBLinkVk(std::move(vertexBufferLink), std::move(indexBufferLink), vbuffer);

    // vertexBufferMap.insert(std::pair<std::shared_ptr<const VertexBuffer>, VBLink *>(vbuffer, vblink));

    // return vblink;
};