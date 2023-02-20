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
	m_font = new aie::Font("./font/consolas.ttf", 32);
	m_fontSmall = new aie::Font("./font/consolas.ttf", 16);
	m_texture = new aie::Texture("./textures/poolTable.png");
	m_whiteBallTexture = new aie::Texture("./textures/ball_16.png");
	m_indicatorTexture = new aie::Texture("./textures/moveIndicator.png");

	for (int i = 0; i < 15; i++)
	{

		std::string temp = "./textures/ball_";
		temp = temp.append(std::to_string(i + 1));
		temp = temp.append(".png");
		m_ballTextures.push_back(new aie::Texture(temp.c_str()));

	}

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, 0));
	m_physicsScene->SetTimeStep(0.01f);

	for (int i = 0; i < 7; i++)
	{
		PoolBall* circle = new PoolBall(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(0, 0, 0, 0));
		m_solids.push_back(circle);
		m_balls.push_back(circle);
	}

	for (int i = 0; i < 7; i++)
	{
		PoolBall* poolBall = new PoolBall(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(0, 0, 0, 0));
		m_stripes.push_back(poolBall);
		m_balls.push_back(poolBall);
	}



	m_whiteBall = new PoolBall(whiteStartPos, glm::vec2(0), 3.5f, 3.f, glm::vec4(0, 0, 0, 0));
	m_blackBall = new PoolBall(glm::vec2(0), glm::vec2(0), 4, 3, glm::vec4(0, 0, 0, 0));


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
			if (other == m_whiteBall && !m_whiteBallSunk)
			{
				m_whiteBallSunk = true;
				m_isWhiteInZone = false;
				m_whiteBall->SetTrigger(true);
				m_playersTurn == 1 ? m_playersTurn = 2 : m_playersTurn = 1;
				m_shotsleft = 0;
				m_shotsleft++;
				m_shotsleft++;
			}
			if (other == m_blackBall)
			{
				m_blackBall->SetVelocity(glm::vec2(0));
				m_blackBall->SetTrigger(true);
				if (m_playersTurn == 1)
				{
					if (m_canPlayerOneWin || !m_ballTypeAssigned)
					{
						m_winner = "Player 1 Wins!";
						m_gameWon = true;
						return;
						
					}
					else 
					{
						m_winner = "Player 2 Wins!";
						m_gameWon = true;
						return;
					}
					
				}
				else if (m_playersTurn == 2)
				{
					if (m_canPlayerTwoWin || !m_ballTypeAssigned)
					{
						m_winner = "Player 2 Wins!";
						m_gameWon = true;
						return;
						
					}
					else
					{
						m_winner = "Player 1 Wins!";
						m_gameWon = true;
						return;

					}
				}
			}

			for (auto solid : m_solids)
			{
				if (other == solid)
				{
					m_sunk.push_back(solid);
					if (m_playersTurn == 1)
					{
						if (m_PlayerOneColour == "Solid" || !m_ballTypeAssigned)
						{
							m_shotsleft++;
						}
					}

					else if (m_playersTurn == 2)
					{
						if (m_PlayerTwoColour == "Solid" || !m_ballTypeAssigned)
							m_shotsleft++;
						
					}
						
				}
			}
			for (auto stripe : m_stripes)
			{

				if (other == stripe)
				{
					m_sunk.push_back(stripe);
					if (m_playersTurn == 1)
					{
						if (m_PlayerOneColour == "Stripe" || !m_ballTypeAssigned)
						{
							m_shotsleft++;
						}
					}
						
					else if (m_playersTurn == 2)
					{
						if(m_PlayerTwoColour == "Stripe" || !m_ballTypeAssigned)
							m_shotsleft++;
					}
						
				}

			}

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
	for (auto sunk : m_sunk)
	{
		sunk->SetSunk(true);
		sunk->SetTrigger(true);
		sunk->SetKinematic(true);
		sunk->SetVelocity(glm::vec2(0));
		sunk->SetAngularVelocity(0);

	}
	 
	if (m_sunk.size() > 0 && !m_ballTypeAssigned)
	{
		AssignBallType();
	}
	aie::Input* input = aie::Input::getInstance();

	if (m_whiteBall->GetPosition().x > 41 && m_whiteBall->GetPosition().x < 78.8 && m_whiteBall->GetPosition().y < 22.33 && m_whiteBall->GetPosition().y > -41.25
		&& m_whiteBallSunk)
	{
		m_isWhiteInZone = true;
	}
	else
	{
		m_isWhiteInZone = false;
	}
	
	m_physicsScene->Update(deltaTime);


	if (m_sunkSolids.size() > 6)
	{
		if (m_PlayerOneColour == "Solid")
		{
			m_canPlayerOneWin = true;
		}
		else if (m_PlayerTwoColour == "Solid")
		{
			m_canPlayerTwoWin = true;
		}
	}

	if (m_sunkStripes.size() > 6)
	{
		if (m_PlayerOneColour == "Stripe")
		{
			m_canPlayerOneWin = true;
		}
		else if (m_PlayerTwoColour == "Stripe")
		{
			m_canPlayerTwoWin = true;
		}
	}

	if (m_whiteBallSunk)
	{
		
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		m_whiteBall->SetPosition(ScreenToWorld(mousePos));

		if (input->wasMouseButtonPressed(1) && m_isWhiteInZone)
		{
			m_whiteBallSunk = false;
			m_isWhiteInZone = false;
			m_whiteBall->SetPosition(ScreenToWorld(mousePos));
			m_whiteBall->SetTrigger(false);
			m_whiteBall->SetVelocity(glm::vec2(0));
			 
		}
	}
	
	if (input->isMouseButtonDown(0) && m_whiteBall->GetVelocity() == glm::vec2(0) && !m_whiteBallSunk && !m_gameWon)
	{
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());
		glm::vec2 whitePos = m_whiteBall->GetPosition();
		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));
		float forceMultiplier = glm::distance(ScreenToWorld(mousePos), whitePos) * 20;
		if (forceMultiplier > 1000) forceMultiplier = 1000;

		glm::vec4 colour;
		if (forceMultiplier < 500) colour = glm::vec4(1, 1, 0, 1);
		else if (forceMultiplier > 500 && forceMultiplier < 1000) colour = glm::vec4(1, 0.647, 0, 1);
		else if (forceMultiplier == 1000) colour = glm::vec4(1, 0, 0, 1);
		aie::Gizmos::add2DLine(whitePos, mouseToWhite * glm::vec2(700 * 700), colour);




	}

	if (input->wasMouseButtonReleased(0) && m_whiteBall->GetVelocity() == glm::vec2(0)&& !m_whiteBallSunk && !m_gameWon)
	{
		m_firstShotTaken = true;
		m_shotsleft--;
		glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());

		glm::vec2 whitePos = m_whiteBall->GetPosition();

		glm::vec2 mouseToWhite = glm::normalize(whitePos - ScreenToWorld(mousePos));

		float forceMultiplier = glm::distance(ScreenToWorld(mousePos), whitePos) * 20;
		if (forceMultiplier > 1000) forceMultiplier = 1000;
		m_whiteBall->ApplyForce(mouseToWhite * glm::vec2(forceMultiplier), glm::vec2(0));

	}
	
	if (m_whiteBall->GetVelocity() == glm::vec2(0) && m_shotsleft == 0)
	{
		m_playersTurn == 1 ? m_playersTurn = 2 : m_playersTurn = 1;
		m_shotsleft++;
	}

	for (int i = 0; i < m_sunk.size(); i++)
	{
		if (m_sunk[i]->GetSunk())
		{
			for (auto solid : m_solids)
			{
				if (solid == m_sunk[i] && !solid->GetSunkTriggered())
				{
					m_sunkSolids.push_back(solid);
					solid->SetSunkTriggered(true);
				}
			}

			for (auto stripe : m_stripes)
			{
				if (stripe == m_sunk[i] && !stripe->GetSunkTriggered())
				{

					m_sunkStripes.push_back(stripe);
					stripe->SetSunkTriggered(true);
				}
			}
		}

	}


	for (int i = 0; i < m_sunkSolids.size(); i++)
	{

		if (m_PlayerOneColour == "Solid")
		{
			m_sunkSolids[i]->SetPosition(glm::vec2(-95 + i * 6.f, 42));

		}
		else if (m_PlayerTwoColour == "Solid")
		{
			m_sunkSolids[i]->SetPosition(glm::vec2(95 - i * 6.f, 42));

		}


	}

	for (int i = 0; i < m_sunkStripes.size(); i++)
	{
		if (m_PlayerOneColour == "Stripe")
		{
			m_sunkStripes[i]->SetPosition(glm::vec2(-95 + i * 6.f, 42));


		}
		else if (m_PlayerTwoColour == "Stripe")
		{
			m_sunkStripes[i]->SetPosition(glm::vec2(95 - i * 6.f, 42));


		}

	}

}

