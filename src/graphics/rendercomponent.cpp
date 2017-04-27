#include "rendercomponent.h"
#include "opengl/opengldriver.h"

RenderComponent::~RenderComponent() 
{
	// If this is the last mesh thats using this vertexbuffer
	// then remove it from the gpu 
	if (mesh->getVertexBuffer()->getReferenceCount() == 1) 
	{
		auto link = parent->getScene()->getVideoDriver()->getBufferLink(mesh->getVertexBuffer());

		if (link != nullptr) 
		{
			parent->getScene()->getVideoDriver()->deleteBuffer(link);
		}
	}

	delete mesh;
	delete material;
}

void RenderComponent::render(const VideoDriver *driver) const 
{
	driver->getActiveShader()->updateUniformMatrix4fv("transform.model", getTransform().getTransformation());
	driver->bindTexture(material->getTexture());
	driver->drawVertexBuffer(mesh->getVertexBuffer());
}