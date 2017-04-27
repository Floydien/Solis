#pragma once
#include "camera.h"
#include "solisdevice.h"
#include "videodriver.h"
#include "mesh.h"
#include <vector>
#include "node.h"

class Scene 
{
public:
	Scene(SolisDevice *device) : 
		device(device), 
		driver(device->getVideoDriver()),
		root(new Node(Transform(), "SceneRootNode")),
		activeCamera(nullptr) {};
	~Scene() = default;

	// void loadScene(const std::string &);
	void addToScene(Node *);
	// void addLight(Light *);
	inline const Node *getRootNode() const { return root; };
	inline VideoDriver *getVideoDriver() { return driver; };
	inline std::unique_ptr<Camera> &getActiveCamera() { return activeCamera; };

	void init();
	void input(float);
	void updateScene(float);
	void renderScene() const;

	void addCamera(const glm::vec3 & = glm::vec3());

	Mesh *getMesh(const std::string &);
	// Mesh *getMesh(const std::string &, VertexBuffer *);
	Mesh *getMesh(const std::string &, std::shared_ptr<VertexBuffer>);

private:
	SolisDevice *device;
	VideoDriver *driver;
	Node *root;

	std::unique_ptr<Camera> activeCamera;

	std::vector<Mesh *> meshes;
	//remove later on please
	int meshcount = 0;
};
