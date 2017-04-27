#include "gui/guibutton.h"


void GUIButton::init(Scene *scene) {
	image = std::make_unique<GUIImage>(name + ".image", transform, new Material(scene->getVideoDriver()->getTexture("solis.png")));
	image->init(scene);
}

void GUIButton::update(std::shared_ptr<SolisDevice> device) {
	if( device->mousePressed(GLFW_MOUSE_BUTTON_LEFT) ) {
		auto pos = device->getCursorPosition();
		if (callback && transform.contains(pos)) {
			callback();
		}
	}
}

void GUIButton::draw(const VideoDriver *driver) const {
	image->draw(driver);
}