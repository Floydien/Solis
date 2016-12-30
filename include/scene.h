#pragma once
#include "camera.h"
#include "solisdevice.h"
#include "videodriver.h"
#include "mesh.h"
#include <vector>
#include "node.h"
// class Node;

class Scene {
public:
	Scene(SolisDevice *device) : 
		device(device), 
		driver(device->getVideoDriver()),
		root(Transform(), "SceneRootNode"),
		activeCamera(nullptr) {};
	~Scene();

	// void loadScene(const std::string &);
	void addToScene(Node *);
	// void addLight(Light *);
	inline const Node &getRootNode() { return root; };
	inline VideoDriver *getVideoDriver() { return driver; };
	inline Camera *getActiveCamera() { return activeCamera; };

	void input(float);
	void updateScene(float);
	void renderScene() const;

	void addCamera(const glm::vec3 & = glm::vec3());

	Mesh *getMesh(const std::string &);
	Mesh *getMesh(VertexBuffer *);

private:
	SolisDevice *device;
	VideoDriver *driver;
	Node root;

	Camera *activeCamera;

	//remove later on please
	int meshcount = 0;
};
