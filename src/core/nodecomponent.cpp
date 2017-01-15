#include "nodecomponent.h"
#include "node.h"

void NodeComponent::setParent(Node *parent) { 
	this->parent = parent; 
}

Node *NodeComponent::getParent() const { 
	return parent; 
}

Transform *NodeComponent::getTransform() { 
	return parent->getTransform();
}
const Transform &NodeComponent::getTransform() const { 
	return *parent->getTransform();
}