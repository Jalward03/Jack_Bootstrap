#include "GameScene.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"

#include "PhysicsScene.h"

#include <glm/ext.hpp>
#include <iostream>

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::startup()
{
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);

	for (int i = 0; i < 7; i++)
	{
		Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4, 4, glm::vec4(1, 1, 0, 1));
		m_yellows.push_back(circle);
		m_balls.push_back(circle);
	}
	for (int i = 0; i < 7; i++)
	{
		Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4, 4, glm::vec4(1, 0, 0, 1));
		m_reds.push_back(circle);
		m_balls.push_back(circle);
	}

	m_whiteBall = new Circle(glm::vec2(50, 0), glm::vec2(0), 3.5f, 3.5f, glm::vec4(1, 1, 1, 1));
	m_blackBall = new Circle(glm::vec2(0), glm::vec2(0), 4, 4, glm::vec4(0, 0, 1, 1));

	Plane* plane1 = new Plane(glm::vec2(1, 0), -80);
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -80);
	Plane* plane3 = new Plane(glm::vec2(0, -1), -45);
	Plane* plane4 = new Plane(glm::vec2(0, 1), -45);

	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);

	m_balls.push_back(m_blackBall);
	m_balls.push_back(m_whiteBall);

	for (auto ball : m_balls)
	{
		m_physicsScene->AddActor(ball);
	}

	SetBallPositions();

	m_playersTurn = 1;
	m_ballTypeAssigned = false;
	return true;
}

void GameScene::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
}

void GameScene::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();


	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();


	if (m_sunk.size() > 0 && !m_ballTypeAssigned)
	{
		AssignBallType();
	}

	if (input->wasMouseButtonReleased(0) && m_whiteBall->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		
		glm::vec2 whitePos = m_whiteBall->GetPosition();

		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		m_whiteBall->ApplyForce(-mouseToWhite * glm::vec2(500, 500), glm::vec2(0));

		m_playersTurn == 1 ? m_playersTurn = 2 : m_playersTurn = 1;
	}




}

void GameScene::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	// draw your stuff here!
	static float aspectRatio = 16.f / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));


	// output some text, uses the last used colour

	// done drawing sprites
	m_2dRenderer->end();
}

void GameScene::SetBallPositions()
{
	// first row
	m_balls[0]->SetPosition(glm::vec2(-25, 0));
	// second row
	m_balls[2]->SetPosition(glm::vec2(-33, 4.1));
	m_balls[10]->SetPosition(glm::vec2(-33, -4.1));
	// third row
	m_balls[14]->SetPosition(glm::vec2(-41, 0));
	m_balls[3]->SetPosition(glm::vec2(-41, -8.2));
	m_balls[9]->SetPosition(glm::vec2(-41, 8.2));
	// fourth row							 
	m_balls[6]->SetPosition(glm::vec2(-49, 12.3));
	m_balls[11]->SetPosition(glm::vec2(-49, 4.1));
	m_balls[5]->SetPosition(glm::vec2(-49, -4.1));
	m_balls[8]->SetPosition(glm::vec2(-49, -12.3));
	// fifth row							 
	m_balls[12]->SetPosition(glm::vec2(-57, 16.4));
	m_balls[7]->SetPosition(glm::vec2(-57, -8.2));
	m_balls[13]->SetPosition(glm::vec2(-57, 0));
	m_balls[1]->SetPosition(glm::vec2(-57, 8.2));
	m_balls[4]->SetPosition(glm::vec2(-57, -16.4));
}

void GameScene::AssignBallType()
{
	m_ballTypeAssigned = true;

	if (m_sunk[0]->GetColor() == glm::vec4(1, 0, 0, 1))
	{
		if (m_playersTurn == 1)
		{
			m_PlayerOneColour = "Red";
			m_PlayerTwoColour = "Yellow";
		}
		else
		{
			m_PlayerOneColour = "Yellow";
			m_PlayerTwoColour = "Red";
		}
	}
	else if (m_sunk[0]->GetColor() == glm::vec4(1, 1, 0, 1))
	{
		if (m_playersTurn == 1)
		{
			m_PlayerOneColour = "Yellow";
			m_PlayerTwoColour = "Red";
		}
		else
		{
			m_PlayerOneColour = "Red";
			m_PlayerTwoColour = "Yellow";
		}
	}


}

glm::vec2 GameScene::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents
	worldPos.x *= 2.0f * 640 / getWindowWidth();
	worldPos.y *= 2.0f * 640 / ((16.f/9.f) * getWindowHeight());

	return worldPos;
}


float GameScene::DegreeToRadian(float degree)
{
	return degree * (PI / 180);
}
