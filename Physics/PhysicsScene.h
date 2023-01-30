#pragma once
#include <glm/vec2.hpp>
#include <vector>

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

	static bool Circle2Circle(PhysicsObject*, PhysicsObject*);

	//Getters
	glm::vec2 GetGravity() { return m_gravity; }
	float GetTimeStep() { return m_timeStep; }

	//Setters
	void SetGravity(const glm::vec2 gravity)
	{
		m_gravity = gravity;
	}

	void SetTimeStep(const float timeStep)
	{
		m_timeStep = timeStep;
	}

private:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
};