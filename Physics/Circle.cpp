#include "Circle.h"
#include "Gizmos.h"



Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) : Rigidbody(CIRCLE, position, velocity, 0, mass, 0, 0)
{
	m_moment = 0.5f * mass * radius * radius;
	m_radius = radius;
	m_color = color;

	
}

Circle::~Circle()
{
}

void Circle::Draw(float alpha)
{
	CalculateSmoothedPosition(alpha);

	aie::Gizmos::add2DCircle(m_position, m_radius, 50, m_color);
	aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition + m_smoothedLocalX * m_radius, glm::vec4(1, 1, 1, 1));
}


