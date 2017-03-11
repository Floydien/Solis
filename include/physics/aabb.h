#pragma once
#include <glm/glm.hpp>

class AABB {
public:
	AABB(const glm::vec3 &position = glm::vec3(), const glm::vec3 &halfExtent = glm::vec3()) : 
		position(position), halfExtent(halfExtent) {};

	glm::vec3 position;
	glm::vec3 halfExtent;
};