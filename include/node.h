#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "solisdevice.h"
#include "videodriver.h"
#include "transform.h"

class NodeComponent;
class Scene;


class Node {
public:
	Node(const Transform &transform = Transform(), const std::string &name = "") :
		scene(nullptr),
		transform(transform),
		name(std::move(name)) {};
	~Node();

	Node *addChild(Node *);
	Node *addComponent(NodeComponent *);

	/* 	Deletes the child node if its a direct child.
		Returns true if the passed node was found and deleted. */
	bool removeChild(Node *);

	void init();
	void input(float, SolisDevice *);
	void update(float);
	void render(const VideoDriver *) const;

	void setScene(Scene *);
	inline Scene *getScene() 			{ return scene; };
	inline Transform *getTransform() 	{ return &transform; }
	inline const std::string &getName() { return name; }

private:
	Scene *scene;

	std::vector<Node *> children;
	std::vector<NodeComponent *> components;

	Transform transform;
	const std::string name;
};