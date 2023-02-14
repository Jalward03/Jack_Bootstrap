#pragma once
#include "Circle.h"

class PoolBall : public Circle
{
public:

	PoolBall();
	PoolBall(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 color);
	~PoolBall();

	virtual void Draw(float alpha) override;
	float GetRadius() { return m_radius; }

	glm::vec4 GetColor() { return m_color; }
	//Setter
	void SetSunk(bool state) { m_hasSunk = state; }
	bool GetSunk() { return m_hasSunk; }
	void SetSunkTriggered(bool state) { m_hasTriggered = state; }
	bool GetSunkTriggered() { return m_hasTriggered; }
protected:

	float m_radius;
	glm::vec4 m_color;

	bool m_hasSunk = false;
	bool m_hasTriggered = false;
};

