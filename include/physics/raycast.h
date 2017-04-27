#pragma once
#include "physicsshape.h"
#include <glm/glm.hpp>

class AABB;

class RayCast : public PhysicsShape 
{
public:
	RayCast(const glm::vec3 &position, const glm::vec3 &direction) : 
		PhysicsShape(PhysicsShapeType::eRayCast, position), direction(glm::normalize(direction)) {};
	
	bool intersects(AABB &other);

	// glm::vec3 position;
	glm::vec3 direction;
};