#pragma once
#include "nodecomponent.h"
#include "videodriver.h"
#include "material.h"
#include "mesh.h"
#include "scene.h"
// #include "shader.h"
// #include "camera.h"

class RenderComponent : public NodeComponent {
public:
	RenderComponent(Mesh *mesh, Material *material) : 
		mesh(mesh),
		material(material) {};

	virtual ~RenderComponent() {};

	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *driver) const {
		// shader.updateUniforms(getTransform(), material, camera);
		// material.bind();
		driver->getActiveShader()->updateUniformMatrix4fv("transform.model", getTransform().getTransformation());
		driver->bindTexture(material->getTexture());
		driver->drawVertexBuffer(mesh->getVertexBuffer());
	}

protected:
	Mesh *mesh;
	Material *material;
};