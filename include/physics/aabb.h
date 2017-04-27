#pragma once
#include "physicsshape.h"
#include <glm/glm.hpp>

class AABB : public PhysicsShape {
public:
	AABB(const glm::vec3 &position = glm::vec3(), const glm::vec3 &halfExtent = glm::vec3()) : 
		PhysicsShape(PhysicsShapeType::eAABB, position), halfExtent(halfExtent) {};

	// glm::vec3 position;
	glm::vec3 halfExtent;
};