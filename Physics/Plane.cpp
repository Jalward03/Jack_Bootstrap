#include "Plane.h"
#include "Gizmos.h"


Plane::Plane(glm::vec2 normal, float distance) : PhysicsObject(ShapeType::PLANE)
{
	
	m_distanceToOrigin = distance;
	m_normal = normal;
	m_colour = glm::vec4(1);

}

Plane::~Plane()
{
}

void Plane::Draw(float alplha)
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	glm::vec2 parralel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parralel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parralel * lineSegmentLength);
	aie::Gizmos::add2DLine(start, end, m_colour);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.f, start - m_normal * 10.f, m_colour, colourFade, colourFade);
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::ResetPosition()
{
}

void Plane::ResolveCollision(Rigidbody* actor2, glm::vec2 contact)
{
	glm::vec2 vRel = actor2->GetVelocity();
	float e = 1;
	float j = glm::dot(-(1 + e) * (vRel), m_normal / (1 / actor2->GetMass()));

	glm::vec2 force = m_normal * j;

	actor2->ApplyForce(force, contact);
}
