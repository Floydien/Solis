#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "physics/shape/shape.h"

class PhysicsObject 
{
public:
	PhysicsObject(const glm::vec3 &position, std::shared_ptr<Shape> shape) :
		_position(position), _shape(shape) {};

	inline std::shared_ptr<Shape> shape() 	const { return _shape; }
	inline const glm::vec3& position() 		const { return _position; }
	inline glm::vec3* position() 				  { return &_position; }
	
private:
	glm::vec3 _position;
	std::shared_ptr<Shape> _shape;
};