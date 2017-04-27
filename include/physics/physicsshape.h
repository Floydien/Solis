#pragma once
#include "glm/glm.hpp"

enum class PhysicsShapeType {
	eAABB,
	eRayCast,
};

class PhysicsShape {
public:
	PhysicsShape(PhysicsShapeType type, const glm::vec3 &position) : type(type), position(position) {};

	PhysicsShapeType type;
	glm::vec3 position;
private:
};