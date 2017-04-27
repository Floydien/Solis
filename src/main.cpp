#include <vector>
#include "game/game.h"
// #include "physics/boundingvolume/aabb.h"
// #include "physics/physicsobject.h"
#include <vector>
#include <iostream>
#include "physics/shape/cuboid.h"
#include <glm/gtc/quaternion.hpp>

int main() 
{ 
	// Cuboid cube(glm::vec3(2));

	// glm::vec3 positionA(2.0f, 3.0f, 0.5f);
	// glm::vec3 positionB(5.0f);

	// float rad = glm::radians(45.0f);
	// glm::quat rotation = glm::rotate(glm::quat(), rad, glm::vec3(1.0f, 0.0f, 0.0f));

	// auto a = cube.getSupportPoint(positionA, rotation, positionB - positionA);
	// auto b = cube.getSupportPoint(positionB, glm::quat(), positionA - positionB);

	// if((positionA.x <= positionB.x && a.x > b.x) ||
	// 	(positionA.x > positionB.x && a.x <= b.x)) 
	// 		std::cout << "intersecting in x" << std::endl;

	// if((positionA.y <= positionB.y && a.y > b.y) ||
	// 	(positionA.y > positionB.y && a.y <= b.y)) 
	// 		std::cout << "intersecting in y" << std::endl;

	// if((positionA.z <= positionB.z && a.z > b.z) ||
	// 	(positionA.z > positionB.z && a.z <= b.z)) 
	// 		std::cout << "intersecting in z" << std::endl;

	// std::cout << "x: " << rotation.x << " y: " << rotation.y << " z: " << rotation.z << " w: " << rotation.w << std::endl;
	// std::cout << "x: " << a.x << " y: " << a.y << " z: " << a.z << std::endl;
	// std::cout << "x: " << b.x << " y: " << b.y << " z: " << b.z << std::endl;

	// AABB aabb(glm::vec3(0,1,2), glm::vec3(4,2,4));
	// AABB aabb2(glm::vec3(5,5,5), glm::vec3(6,7,6));
	// std::cout << (aabb.intersects(aabb2) ? "true" : "false") << std::endl;
	// std::cout << "min: " << aabb.min().x << " " << aabb.min().y << " " << aabb.min().z << " " << std::endl;
	// std::cout << "max: " << aabb.max().x << " " << aabb.max().y << " " << aabb.max().z << " " << std::endl;
	// std::cout << "center: " << aabb.center().x << " " << aabb.center().y << " " << aabb.center().z << " " << std::endl;
	// std::cout << "halfExtent: " << aabb.halfExtent().x << " " << aabb.halfExtent().y << " " << aabb.halfExtent().z << " " << std::endl;

	#ifndef USE_VULKAN 
	{ 
		Game g;
		g.init(); 
		g.run();
	}
	#else
	SolisDevice device(VideoDriverType::eVulkan, 1280, 720);
	#endif 
}
  