#pragma once
#include <glm/glm.hpp>

class Rectangle 
{
public:
	Rectangle(const glm::vec2 &center = glm::vec2(), const glm::vec2 &extent = glm::vec2()) :
		center(std::move(center)),
		extent(std::move(extent)) {};


	inline float getWidth() const { return extent.x; };
	inline float getHeight() const { return extent.y; };
	inline glm::vec2 getExtent() const { return extent; };
	inline glm::vec2 getCenter() const { return center; };

	inline bool contains(const glm::vec2 &pos) const {
		return pos.x <= center.x + extent.x / 2.0f &&
			pos.x >= center.x - extent.x / 2.0f &&
			pos.y <= center.y + extent.y / 2.0f &&
			pos.y >= center.y - extent.y / 2.0f;
	};

	glm::vec2 center;
	glm::vec2 extent;
};