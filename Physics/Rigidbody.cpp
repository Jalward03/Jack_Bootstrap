#include "Rigidbody.h"
#include "PhysicsScene.h"
#include <iostream>



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position,
	glm::vec2 velocity, float orientation, float mass,
	float moment, float angularVelocity) : PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
	m_angularVelocity = 0;
	m_moment = 0;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_lastOrientation = m_orientation;
	m_lastPosition= m_position;
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep, glm::vec2(0));
	m_orientation += m_angularVelocity * timeStep;
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / GetMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}


void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
		actor2->GetPosition() - m_position);

	glm::vec2 perp(normal.y, -normal.x);

	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->GetPosition(), perp);

	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2)
	{

		glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;


		float mass1 = 1.f / (1.f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.f / (1.f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());

		float elasticity = 1;

		//glm::vec2 force = (1.0f + elasticity) * glm::dot(normal, relativeVelocity) / (mass1 + mass2) *  normal;
		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;


		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->GetPosition());
	}

	
	//float elasticity = 1;
	//float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / ((1 / GetMass()) + (1 / actor2->GetMass()));
	//
	//glm::vec2 force = normal * j;
	//
	//
	//actor2->ApplyForce(force);


}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
} 

float Rigidbody::GetKinematicEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity) * m_angularVelocity;
}

float Rigidbody::GetEnergy()
{
	return GetKinematicEnergy() + GetPotentialEnergy();
}

void Rigidbody::CalculateSmoothedPosition(float alpha)
{
	m_smoothedPosition = alpha * m_position + (1 - alpha) * m_lastPosition;

	float smoothedOrientation = alpha * m_orientation + (1 - alpha) * m_lastOrientation;

	float sn = sinf(smoothedOrientation);
	float cs = cosf(smoothedOrientation);
	m_smoothedLocalX = glm::vec2(cs, sn);
	m_smoothedLocalY = glm::vec2(-sn, cs);

}
