#include "Circle.h"
#include "Gizmos.h"



Circle::Circle(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) : Rigidbody(CIRCLE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_color = color;
}

Circle::~Circle()
{
}

void Circle::Draw(float alpha)
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 0, m_color);
}


