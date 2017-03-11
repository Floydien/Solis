#pragma once
#include <glm/glm.hpp>

class AABB;

class RayCast {
public:
	RayCast(const glm::vec3 &position, const glm::vec3 &direction) : 
		position(position), direction(glm::normalize(direction)) {};
	
	bool intersects(AABB &other);

	glm::vec3 position;
	glm::vec3 direction;
};