#include "physics/raycast.h"
#include "physics/aabb.h"
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>


// TODO: fix that it only goes towards the direction and not also towards the negative
bool RayCast::intersects(AABB &other) 
{
	auto relativePos = other.position - this->position;

	// Rotate aroung the Z Axis (X = 0)
	float angleY = glm::atan(direction.x / direction.y);
	float rotatedPositionX = glm::abs(relativePos.x * glm::cos(angleY) - relativePos.y * glm::sin(angleY));
	float rotatedHalfExtentX = glm::abs(other.halfExtent.x * glm::cos(angleY) + glm::abs(other.halfExtent.y * glm::sin(angleY)));

	if(rotatedPositionX > rotatedHalfExtentX) 
	{
		return false;
	}

	// Rotate around the X Axis (Y = 0)
	float angleZ = glm::atan(direction.y / direction.z);
	float rotatedPositionY = glm::abs(relativePos.y * glm::cos(angleZ) - relativePos.z * glm::sin(angleZ));
	float rotatedHalfExtentY = glm::abs(other.halfExtent.y * glm::cos(angleZ) + glm::abs(other.halfExtent.z * glm::sin(angleZ)));

	if(rotatedPositionY > rotatedHalfExtentY) 
	{
		return false;
	}

	// Rotate around the Y Axis (Z = 0)
	float angleX = glm::atan(direction.z / direction.x);
	float rotatedPositionZ = glm::abs(relativePos.z * glm::cos(angleX) - relativePos.x * glm::sin(angleX));
	float rotatedHalfExtentZ = glm::abs(other.halfExtent.z * glm::cos(angleX) + glm::abs(other.halfExtent.x * glm::sin(angleX)));

	if(rotatedPositionZ > rotatedHalfExtentZ) 
	{
		return false;
	}

	return true;
}