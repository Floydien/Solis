#include "physics/shape/cuboid.h"
#include <glm/gtc/quaternion.hpp>

//builds an AABB where all max values are positive and all min values are negative (or 0)
AABB _safeAABB(glm::vec3 halfExtent) 
{
	if(halfExtent.x < 0)
		halfExtent.x = -halfExtent.x;
	if(halfExtent.y < 0)
		halfExtent.y = -halfExtent.y;
	if(halfExtent.z < 0)
		halfExtent.z = -halfExtent.z;

	return AABB(-halfExtent, halfExtent);
}

Cuboid::Cuboid(const glm::vec3 &halfExtent) :
		Shape(PhysicsShapeType::CUBOID, true, _safeAABB(halfExtent)),
		_halfExtent(halfExtent){}


glm::vec3 Cuboid::getSupportPoint(const glm::vec3 &position, const glm::quat &rotation, const glm::vec3 &direction) const {
	glm::vec3 rotated = direction * glm::inverse(rotation);

	glm::vec3 res = halfExtent();

	if(rotated.x < 0)
		res.x = -res.x;
	if(rotated.y < 0)
		res.y = -res.y;
	if(rotated.z < 0)
		res.z = -res.z;
	return res * rotation + position;
}