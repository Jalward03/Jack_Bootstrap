#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>
#include <functional>
#include <list>

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float orientation, float mass, float moment, float angularVelocity);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);

	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr, float pen = 0);
	float GetPotentialEnergy();
	void CalculateSmoothedPosition(float alpha);

	float GetEnergy() override;
	float GetKineticEnergy();
	//Getters
	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetLastPosition() const { return m_lastPosition; }
	float GetOrientation() { return m_orientation; }
	float GetElasticity() { return m_elasticity; }
	float GetAngularVelocity() { return m_angularVelocity; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment; }
	//Setters
	void SetVelocity(glm::vec2 vel) { m_velocity = vel; }
	void SetAngularVelocity(float angVel) { m_angularVelocity = angVel; }
	void SetPosition(glm::vec2 pos) { m_position = pos; }
	void SetElasticity(float elasticity) { m_elasticity = elasticity; }
	void CalculateAxes();
	void SetKinematic(bool state) { m_isKinematic = state; }
	void SetTrigger(bool state) { m_isTrigger = state; }
	void SetColour(glm::vec4 colour) { m_colour = colour; }

	

	bool IsKinematic() { return m_isKinematic; }
	bool IsTrigger() { return m_isTrigger; }

	void TriggerEnter(PhysicsObject* actor2);


	std::function<void(PhysicsObject*)> collisionCallback;

	glm::vec2 ToWorld(glm::vec2 contact);

	std::function<void(PhysicsObject*)> triggerEnter;
	std::function<void(PhysicsObject*)> triggerExit;
	

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

	glm::vec2 m_localX;
	glm::vec2 m_localY;

	float m_elasticity;
	float m_linearDrag;
	float m_angularDrag;

	bool m_isKinematic;
	bool m_isTrigger;
	std::list<PhysicsObject*> m_objectsInside;
	std::list<PhysicsObject*> m_objectsInsideThisFrame;


};