#include "gui/guiimage.h"
#include "vertexbuffer.h"

void GUIImage::init(Scene *scene) {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	vertices.emplace_back(glm::vec3(transform.center.x - transform.extent.x / 2, transform.center.y - transform.extent.y / 2, 0.0), glm::vec2(0.0f, 0.0f), glm::vec3());
	vertices.emplace_back(glm::vec3(transform.center.x - transform.extent.x / 2, transform.center.y + transform.extent.y / 2, 0.0), glm::vec2(0.0f, 1.0f), glm::vec3());
	vertices.emplace_back(glm::vec3(transform.center.x + transform.extent.x / 2, transform.center.y + transform.extent.y / 2, 0.0), glm::vec2(1.0f, 1.0f), glm::vec3());
	vertices.emplace_back(glm::vec3(transform.center.x + transform.extent.x / 2, transform.center.y - transform.extent.y / 2, 0.0), glm::vec2(1.0f, 0.0f), glm::vec3());

	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(2);
	indices.emplace_back(3);
	indices.emplace_back(0);

	// VertexBuffer *buffer = new VertexBuffer(vertices, indices);
	auto buffer = std::make_shared<VertexBuffer>(vertices, indices);

	quad = std::shared_ptr<Mesh>(scene->getMesh(name, buffer));
}

void GUIImage::draw(const VideoDriver *driver) const {
	driver->getActiveShader()->updateUniform2i("windowSize", 1280, 720);
	driver->bindTexture(material->getTexture());
	driver->drawVertexBuffer(quad->getVertexBuffer());
}
