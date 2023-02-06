#pragma once
#include <glm/vec2.hpp>
#include <vector>
#include "Circle.h"

class PhysicsObject;

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);

	void Update(float dt);
	void Draw();

	void CheckForCollision();


	static bool Plane2Plane(PhysicsObject*, PhysicsObject*);
	static bool Plane2Circle(PhysicsObject*, PhysicsObject*);
	static bool Plane2Box(PhysicsObject*, PhysicsObject*);
	static bool Circle2Plane(PhysicsObject*, PhysicsObject*);
	static bool Circle2Circle(PhysicsObject*, PhysicsObject*);
	static bool Circle2Box(PhysicsObject*, PhysicsObject*);
	static bool Box2Plane(PhysicsObject*, PhysicsObject*);
	static bool Box2Circle(PhysicsObject*, PhysicsObject*);
	static bool Box2Box(PhysicsObject*, PhysicsObject*);

	//Getters
	static glm::vec2 GetGravity() { return m_gravity; }
	float GetTimeStep() { return m_timeStep; }
	float GetTotalEnergy();
	Circle* GetRocket() { return m_rocket; }

	//Setters
	void SetGravity(const glm::vec2 gravity)
	{
		m_gravity = gravity;
	}

	void SetTimeStep(const float timeStep)
	{
		m_timeStep = timeStep;
	}
protected:
	

private:
	static glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	Circle* m_rocket;
};