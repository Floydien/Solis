#include "node.h"
#include "nodecomponent.h"
#include "scene.h"

Node::~Node() {
	for (size_t i = 0; i < components.size(); i++) {
		if(components[i]) {
			delete components[i];
		}
	}

	for (size_t i = 0; i < children.size(); i++) {
		if(children[i]) {
			delete children[i];
		}
	}	
}

Node *Node::addChild(Node *child) {
	children.push_back(child);
	child->getTransform()->setParent(&transform);
	return this;	
}

Node *Node::addComponent(NodeComponent *component) {
	components.push_back(component);
	component->setParent(this);
	return this;
}

bool Node::removeChild(Node *node) {
	for (std::vector<Node *>::iterator it = children.begin(); 
		it != children.end(); ++it) {
		if (*it == node) {
			children.erase(it);
			return true;
		}
	}
	return false;
}

void Node::init() {
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->init();
	}
	for (size_t i = 0; i < children.size(); i++) {
		children[i]->init();
	}
}
void Node::input(float delta, SolisDevice *device) {
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->input(delta, device);
	}
	for (size_t i = 0; i < children.size(); i++) {
		children[i]->input(delta, device);
	}
}

void Node::update(float delta) {
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->update(delta);
	}
	for (size_t i = 0; i < children.size(); i++) {
		children[i]->update(delta);
	}
}

void Node::render(const VideoDriver *driver) const {
	for (size_t i = 0; i < components.size(); i++) {
		components[i]->render(driver);
	}
	for (size_t i = 0; i < children.size(); i++) {
		children[i]->render(driver);
	}
}

void Node::setScene(Scene *scene) {
	if(this->scene != scene) {
		this->scene = scene;
		for (size_t i = 0; i < children.size(); i++) {
			children[i]->setScene(scene);
		}
	}
}