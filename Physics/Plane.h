#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance);
	~Plane();

	virtual void Draw(float alplha);
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void ResetPosition();
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact);

	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }


protected:
	glm::vec2 m_normal;
	glm::vec4 m_colour;
	float m_distanceToOrigin;
};

