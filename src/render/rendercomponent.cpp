#include "rendercomponent.h"

RenderComponent::~RenderComponent() {
	delete mesh;
	delete material;
}

void RenderComponent::render(const VideoDriver *driver) const {
		driver->getActiveShader()->updateUniformMatrix4fv("transform.model", getTransform().getTransformation());
		driver->bindTexture(material->getTexture());
		driver->drawVertexBuffer(mesh->getVertexBuffer());
}