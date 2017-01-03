#pragma once
#include "node.h"
#include "solisdevice.h"

class NodeComponent {
public:

	virtual ~NodeComponent() = default;

	virtual void init() {};
	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *) const {};

	inline void setParent(Node *parent) { this->parent = parent; };
	inline Node *getParent() const		{ return parent; };

	inline Transform *getTransform() { return parent->getTransform();};
	inline const Transform &getTransform() const { return *parent->getTransform();};

protected:
	Node *parent;
};