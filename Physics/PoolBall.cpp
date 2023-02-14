#include "PoolBall.h"

#include <Gizmos.h>
PoolBall::PoolBall(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color) : Circle(position, velocity, mass, radius, color)
{
	m_moment = 0.5f * mass * radius * radius;
	m_radius = radius;
	m_color = color;
}
PoolBall::~PoolBall()
{

}
void PoolBall::Draw(float alpha)
{

	CalculateSmoothedPosition(alpha);

	aie::Gizmos::add2DCircle(m_position, m_radius, 50, m_color);
}
