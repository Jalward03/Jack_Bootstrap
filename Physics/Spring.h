#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"


class Spring : public PhysicsObject
{
public:
	Spring();
	Spring(Rigidbody* body1, Rigidbody* body2,
		float springCoefficient, float damping, float restLength,
		glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0));
	~Spring();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw(float alpha);

	glm::vec2 GetContact1();
	glm::vec2 GetContact2();


protected:

	Rigidbody* m_body1;
	Rigidbody* m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	glm::vec4 m_color;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

