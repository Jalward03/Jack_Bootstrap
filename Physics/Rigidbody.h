#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float orientation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void ApplyForce(glm::vec2 force);
	void ApplyForceToActor(Rigidbody* actor2, glm::vec2 force);

	//Getters
	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation;}
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	//Setters
	void SetVelocity(glm::vec2 vel) { m_velocity = vel; }
protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	
	float m_mass;
	float m_orientation;

	glm::vec4 m_colour;
};