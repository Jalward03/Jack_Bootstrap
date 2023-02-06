#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Circle.h"
#include "Plane.h"
#include <glm/glm.hpp>

glm::vec2 PhysicsScene::m_gravity(0, 0);

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	m_actors.erase(std::find(m_actors.begin(), m_actors.end(), actor));
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle, PhysicsScene::Plane2Box,
	PhysicsScene::Circle2Plane,  PhysicsScene::Circle2Circle,  PhysicsScene::Circle2Box,
	PhysicsScene::Box2Plane,  PhysicsScene::Box2Circle,  PhysicsScene::Box2Box
};

void PhysicsScene::Update(float dt)
{
	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		int actorCount = m_actors.size();

		

		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysicsObject* object1 = m_actors[outer];
				PhysicsObject* object2 = m_actors[inner];
				int shapeId1 = object1->GetShapeID();
				int shapeId2 = object2->GetShapeID();

				int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
				fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
				if (collisionFunctionPtr != nullptr)
				{
					collisionFunctionPtr(object1, object2);
				}

			}
		}
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw(1);
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject* ob1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return  PhysicsScene::Circle2Plane(obj2, obj1);
}

bool PhysicsScene::Plane2Box(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Circle* circle = dynamic_cast<Circle*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision
	if (circle != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		float sphereToPlane = glm::dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();

		float intersection = circle->GetRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(circle->GetVelocity(), plane->GetNormal());
		glm::vec2 contact = circle->GetPosition() + (collisionNormal * -circle->GetRadius());

		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			//set Circle velocity to zero here

			plane->ResolveCollision(circle, contact);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Circle2Circle(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to Circle and Circle
	Circle* circle1 = dynamic_cast<Circle*>(obj1);
	Circle* circle2 = dynamic_cast<Circle*>(obj2);
	// if we are successful then test for collision
	if (circle1 != nullptr && circle2 != nullptr)
	{
		if (circle1->GetRadius() + circle2->GetRadius() > glm::distance(circle1->GetPosition(), circle2->GetPosition()))
		{
			circle1->ResolveCollision(circle2, 0.5f * (circle1->GetPosition() + circle2->GetPosition()));

			return true;
		}
		
	}
	return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Box2Circle(PhysicsObject*, PhysicsObject*)
{
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject*, PhysicsObject*)
{
	return false;
}

float PhysicsScene::GetTotalEnergy()
{
	float total = 0;

	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* obj = *it;
		total += obj->GetEnergy();
	}
	return total;
}
