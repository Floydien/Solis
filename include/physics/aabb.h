#pragma once
#include <glm/glm.hpp>

class AABB {
public:
	AABB(const glm::vec3 &pos = glm::vec3(), const glm::vec3 &halfextent = glm::vec3(1.0f)) : 
		pos(pos), halfextent(halfextent) {};

	glm::vec3 pos;
	glm::vec3 halfextent;
private:
};