void GameScene::draw()
{
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

	if (m_gameWon)
	{
		m_2dRenderer->drawText(m_font, m_winner.c_str(), 520, 680, 1);

	}

	if (m_ballTypeAssigned)
	{
		m_2dRenderer->drawText(m_fontSmall, m_PlayerOneColour.c_str(), 10, 655, 1);
		m_2dRenderer->drawText(m_fontSmall, m_PlayerTwoColour.c_str(), 1220, 655, 1);

	}
	if (m_whiteBallSunk)
	{
		std::string toolTip = "Place Behind Line With RMB";

		m_2dRenderer->drawText(m_fontSmall, toolTip.c_str(),
			WorldToScreen(m_whiteBall->GetPosition()).x + 15,
			WorldToScreen(m_whiteBall->GetPosition()).y + 15, 1);
	}

	if (!m_firstShotTaken)
	{
		std::string toolTip = "Hold LMB To Aim & Set Power";

		m_2dRenderer->drawText(m_fontSmall, toolTip.c_str(),
			WorldToScreen(m_whiteBall->GetPosition()).x + 15,
			WorldToScreen(m_whiteBall->GetPosition()).y + 15, 1);
	}

	if (m_playersTurn == 1) m_2dRenderer->drawSprite(m_indicatorTexture, 190, 690, 30, 30, 0, 1);
	else m_2dRenderer->drawSprite(m_indicatorTexture, 1075, 690, 30, 30, 0, 1);


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
		if (solid == m_sunk[0])
		{
			if (m_playersTurn == 1)
			{
				m_PlayerOneColour = "Solid";
				m_PlayerTwoColour = "Stripe";
			}
			else
			{
				m_PlayerOneColour = "Stripe";
				m_PlayerTwoColour = "Solid";
			}

		}
	}

	for (auto stripe : m_stripes)
	{
		if (stripe == m_sunk[0])
		{

			if (m_playersTurn == 1)
			{
				m_PlayerOneColour = "Stripe";
				m_PlayerTwoColour = "Solid";
			}
			else
			{
				m_PlayerOneColour = "Solid";
				m_PlayerTwoColour = "Stripe";
			}
		}
	}



}

