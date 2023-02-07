#pragma once
#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
	Box();
	Box(glm::vec2 position,float width, float height, float orientation, glm::vec2 velocity, float mass, glm::vec4 color);
	~Box();

	virtual void Draw(float alpha);

	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);

	//Getter
	glm::vec2 GetExtents() const { return m_extents; }
	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetLocalX() { return m_localX; }
	glm::vec2 GetLocalY() { return m_localY; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetAngularVelocity() { return m_angularVelocity; }
	float GetHeight() { return m_height; }
	float GetWidth() { return m_width; }
	//Setter

protected:
	glm::vec2 m_extents;
	float m_height;
	float m_width;
};