#include "player.h"

static bool pressed[4];

void Player::input(float, SolisDevice *device) {
	if(device->keyPressed(GLFW_KEY_I)) {
		if(!pressed[0]) {
			getTransform()->moveForward(1.0);
			pressed[0] = true;
		}
	} else {
		pressed[0] = false;
	}
	if(device->keyPressed(GLFW_KEY_J)) {
		if(!pressed[1]) {
			getTransform()->moveRight(-1.0);
			pressed[1] = true;
		}
	} else {
		pressed[1] = false;
	}
	if(device->keyPressed(GLFW_KEY_K)) {
		if(!pressed[2]) {
			getTransform()->moveForward(-1.0);
			pressed[2] = true;
		}
	} else {
		pressed[2] = false;
	}
	if(device->keyPressed(GLFW_KEY_L)) {
		if(!pressed[3]) {
			getTransform()->moveRight(1.0);
			pressed[3] = true;
		}
	} else {
		pressed[3] = false;
	}
	
}

void Player::update(float) {

}