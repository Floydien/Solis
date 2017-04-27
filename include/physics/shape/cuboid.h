#pragma once
#include "shape.h"
#include "supportmap.h"

class Cuboid: public Shape, public SupportMap
{
public:
	Cuboid(const glm::vec3 &halfExtent = glm::vec3());

	inline const glm::vec3 &halfExtent() const { return _halfExtent; }

	glm::vec3 getSupportPoint(const glm::vec3 &, const glm::quat &, const glm::vec3 &) const override;
private:
	glm::vec3 _halfExtent;
};