#pragma once
#ifndef GLFW_INCLUDE_VULKAN
	#define GLFW_INCLUDE_VULKAN
#endif
#include "videodriver.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "input.h"

enum class VideoDriverType 
{
	eOpenGL,
	eVulkan
};

class Scene;

class SolisDevice 
{
public:
	SolisDevice(VideoDriverType, uint32_t, uint32_t);
	~SolisDevice();

	bool run();

	inline VideoDriver *getVideoDriver() { return videoDriver; };
	inline VideoDriverType getDriverType() { return ACTIVE_VIDEO_DRIVER_TYPE; };
	inline GLFWwindow *getWindow() { return window; };
	inline Scene *getScene() { return activeScene; };
	
	inline int32_t getWidth() const { return width; };
	inline int32_t getHeight() const { return height; };

	void setFramerate(uint32_t) { /* TODO: Implement framerate settings*/ };
	bool keyPressed(int32_t key) { return glfwGetKey(window, key) == GLFW_PRESS; };
	bool mousePressed(int32_t key) { return glfwGetMouseButton(window, key) == GLFW_PRESS; };
	glm::vec2 getCursorPosition();
	Input *getInput() { return input; };

private:
	void createDriver();

	const VideoDriverType ACTIVE_VIDEO_DRIVER_TYPE;

    GLFWwindow *window;
    int32_t width;
    int32_t height;
    std::string title;

    Input *input;

    VideoDriver *videoDriver;
    Scene *activeScene;

};