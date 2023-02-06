#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float orientation, float mass, float moment, float angularVelocity);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);
	float GetPotentialEnergy();
	void CalculateSmoothedPosition(float alpha);

	float GetEnergy() override;
	float GetKinematicEnergy();
	//Getters
	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetMoment() { return m_moment; }
	//Setters
	void SetVelocity(glm::vec2 vel) { m_velocity = vel; }
protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;

	float m_mass;
	float m_orientation;
	float m_angularVelocity;
	float m_moment;

	float m_lastOrientation;
	glm::vec2 m_lastPosition;
	glm::vec4 m_colour;

	glm::vec2 m_smoothedPosition;
	glm::vec2 m_smoothedLocalX;
	glm::vec2 m_smoothedLocalY;
};