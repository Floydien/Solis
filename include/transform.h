#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

/** 
 *	Class to represent the transformation of an object in 3D-space
 */
class Transform {
public:
	Transform(const glm::vec3& position = glm::vec3(0,0,0),
			const glm::quat& rotation = glm::quat(),
			const glm::vec3& scale = glm::vec3(1,1,1)):
				position(position),
				rotation(rotation),
				scale(scale),
				parentMatrix(glm::mat4(1)),
				parent(nullptr)
				{};
	~Transform() = default;

	void rotate(const glm::quat& rotation);
	void rotate(const glm::vec3& axis, float angle);
	void moveForward(float amount);
	void moveRight(float amount);
	void move(const glm::vec3& direction, float amount);

	glm::mat4 getTransformation() const;

	inline const glm::vec3& getPosition() 	const 	{ return position;	}
	inline const glm::quat& getRotation() 	const 	{ return rotation;	}
	inline const glm::vec3& getScale() 		const 	{ return scale;		}
	inline glm::vec3* getPosition() 				{ return &position;	}
	inline glm::quat* getRotation() 				{ return &rotation;	}
	inline glm::vec3* getScale() 					{ return &scale;	}
	glm::vec3 getTransformedPosition() 		const;
	glm::quat getTransformedRotation() 		const;
	glm::vec3 getForward()					const;
	glm::vec3 getRight()					const;
	glm::vec3 getUp()						const;


	inline void setPosition(const glm::vec3& position) 	{ this->position = position; }
	inline void setRotation(const glm::quat& rotation) 	{ this->rotation = rotation; }
	inline void setScale(const glm::vec3& scale) 		{ this->scale = scale; }
	inline void setScale(float scale) 					{ this->scale = glm::vec3(scale); }
	inline void setParent(Transform* parent) 			{ this->parent = parent; }

private:
	const glm::mat4& getParentMatrix() const;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	mutable glm::mat4 parentMatrix;
	Transform* parent;

};
