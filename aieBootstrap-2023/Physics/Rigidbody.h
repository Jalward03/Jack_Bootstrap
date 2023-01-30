#pragma once
#include "PhysicsObject.h"

#include<glm/glm.hpp>
class Rigidbody : public PhysicsObject
{
public:
	Rigidbody();
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force);
	void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);

	// Getters

	// Setters

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation;

	glm::vec4 m_color;
};

