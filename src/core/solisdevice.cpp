#include "solisdevice.h"
#include "../render/opengl/opengldriver.h"
#include "../render/vulkan/vulkandriver.h"
#include "scene.h"
#include "input.h"

SolisDevice::SolisDevice(VideoDriverType type, uint32_t width, uint32_t height) :
		ACTIVE_VIDEO_DRIVER_TYPE(type){
	glfwInit();

    if (type == VideoDriverType::eVulkan) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    } else if (type == VideoDriverType::eOpenGL){
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }

    this->window = glfwCreateWindow(width, height, "", NULL, NULL);
    this->width = width;
    this->height = height;

    if (!this->window) {
	    	throw "Error: Could not create a window";
    }

    glfwMakeContextCurrent(this->window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);   

    input = new Input(this);

    createDriver();
    activeScene = new Scene(this);
}

SolisDevice::~SolisDevice() {
    glfwTerminate();
    if(videoDriver) {
        if(ACTIVE_VIDEO_DRIVER_TYPE == VideoDriverType::eOpenGL) {
            delete static_cast<OpenGLDriver *>(videoDriver);
        } else if(ACTIVE_VIDEO_DRIVER_TYPE == VideoDriverType::eVulkan) {
            delete static_cast<VulkanDriver *>(videoDriver);
        }
    }

    if(activeScene) {
        delete activeScene;
    }

    if(input) {
        delete input;
    }
}

bool SolisDevice::run() {
    glfwSwapBuffers(this->window);
    glfwPollEvents();

	return !glfwWindowShouldClose(this->window);
}

void SolisDevice::createDriver() {
	if(ACTIVE_VIDEO_DRIVER_TYPE == VideoDriverType::eOpenGL) {
		videoDriver = new OpenGLDriver(this);
	} else if (ACTIVE_VIDEO_DRIVER_TYPE == VideoDriverType::eVulkan) {
        // videoDriver = new VulkanDriver(this);
    }
}
