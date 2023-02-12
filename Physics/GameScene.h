#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <vector>

#include "Input.h"
#include "Circle.h"
#include "Box.h"
#include "Rigidbody.h"
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

	void SpawnTable();
	void SetBallPositions();
	void AssignBallType();

	glm::vec2 ScreenToWorld(glm::vec2 screenPos);

protected:

	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;
	aie::Texture* m_texture;

	PhysicsScene* m_physicsScene;

	std::vector<PhysicsObject*> m_objects;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;
	glm::vec2 whiteStartPos = glm::vec2(50, 0);

public:
	Circle* m_whiteBall;
	Circle* m_blackBall;
	Circle* m_circle;
	std::vector<Circle*> m_balls;
	std::vector<Circle*> m_yellows;
	std::vector<Circle*> m_reds;
	std::vector<Circle*> m_sunk;
	std::vector<Box*> m_kinematics;
	std::vector<Circle*> m_holes;
	float DegreeToRadian(float degree);

	std::string m_PlayerOneColour = "";
	std::string m_PlayerTwoColour = "";

	int m_playersTurn;
	bool m_ballTypeAssigned;
};