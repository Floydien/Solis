#pragma once
#include <vector>
#include <memory>
#include "physicsobject.h"

struct Intersection 
{
	std::shared_ptr<PhysicsObject> objectA;
	std::shared_ptr<PhysicsObject> objectB;
};

class PhysicsSpace 
{
public:
	PhysicsSpace();
	~PhysicsSpace();
	
	void addObject(std::shared_ptr<PhysicsObject>);
	void findCollisions();

private:
	std::vector<std::shared_ptr<PhysicsObject>> objects;
	std::vector<Intersection> intersections;
};