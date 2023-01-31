#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"
#include "Circle.h"
#include "Plane.h"

#include "PhysicsScene.h"

#include <glm/ext.hpp>

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	//increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);

	

	DemoStartUp(1);

	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	DemoUpdates(input, deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DemoStartUp(int num)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif // NewtonsFirstLaw

#ifdef NewtonsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0, -10));
	Circle* ball;
	ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);

#endif // NewtonsSecondLaw
#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0, 0));  // turn off gravity

	Circle* ball1 = new Circle(glm::vec2(-8, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(8, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForceToActor(ball2, glm::vec2(-2, 0));
#endif // NewtonsThirdLaw

#ifdef SimulatingCollision
	m_physicsScene->SetGravity(glm::vec2(0, 0));  // turn off gravity

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0, 0), 4.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0, 0), 4.0, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(30, 0));
	ball2->ApplyForce(glm::vec2(-15, 0));
#endif // SimulatingCollision

#ifdef SimulatingRocket

	m_physicsScene->SetGravity(glm::vec2(0, 0));
	
	Circle* ball = new Circle(glm::vec2(0, -40), glm::vec2(0, 0), 1000, 15, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(ball);
	ball->ApplyForce(glm::vec2(0, 20));

#endif // SimulatingRocket
}

void PhysicsApp::DemoUpdates(aie::Input* input, float dt)
{
#ifdef SimulatingRocket

	Circle* ball = new Circle(m_physicsScene->GetRocket()->GetPoisition(),
		m_physicsScene->GetRocket()->GetVelocity(),
		m_physicsScene->GetRocket()->GetMass(),
		m_physicsScene->GetRocket()->GetRadius(),
		m_physicsScene->GetColor());

#endif

#ifdef Planes
	m_physicsScene->SetGravity(glm::vec2(0, -9.82));
	Plane* plane = new Plane(glm::vec2(0, 1), -30);
	m_physicsScene->AddActor(plane);

#endif // Planes
}

float PhysicsApp::DegreeToRadian(float degree)
{
	return degree * (PI / 180);
}
