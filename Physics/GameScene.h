#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <vector>

#include "Input.h"
#include "Circle.h"
#include <string>

class PhysicsScene;
class PhysicsObject;

class GameScene : public aie::Application {
public:

	GameScene();
	virtual ~GameScene();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();


	void SetBallPositions();
	void AssignBallType();

	glm::vec2 ScreenToWorld(glm::vec2 screenPos);

protected:

	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;

	PhysicsScene* m_physicsScene;

	std::vector<PhysicsObject*> m_objects;

public:
	Circle* m_whiteBall;
	Circle* m_blackBall;
	std::vector<Circle*> m_balls;
	std::vector<Circle*> m_yellows;
	std::vector<Circle*> m_reds;
	std::vector<Circle*> m_sunk;
	float DegreeToRadian(float degree);

	std::string m_PlayerOneColour = "";
	std::string m_PlayerTwoColour = "";

	int m_playersTurn;
	bool m_ballTypeAssigned;
};