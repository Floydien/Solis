#pragma once
#include <array>
#include <glm/glm.hpp>
#include "physics/aabb.h"

template<typename T>
class Octree {
public:
	Octree(const glm::vec3 &size = glm::vec3()) : 
		bounds({glm::vec3(), size}) {};

private:
	std::array<Octree, 8> nodes;
	AABB bounds;

	T *content;
};