#pragma once
#include <glm/glm.hpp>
#include <iostream>

class BoundingBall {
public:
	BoundingBall(const glm::vec3 &pos = glm::vec3(), float radius = 0.0f) : 
		pos(pos), rad(radius) {};

	inline const glm::vec3 &position() 	const 	{ return pos; }
	inline float radius()				const 	{ return rad; }

	inline void setMin(const glm::vec3 &pos) 	{ this->pos = pos; }
	inline void setMax(float rad) 				{ this->rad = rad; }

	bool intersects(const BoundingBall &other) const {
		return glm::length(position() - other.position()) 
				<= radius() + other.radius();

	};

	bool contains(const AABB &other) const {
		std::cout << "STUB: BoundingBall::contains(const AABB&) is not yet implemented" << std::endl;
		return false;
	};

private:
	glm::vec3 pos;
	float rad;
};