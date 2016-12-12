#pragma once
#include "node.h"
#include "solisdevice.h"
// #include "../render/window.h"
// #include "../render/shader.h"

class NodeComponent {
public:

	virtual ~NodeComponent() {};

	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *) const {};

	inline void setParent(Node *parent) { this->parent = parent; };

	inline Transform *getTransform() { return parent->getTransform();};
	inline const Transform &getTransform() const { return *parent->getTransform();};

protected:
	Node *parent;
};