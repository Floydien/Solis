#pragma once
#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
#endif
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader 
{
public:
    Shader() = default;
    virtual ~Shader() = default;

    virtual void bind() const {};

    virtual void addUniform(const std::string &) = 0;
    virtual void updateUniform2i(const std::string &, int, int) const = 0;
    virtual void updateUniformMatrix4fv(const std::string &, const glm::mat4 &) const = 0;

private:
};