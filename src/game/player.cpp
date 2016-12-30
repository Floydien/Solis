#include "player.h"

//TODO: Find more elegant solution for this
static bool pressed[4];

void Player::init() {
	prevPosition = *getTransform()->getPosition();
}

void Player::input(float, SolisDevice *device) {
	if(device->keyPressed(GLFW_KEY_I)) {
		if(!pressed[0] && getTransform()->getPosition()->z < 31) {
			getTransform()->moveForward(-1.0);
			pressed[0] = true;
		}
	} else {
		pressed[0] = false;
	}
	if(device->keyPressed(GLFW_KEY_J)) {
		if(!pressed[1] && getTransform()->getPosition()->x < 31) {
			getTransform()->moveRight(1.0);
			pressed[1] = true;
		}
	} else {
		pressed[1] = false;
	}
	if(device->keyPressed(GLFW_KEY_K)) {
		if(!pressed[2] && getTransform()->getPosition()->z > 0) {
			getTransform()->moveForward(1.0);
			pressed[2] = true;
		}
	} else {
		pressed[2] = false;
	}
	if(device->keyPressed(GLFW_KEY_L)) {
		if(!pressed[3] && getTransform()->getPosition()->x > 0) {
			getTransform()->moveRight(-1.0);
			pressed[3] = true;
		}
	} else {
		pressed[3] = false;
	}
	
}

void Player::update(float) {
	auto pos = *getTransform()->getPosition();
	if(pos != prevPosition) {
		if((int)pos.x >= 32 || (int)pos.y >= 32) {
			getTransform()->setPosition(prevPosition);
			return;
		}
		if(field->blocks.at((int)pos.x).at((int)pos.z)->getType() != BlockType::eEmpty) {
			// printf("%d\n", (int)field->blocks.at((int)pos.x).at((int)pos.z).getType());
			getTransform()->setPosition(prevPosition);
		} else {
			prevPosition = *getTransform()->getPosition();
		}
	}

}