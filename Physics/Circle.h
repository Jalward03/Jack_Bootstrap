#pragma once
#include "Rigidbody.h"

class Circle : public Rigidbody
{
public:
	Circle();
	Circle(glm::vec2 position, glm::vec2 velocity, float mass,
		float radius, glm::vec4 color);
	~Circle();

	virtual void Draw(float alpha);
	virtual bool IsInside(glm::vec2 point) override;
	//Getter
	float GetRadius() { return m_radius;}

	glm::vec4 GetColor() { return m_color; }
	//Setter

protected:

	float m_radius;
	glm::vec4 m_color;

};