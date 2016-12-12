#pragma once
#include "solisdevice.h"
#include "videodriver.h"
// #include "shader.h"
#include "transform.h"
#include <vector>
#include <glm/glm.hpp>

class NodeComponent;
class Scene;


class Node {
public:
	Node(const Transform &transform = Transform()) :
		transform(transform) {};
	~Node();

	Node *addChild(Node *);
	Node *addComponent(NodeComponent *);

	void input(float, SolisDevice *);
	void update(float);
	void render(const VideoDriver *) const;

	void setScene(Scene *);
	inline Scene *getScene() { return scene; };

	inline Transform *getTransform() { return &transform; }
private:
	Scene *scene;

	std::vector<Node *> children;
	std::vector<NodeComponent *> components;

	Transform transform;
};