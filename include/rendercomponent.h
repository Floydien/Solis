#pragma once
#include "nodecomponent.h"
#include "videodriver.h"
#include "material.h"
#include "mesh.h"
#include "scene.h"

class RenderComponent : public NodeComponent {
public:
	RenderComponent(Mesh *mesh, Material *material) : 
		mesh(mesh),
		material(material) {};

	virtual ~RenderComponent();

	virtual void input(float, SolisDevice *) {};
	virtual void update(float) {};
	virtual void render(const VideoDriver *driver) const;

protected:
	Mesh *mesh;
	Material *material;
};