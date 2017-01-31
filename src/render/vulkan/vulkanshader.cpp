#include "vulkanshader.h"
#include <iostream>
#include <fstream>


VulkanShader::VulkanShader(std::shared_ptr<VulkanDevice> _device, const std::string &shaderName) :
		device(std::move(_device)) {

	auto vsData = loadShader(shaderName + "_vert.spv");
	auto fsData = loadShader(shaderName + "_frag.spv");

	VkShaderModuleCreateInfo vsmci = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		nullptr,
		0,
		vsData.size(),
		(uint32_t *)vsData.data()
	};

	VkShaderModuleCreateInfo fsmci = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		nullptr,
		0,
		fsData.size(),
		(uint32_t *)fsData.data()
	};

	shaders.emplace_back(ShaderModule{
		VK_NULL_HANDLE,
		"main",
		VK_SHADER_STAGE_VERTEX_BIT
	});

	shaders.emplace_back(ShaderModule{
		VK_NULL_HANDLE,
		"main",
		VK_SHADER_STAGE_FRAGMENT_BIT
	});

	device->vk.createShaderModule(device->device, &vsmci, nullptr, &shaders[0].module);
	device->vk.createShaderModule(device->device, &fsmci, nullptr, &shaders[1].module);
}

VulkanShader::~VulkanShader() {
	for (auto &shader : shaders) {
		device->vk.destroyShaderModule(device->device, shader.module, nullptr);
	}
}

std::vector<char> VulkanShader::loadShader(const std::string &file) {
    std::ifstream shaderFile;
    shaderFile.open(file, std::ios::ate | std::ios::binary);
    std::vector<char> buffer;

    if(shaderFile.is_open()) {
    	size_t fileSize = shaderFile.tellg();
    	buffer.resize(fileSize);
    	shaderFile.seekg(0);
    	shaderFile.read(buffer.data(), fileSize);
    	shaderFile.close();
    } else {
        fprintf(stderr, "ERROR: failed to load the shader %s\n", file.c_str());
    }

    return buffer;
}