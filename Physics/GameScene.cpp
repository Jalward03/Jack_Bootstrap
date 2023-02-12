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
	//m_texture = new aie::Texture("../bin/textures/poolTable.png");

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);


	for (int i = 0; i < 7; i++)
	{
		Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(1, 1, 0, 1));
		m_yellows.push_back(circle);
		m_balls.push_back(circle);
	}
	for (int i = 0; i < 7; i++)
	{
		Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(1, 0, 0, 1));
		m_reds.push_back(circle);
		m_balls.push_back(circle);
	}

	m_whiteBall = new Circle(whiteStartPos, glm::vec2(0), 3.5f, 3.f, glm::vec4(1, 1, 1, 1));
	m_blackBall = new Circle(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(0, 0, 1, 1));


	SpawnTable();

	
	m_balls.push_back(m_blackBall);
	m_balls.push_back(m_whiteBall);

	for (auto ball : m_balls)
	{
		m_physicsScene->AddActor(ball);
	}

	SetBallPositions();

	m_playersTurn = 1;
	m_ballTypeAssigned = false;
	
	for (auto trigger : m_holes)
	{
		trigger->triggerEnter = [=](PhysicsObject* other)
		{
			if (other == m_whiteBall)
			{
				m_whiteBall->SetVelocity(glm::vec2(0));
				m_whiteBall->SetPosition(whiteStartPos);
			}
			
			for (auto red : m_reds)
			{
				if (other == red)
				{
					m_sunk.push_back(red);
				}
			}
			for (auto yellow : m_yellows)
			{
				if (other == yellow)
				{
					m_sunk.push_back(yellow);
				}
			}
			
		};
		trigger->triggerExit = [=](PhysicsObject* other)
		{
			std::cout << "lmao" << std::endl;
		};
	}
	
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

	if (input->isMouseButtonDown(0) && m_whiteBall->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		glm::vec2 whitePos = m_whiteBall->GetPosition();
		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		aie::Gizmos::add2DLine(whitePos, mouseToWhite * glm::vec2(700* 700), glm::vec4(1, 1, 1, 1));
	}

	if (input->wasMouseButtonReleased(0) && m_whiteBall->GetVelocity() == glm::vec2(0))
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		
		glm::vec2 whitePos = m_whiteBall->GetPosition();

		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		float forceMultiplier = glm::distance(ScreenToWorld(mousePos), whitePos) * 30;
		if (forceMultiplier > 800.f) forceMultiplier = 800.f;
		m_whiteBall->ApplyForce(mouseToWhite * glm::vec2(forceMultiplier), glm::vec2(0));

		m_playersTurn == 1 ? m_playersTurn = 2 : m_playersTurn = 1;
	}

	for (int i = 0; i < m_sunk.size(); i++)
	{
		m_sunk[i]->SetVelocity(glm::vec2(0));
		m_sunk[i]->SetPosition(glm::vec2(-90 + (i+1) * 6.f, 50));
	}
}

void GameScene::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	
	// begin drawing sprites
	m_2dRenderer->begin();
	//m_2dRenderer->drawSprite(m_texture, 640, 300, 1200, 580, 0, -1.1f);

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

void GameScene::SpawnTable()
{
	Circle* circle1 = new Circle(glm::vec2(0, 35), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));
	Circle* circle2 = new Circle(glm::vec2(89.f, 35), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));
	Circle* circle3 = new Circle(glm::vec2(-89.f, 35), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));
	Circle* circle4 = new Circle(glm::vec2(0,-45.5f), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));
	Circle* circle5 = new Circle(glm::vec2(89.f, -45.5f), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));
	Circle* circle6 = new Circle(glm::vec2(-89.f, -45.5f), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));
	Circle* circle7 = new Circle(glm::vec2(-89.f, -45.5f), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));
	Circle* circle8 = new Circle(glm::vec2(-89.f, -45.5f), glm::vec2(0), 4.f, 4.5f, glm::vec4(1, 1, 1, 1));

	m_holes.push_back(circle1);
	m_holes.push_back(circle2);
	m_holes.push_back(circle3);
	m_holes.push_back(circle4);
	m_holes.push_back(circle5);
	m_holes.push_back(circle6);
	m_holes.push_back(circle7);
	m_holes.push_back(circle8);

	for (auto circle : m_holes)
	{
		circle->SetTrigger(true);
		m_physicsScene->AddActor(circle);
		
	}

	Box* box1 = new Box(glm::vec2(-43.5f, 32.5f), 72, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* box2 = new Box(glm::vec2(43.5f, 32.5f), 72, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* box3 = new Box(glm::vec2(43.5f, -42.f), 72, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* box4 = new Box(glm::vec2(-43.5f, -42.f), 72, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* box5 = new Box(glm::vec2(-91.5f, -5.5), 6.f, 59, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* box6 = new Box(glm::vec2(91.5f, -5.5), 6.f, 59, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));

	Box* edge1 = new Box(glm::vec2(-8.5, 33.5), 6.f, 6.f, DegreeToRadian(60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge2 = new Box(glm::vec2(8.5, 33.5), 6.f, 6.f, DegreeToRadian(-60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge3 = new Box(glm::vec2(-78.5, 33.5), 6.f, 6.f, DegreeToRadian(-60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge4 = new Box(glm::vec2(78.5, 33.5), 6.f, 6.f, DegreeToRadian(60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge5 = new Box(glm::vec2(-8.5, -44), 6.f, 6.f, DegreeToRadian(-60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge6 = new Box(glm::vec2(8.5, -44), 6.f, 6.f, DegreeToRadian(60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge7 = new Box(glm::vec2(-78.5, -44), 6.f, 6.f, DegreeToRadian(60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge8 = new Box(glm::vec2(78.5, -44), 6.f, 6.f, DegreeToRadian(-60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge9 = new Box(glm::vec2(-92.5f, -36.f), 6.f, 6.f, DegreeToRadian(-30), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge10 = new Box(glm::vec2(-92.5f, 25.f), 6.f, 6.f, DegreeToRadian(30), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge11 = new Box(glm::vec2(92.5f, -36.f), 6.f, 6.f, DegreeToRadian(30), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));
	Box* edge12 = new Box(glm::vec2(92.5f, 25.f), 6.f, 6.f, DegreeToRadian(-30), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1));

	m_kinematics.push_back(box1);
	m_kinematics.push_back(box2);
	m_kinematics.push_back(box3);
	m_kinematics.push_back(box4);
	m_kinematics.push_back(box5);
	m_kinematics.push_back(box6);
	m_kinematics.push_back(edge1);
	m_kinematics.push_back(edge2);
	m_kinematics.push_back(edge3);
	m_kinematics.push_back(edge4);
	m_kinematics.push_back(edge5);
	m_kinematics.push_back(edge6);
	m_kinematics.push_back(edge7);
	m_kinematics.push_back(edge8);
	m_kinematics.push_back(edge9);
	m_kinematics.push_back(edge10);
	m_kinematics.push_back(edge11);
	m_kinematics.push_back(edge12);

	for (auto kinematic : m_kinematics)
	{
		kinematic->SetKinematic(true);
		m_physicsScene->AddActor(kinematic);
	}
}

glm::vec2 GameScene::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;

	// move the centre of the screen to (0,0)
	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	// scale according to our extents
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}


float GameScene::DegreeToRadian(float degree)
{
	return degree * (PI / 180);
}
