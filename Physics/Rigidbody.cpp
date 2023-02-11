#include "Rigidbody.h"
#include "PhysicsScene.h"
#include "Demos.h"
#include <iostream>



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position,
	glm::vec2 velocity, float orientation, float mass,
	float moment, float angularVelocity) : PhysicsObject(shapeID)
{
	m_linearDrag = 1.3f;
	m_angularDrag = 1.3f;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
	m_angularVelocity = 0;
	m_moment = 0;
	m_elasticity = 0.8f;
	m_isKinematic = false;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	Rigidbody::CalculateAxes();

	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0;
		return;
	}
	m_lastOrientation = m_orientation;
	m_lastPosition = m_position;
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep, glm::vec2(0));
	m_orientation += m_angularVelocity * timeStep;
	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
	{
		m_angularVelocity = 0;
	}
}

void Rigidbody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / GetMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}


void Rigidbody::ResolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
{
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->GetPosition() - GetPosition());
	glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;

	glm::vec2 perp(normal.y, -normal.x);
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->GetPosition(), perp);
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2)
	{


		float mass1 = 1.f / (1.f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.f / (1.f / actor2->GetMass() + (r2 * r2) / actor2->GetMoment());

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
			glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->GetMass())));

		glm::vec2 force = normal * j;

		ApplyForce(-force, contact - m_position);

		actor2->ApplyForce(force, contact - actor2->GetPosition());

		//glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;


		//actor2->ApplyForce(force, contact - actor2->GetPosition());
		if (pen > 0)
			PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
	}
}

float Rigidbody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}

float Rigidbody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) +
		m_moment * m_angularVelocity * m_angularVelocity);
}

void Rigidbody::CalculateAxes()
{
	float sn = sinf(m_orientation);
	float cs = cosf(m_orientation);
	m_localX = glm::vec2(cs, sn);
	m_localY = glm::vec2(-sn, cs);
}

glm::vec2 Rigidbody::ToWorld(glm::vec2 contact)
{
	return m_position + m_localX * contact.x + m_localY * contact.y;
}

float Rigidbody::GetEnergy()
{
	return GetKineticEnergy() + GetPotentialEnergy();
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
