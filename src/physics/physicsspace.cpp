#include "physics/physicsspace.h"


void PhysicsSpace::addObject(std::shared_ptr<PhysicsObject> object) 
{
	if (object) 
	{
		objects.emplace_back(object);
	}
}

void PhysicsSpace::findCollisions() 
{
	for(auto& objectA: objects) 
	{
		for(auto& objectB: objects) 
		{
			if(objectA == objectB) 
			{
				continue;
			}

			if(objectA->shape()->type() == PhysicsShapeType::CUBOID && 
				objectB->shape()->type() == PhysicsShapeType::CUBOID ) 
			{
				// objectA->
			}
		}
	}
}