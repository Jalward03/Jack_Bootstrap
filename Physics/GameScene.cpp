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
	m_texture = new aie::Texture("../bin/textures/poolTable.png");
	m_whiteBallTexture = new aie::Texture("../bin/textures/ball_16.png");
	for (int i = 0; i < 15; i++)
	{
		
		std::string temp = "../bin/textures/ball_";
		temp = temp.append(std::to_string(i + 1));
		temp = temp.append(".png");
		m_ballTextures.push_back(new aie::Texture(temp.c_str()));
		
	}

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);


	for (int i = 0; i < 7; i++)
	{
		Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(0, 0, 0, 0));
		m_stripes.push_back(circle);
		m_balls.push_back(circle);
	}
	for (int i = 0; i < 7; i++)
	{
		Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(0, 0, 0, 0));
		m_solids.push_back(circle);
		m_balls.push_back(circle);
	}


	m_whiteBall = new Circle(whiteStartPos, glm::vec2(0), 3.5f, 3.f, glm::vec4(0, 0, 0, 0));
	m_blackBall = new Circle(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(0, 0, 0, 0));


	
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
			
			for (auto solid : m_solids)
			{
				if (other == solid)
				{
					m_sunk.push_back(solid);
				}
			}
			for (auto stripe : m_stripes)
			{
				if (other == stripe)
				{
					m_sunk.push_back(stripe);
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


	m_physicsScene->Update(deltaTime);
	if (m_sunk.size() > 0 && !m_ballTypeAssigned)
	{
		AssignBallType();
	}

	if (m_whiteBall->GetVelocity() == glm::vec2(0))
	{
		m_readyToShoot = true;
		
	}
	else
	{
		m_readyToShoot = false;
	}

	if (m_readyToShoot && !m_changeTurn)
	{
		m_changeTurn;
	}
	

	if (input->isMouseButtonDown(0) && m_readyToShoot)
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		glm::vec2 whitePos = m_whiteBall->GetPosition();
		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		aie::Gizmos::add2DLine(whitePos, mouseToWhite * glm::vec2(700* 700), glm::vec4(1, 1, 1, 1));
	}

	if (input->wasMouseButtonReleased(0) && m_readyToShoot)
	{

		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		
		glm::vec2 whitePos = m_whiteBall->GetPosition();

		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		float forceMultiplier = glm::distance(ScreenToWorld(mousePos), whitePos) * 30;
		if (forceMultiplier > 800.f) forceMultiplier = 800.f;
		m_whiteBall->ApplyForce(mouseToWhite * glm::vec2(forceMultiplier), glm::vec2(0));

	}

	if (m_readyToShoot && m_changeTurn)
	{
		m_playersTurn == 1 ? m_playersTurn = 2 : m_playersTurn = 1;
		m_changeTurn = false;

	}

	if (input->wasKeyPressed(aie::INPUT_KEY_F)) 
	{
		std::cout << m_playersTurn << std::endl;
	}
	for (int i = 0; i < m_sunk.size(); i++)
	{
		m_sunk[i]->SetTrigger(true);
		m_sunk[i]->SetVelocity(glm::vec2(0));
		m_sunk[i]->SetPosition(glm::vec2(-90 + i * 6.f, 35));
	}

}

void GameScene::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	
	m_2dRenderer->begin();


	m_2dRenderer->drawSprite(m_texture, 640, 300, 1200, 580, 0, 1);

	float x = WorldToScreen(m_whiteBall->GetPosition()).x;
	float y = WorldToScreen(m_whiteBall->GetPosition()).y;

	m_2dRenderer->drawSprite(m_whiteBallTexture, x, y, 40, 40, 0, 1);


	for (int i = 0; i < m_ballTextures.size(); i++)
	{
		m_2dRenderer->drawSprite(m_ballTextures[i],
			WorldToScreen(m_balls[i]->GetPosition()).x, 
			WorldToScreen(m_balls[i]->GetPosition()).y,
			40, 40, 0, 1);
	}
	static float aspectRatio = 16.f / 9.f;
	
	m_physicsScene->Draw();

	m_2dRenderer->drawText(m_font, "Player 1", 10, 680, 1);
	m_2dRenderer->drawText(m_font, "Player 2", 1120, 680, 1);



	m_2dRenderer->end();
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));
	aie::Gizmos::clear();

	
}

