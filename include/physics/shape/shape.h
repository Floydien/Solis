#pragma once
#include "physics/boundingvolume/aabb.h"
#include "supportmap.h"

enum class PhysicsShapeType 
{
	CUBOID,
};

class Shape 
{
public:
	Shape(PhysicsShapeType type, bool supportMap, const AABB &boundings = AABB()) : 
		_type(type),
		_hasSupportMap(supportMap),
		boundingBox(boundings) {};

	inline AABB *aabb() 				  { return &boundingBox; }
	inline const AABB &aabb() 		const { return boundingBox; }
	inline PhysicsShapeType type() 	const { return _type; }
	inline bool hasSupportMap() 		const { return _hasSupportMap; }

private:
	PhysicsShapeType _type;
	bool _hasSupportMap; // Stupid polymorphism
	AABB boundingBox;
};