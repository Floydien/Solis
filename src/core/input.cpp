#include "input.h"
#include "solisdevice.h"


bool Input::isKeyPressed(KeyCode key) {
	return glfwGetKey(device->getWindow(), static_cast<int>(key)) == GLFW_PRESS;
}

bool Input::isKeyDown(KeyCode key) {
	int code = static_cast<int>(key);

	if(glfwGetKey(device->getWindow(), code) == GLFW_PRESS && !keyDown[code]) {
		printf("%d\n", code);
		keyDown[code] = true;
		return true;
	}
	
	return false;
}

bool Input::isKeyUp(KeyCode key) {
	int code = static_cast<int>(key);

	if(glfwGetKey(device->getWindow(), code) != GLFW_PRESS && keyDown[code]) {
		keyDown[code] = false;
		return true;
	}
	return false;
}