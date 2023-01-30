#include "PhysicsScene.h"

#include "PhysicsObject.h"

PhysicsScene::PhysicsScene()
{
	m_timesStep = 0.01f;
	m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
	
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	std::vector<PhysicsObject*>::iterator it;
	it = std::find(m_actors.begin(), m_actors.end(), actor);
	m_actors.erase(it);
}

void PhysicsScene::Update(float dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timesStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timesStep);
		}
		accumulatedTime -= m_timesStep;
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
}
