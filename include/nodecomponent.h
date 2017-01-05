#pragma once
#include "solisdevice.h"
#include "transform.h"

class Node;

class NodeComponent {
public:

	virtual ~NodeComponent() = default;

	virtual void init() {};
	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *) const {};

	void setParent(Node *parent);
	Node *getParent() const;

	Transform *getTransform();
	const Transform &getTransform() const;

protected:
	Node *parent;
};