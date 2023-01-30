#pragma once
#include "Rigidbody.h"
class Circle : public Rigidbody
{
public:
	Circle();
	Circle(glm::vec2 position, glm::vec2 velocity,
		float mass, float radius, glm::vec4 color);
	~Circle();

	virtual void Draw();

	// Getter

	// Setter

protected:
	float m_radius;
};

