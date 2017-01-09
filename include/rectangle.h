#pragma once
#include <glm/glm.hpp>

class Rectangle {
public:
	Rectangle(const glm::vec2 &topLeft, const glm::vec2 &bottomRight) :
		topLeft(topLeft),
		bottomRight(bottomRight) {};

	float getWidth() const;
	float getHeight() const;
	glm::vec2 getCenter() const;

	glm::vec2 topLeft;
	glm::vec2 bottomRight;
};