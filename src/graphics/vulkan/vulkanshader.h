#pragma once
#include "vulkandevice.h"
#include "shader.h"

class VulkanShader : public Shader {
	struct ShaderModule {
		VkShaderModule module;
		std::string entryPoint;
		VkShaderStageFlagBits stage;
	};
public:
	VulkanShader(std::shared_ptr<VulkanDevice>, const std::string &);
	~VulkanShader();


	void bind() const {};

    void addUniform(const std::string &) {};
    void updateUniform2i(const std::string &, int, int) const {};
    void updateUniformMatrix4fv(const std::string &, const glm::mat4 &) const {};
    
	std::shared_ptr<VulkanDevice> device;
	std::vector<ShaderModule> shaders;
private:
	std::vector<char> loadShader(const std::string &);
};