void GameScene::SetBallPositions()
{
	// first row
	m_balls[0]->SetPosition(glm::vec2(-25, -9));
	// second row
	m_balls[2]->SetPosition(glm::vec2(-33, -4.9));
	m_balls[10]->SetPosition(glm::vec2(-33, -13.1));
	// third row
	m_balls[14]->SetPosition(glm::vec2(-41, -9.2));
	m_balls[3]->SetPosition(glm::vec2(-41, -17.2));
	m_balls[9]->SetPosition(glm::vec2(-41, -0.8));
	// fourth row							 
	m_balls[6]->SetPosition(glm::vec2(-49, 3.3));
	m_balls[11]->SetPosition(glm::vec2(-49, -4.9));
	m_balls[5]->SetPosition(glm::vec2(-49, -13.1));
	m_balls[8]->SetPosition(glm::vec2(-49, -21.3));
	// fifth row							 
	m_balls[12]->SetPosition(glm::vec2(-57, 7.4));
	m_balls[7]->SetPosition(glm::vec2(-57, -17.2));
	m_balls[13]->SetPosition(glm::vec2(-57, -9));
	m_balls[1]->SetPosition(glm::vec2(-57, -0.8));
	m_balls[4]->SetPosition(glm::vec2(-57, -25.4));
}

void GameScene::AssignBallType()
{
	m_ballTypeAssigned = true;
	for (auto solid : m_solids)
	{
		if (m_sunk[0] == solid)
		{
			if (m_playersTurn == 1)
			{
				m_PlayerOneColour = "Solid";
				m_PlayerTwoColour = "Stripes";
			}
			else
			{
				m_PlayerOneColour = "Stripes";
				m_PlayerTwoColour = "Solid";
			}

		}
	}

	for (auto stripe : m_stripes)
	{
		if (m_sunk[0] == stripe)
		{

			if (m_playersTurn == 1)
			{
				m_PlayerOneColour = "Stripes";
				m_PlayerTwoColour = "Solids";
			}
			else
			{
				m_PlayerOneColour = "Solids";
				m_PlayerTwoColour = "Stripes";
			}
		}
	}

}

void GameScene::SpawnTable()
{
	

	m_holes.push_back(new Circle(glm::vec2(0, 31), glm::vec2(0), 4.f, 2.f,         glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(83.f, 27), glm::vec2(0), 4.f, 2.f,      glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(-83.f, 27), glm::vec2(0), 4.f, 2.f,     glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(0, -50.f), glm::vec2(0), 4.f, 2.f,      glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(83.f, -45.5f), glm::vec2(0), 4.f, 2.f,  glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(-84.f, -46.f), glm::vec2(0), 4.f, 2.f,  glm::vec4(0, 0, 0, 0)));


	for (auto circle : m_holes)
	{
		circle->SetColour(glm::vec4(0));
		circle->SetTrigger(true);
		m_physicsScene->AddActor(circle);

	}

	m_kinematics.push_back(new Box(glm::vec2(-41.5f, 28.5), 70, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(41.5f, 28.5), 70, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(-41.f, -47.5f), 70, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(40.f, -47.5f), 68.25f, 6.f, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(-84.8f, -9.5), 6.f, 59, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(84.8f, -9.5), 6.f, 59.25, 0.f, glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));

	// corners
	m_kinematics.push_back(new Box(glm::vec2(-7.5, 29.5), 6.f, 6.f, DegreeToRadian(63), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(7.5, 29.5), 6.f, 6.f, DegreeToRadian(-63), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(75.5, 29.5), 6.f, 6.f, DegreeToRadian(60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(86.f, 19), 6.f, 6.f, DegreeToRadian(-65), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(86.f, -37.5f), 6.f, 6.f, DegreeToRadian(65), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(74.5f, -48.7f), 6.f, 6.f, DegreeToRadian(45), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(-7.5, -48.5f), 6.f, 6.f, DegreeToRadian(-65), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(7.5, -48.5f), 6.f, 6.f, DegreeToRadian(65), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(-74.5f, -48.5f), 6.f, 6.f, DegreeToRadian(65), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(-86.f, 18.75f), 6.f, 6.f, DegreeToRadian(65), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(-86.f, -38.f), 6.f, 6.f, DegreeToRadian(-65), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));
	m_kinematics.push_back(new Box(glm::vec2(-75.5, 29.5), 6.f, 6.f, DegreeToRadian(-60), glm::vec2(0), 4.0f, glm::vec4(1, 0, 1, 1)));

	for (auto kinematic : m_kinematics)
	{
		kinematic->SetKinematic(true);
		m_physicsScene->AddActor(kinematic);
		kinematic->SetColour(glm::vec4(0));
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

glm::vec2 GameScene::WorldToScreen(glm::vec2 worldPos)
{
	glm::vec2 screenPos = worldPos;


	// scale according to our extents
	screenPos.x /= 2.0f;
	screenPos.x /= m_extents;
	screenPos.x *= getWindowWidth();
	screenPos.y /= 2.0f;
	screenPos.y /= m_extents;
	screenPos.y *= (m_aspectRatio * getWindowHeight());

	// move the centre of the screen to (0,0)
	screenPos.x += getWindowWidth() / 2;
	screenPos.y += getWindowHeight() / 2;

	return screenPos;
}


float GameScene::DegreeToRadian(float degree)
{
	return degree * (PI / 180);
}
