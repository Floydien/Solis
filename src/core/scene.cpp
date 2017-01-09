#include "node.h"
#include "scene.h"

void Scene::addToScene(Node *node) {
	root->addChild(node);
	node->setScene(this);
}

void Scene::init() {
	root->init();
}

void Scene::input(float delta) {
	if(activeCamera) {
		activeCamera->input(delta, device);
	}

	root->input(delta, device);
}

void Scene::updateScene(float delta) {
	root->update(delta);
}

void Scene::renderScene() const {
	driver->getActiveShader()->bind();
	driver->getActiveShader()->updateUniformMatrix4fv("camera.viewProjection", activeCamera->getViewProjection());

	root->render(driver);
}

// void Scene::addLight(Light *light) {
// 	lights.push_back(light);
// }

Mesh *Scene::getMesh(const std::string &filename) {
	Mesh *mesh = new Mesh(filename);
	driver->createBuffer(mesh->getVertexBuffer());
	return mesh;
}

Mesh *Scene::getMesh(VertexBuffer *buffer) {
	meshcount++;
	Mesh *mesh = new Mesh("name" + std::to_string(meshcount), buffer);
	driver->createBuffer(mesh->getVertexBuffer());
	return mesh;
}

void Scene::addCamera(const glm::vec3 &pos) {
	//TODO: implement multiple cameras

	if(activeCamera) {
		activeCamera.reset();
	}
	activeCamera = std::make_unique<Camera>(
		45.0f, (float)device->getWidth()/(float)device->getHeight(), 0.1f, 1000.0f, pos);
}