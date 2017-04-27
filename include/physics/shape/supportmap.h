#pragma once
#include <glm/glm.hpp>

class SupportMap
{
public:
	virtual glm::vec3 getSupportPoint(
			const glm::vec3 &/*position*/, 
			const glm::quat &/*rotation*/, 
			const glm::vec3 &/*direction*/) const = 0;
};