void GameScene::SpawnTable()
{


	m_holes.push_back(new Circle(glm::vec2(0, 31), glm::vec2(0), 4.f, 2.f, glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(83.f, 27), glm::vec2(0), 4.f, 2.f, glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(-83.f, 27), glm::vec2(0), 4.f, 2.f, glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(0, -50.f), glm::vec2(0), 4.f, 2.f, glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(83.f, -45.5f), glm::vec2(0), 4.f, 2.f, glm::vec4(0, 0, 0, 0)));
	m_holes.push_back(new Circle(glm::vec2(-84.f, -46.f), glm::vec2(0), 4.f, 2.f, glm::vec4(0, 0, 0, 0)));


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

	m_whiteBallZone = new Box(glm::vec2(60, -9.5), 32.5f, 60.f, 0, glm::vec2(0), 4.0f, glm::vec4(0));
	m_whiteBallZone->SetTrigger(true);
	m_whiteBallZone->SetKinematic(true);
	m_physicsScene->AddActor(m_whiteBallZone);

}

glm::vec2 GameScene::ScreenToWorld(glm::vec2 screenPos)
{
	glm::vec2 worldPos = screenPos;


	worldPos.x -= getWindowWidth() / 2;
	worldPos.y -= getWindowHeight() / 2;

	
	worldPos.x *= 2.0f * m_extents / getWindowWidth();
	worldPos.y *= 2.0f * m_extents / (m_aspectRatio * getWindowHeight());

	return worldPos;
}

glm::vec2 GameScene::WorldToScreen(glm::vec2 worldPos)
{
	glm::vec2 screenPos = worldPos;


	screenPos.x /= 2.0f;
	screenPos.x /= m_extents;
	screenPos.x *= getWindowWidth();
	screenPos.y /= 2.0f;
	screenPos.y /= m_extents;
	screenPos.y *= (m_aspectRatio * getWindowHeight());


	screenPos.x += getWindowWidth() / 2;
	screenPos.y += getWindowHeight() / 2;

	return screenPos;
}

float GameScene::DegreeToRadian(float degree)
{
	return degree * (PI / 180);
}
