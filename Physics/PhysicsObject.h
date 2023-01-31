#pragma once
#include<glm/vec2.hpp>

enum ShapeType {

	PLANE = 0,
	CIRCLE,
	BOX
};
const int SHAPE_COUNT = 3;
class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}
public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw(float alpha) = 0;
	virtual void ResetPosition() {};

	ShapeType GetShapeID() { return m_shapeID; }

protected:
	ShapeType m_shapeID;

};

