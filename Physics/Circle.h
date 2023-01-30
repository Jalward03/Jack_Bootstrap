#pragma once
#include "Rigidbody.h"

class Circle : public Rigidbody
{
public:
	Circle();
	Circle(glm::vec2 position, glm::vec2 velocity, float mass,
		float radius, glm::vec4 colour);
	~Circle();

	virtual void Draw(float alpha);

	//Getter
	float GetRadius() { return m_radius;}
	glm::vec4 GetColor() { return m_color; }
	//Setter

protected:
	float m_radius;
	glm::vec4 m_color;
};