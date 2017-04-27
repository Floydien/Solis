#pragma once
#include <glm/glm.hpp>

class AABB {
public:
	AABB(const glm::vec3 &min = glm::vec3(), const glm::vec3 &max = glm::vec3()) : 
		minPos(min), maxPos(max) {};

	inline const glm::vec3 &min() 	const 	{ return minPos; }
	inline const glm::vec3 &max()	const 	{ return maxPos; }
	inline glm::vec3 center() 		const 	{ return (minPos + maxPos) / 2.0f; };
	inline glm::vec3 halfExtent() 	const	{ return (maxPos - minPos) / 2.0f; };

	inline void setMin(const glm::vec3 &pos) { minPos = pos; }
	inline void setMax(const glm::vec3 &pos) { maxPos = pos; }

	bool intersects(const AABB &other) const {
		return glm::all(glm::lessThanEqual(minPos, other.maxPos)) &&
				glm::all(glm::lessThanEqual(other.minPos, maxPos));
	};

	bool contains(const AABB &other) const {
		return glm::all(glm::lessThanEqual(minPos, other.minPos)) &&
				glm::all(glm::lessThanEqual(other.maxPos, maxPos));
	};

private:
	glm::vec3 minPos;
	glm::vec3 maxPos;
};