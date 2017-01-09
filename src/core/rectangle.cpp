#include "rectangle.h"

float Rectangle::getWidth() const {
	return glm::abs(topLeft.x - bottomRight.x); //return absolute value
}

float Rectangle::getHeight() const {

	return glm::abs(topLeft.y - bottomRight.y); //return absolute value
}

glm::vec2 Rectangle::getCenter() const {
	return (topLeft + bottomRight) / 2.0f;
}
