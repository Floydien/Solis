#include "node.h"
#include "nodecomponent.h"
#include "scene.h"

Node::~Node() {
	for (auto component : components) {
		delete component;
	}
	for (auto child : children) {
		delete child;
	}
	/*for (size_t i = 0; i < components.size(); i++) {
		if(components[i]) {
		}
	}

	for (size_t i = 0; i < children.size(); i++) {
		if(children[i]) {
			delete children[i];
		}
	}*/	
}

Node *Node::addChild(Node *child) {
	child->setScene(scene);
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
	for (auto it = children.begin(); 
		it != children.end(); ++it) {
		if (*it == node) {
			delete *it;
			children.erase(it);
			return true;
		}
	}
	return false;
}

void Node::init() {
	for (auto component : components) {
		component->init();
	}
	for (auto child : children) {
		child->init();
	}
}
void Node::input(float delta, SolisDevice *device) {
	for (auto component : components) {
		component->input(delta, device);
	}
	for (auto child : children) {
		child->input(delta, device);
	}
}

void Node::update(float delta) {
	for (auto component : components) {
		component->update(delta);
	}
	for (auto child : children) {
		child->update(delta);
	}
}

void Node::render(const VideoDriver *driver) const {
	for (auto component : components) {
		component->render(driver);
	}
	for (auto child : children) {
		child->render(driver);
	}
}

void Node::setScene(Scene *scene) {
	if(this->scene != scene) {
		this->scene = scene;

		for (auto child : children) {
			child->setScene(scene);
		}
	}
}