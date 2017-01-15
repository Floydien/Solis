#pragma once
#include "videodriver.h"
#include "scene.h"
#include "rectangle.h"
#include "material.h"

class GUIImage {
public:
	GUIImage(const std::string& name, const Rectangle &rect = Rectangle(), Material *material = nullptr) :
		name(std::move(name)),
		transform(rect),
		material(std::shared_ptr<Material>(material)) {};

	void init(Scene *);
	void draw(const VideoDriver *) const;

private:
	std::string name;
	Rectangle transform;
	std::shared_ptr<Material> material;
	std::shared_ptr<Mesh> quad;
};