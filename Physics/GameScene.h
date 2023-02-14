#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <vector>

#include "Input.h"
#include "Circle.h"
#include "Box.h"
#include "Rigidbody.h"
#include "PoolBall.h"
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
	void PlaceWhiteBall();
 
	glm::vec2 ScreenToWorld(glm::vec2 screenPos);
	glm::vec2 WorldToScreen(glm::vec2 worldPos);

protected:

	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;
	aie::Font* m_fontSmall;
	aie::Texture* m_texture;
	aie::Texture* m_whiteBallTexture;
	aie::Texture* m_indicatorTexture;
	
	PhysicsScene* m_physicsScene;

	std::vector<PhysicsObject*> m_objects;

	const float m_extents = 100;
	const float m_aspectRatio = 16.0f / 9.0f;
	glm::vec2 whiteStartPos = glm::vec2(41, -9);

public:
	PoolBall* m_whiteBall;
	PoolBall* m_blackBall;
	Circle* m_circle;
	Box* m_whiteBallZone;
	std::vector<Circle*> m_holes;
	std::vector<aie::Texture*> m_ballTextures;
	std::vector<PoolBall*> m_balls;
	std::vector<PoolBall*> m_stripes;
	std::vector<PoolBall*> m_solids;
	std::vector<PoolBall*> m_sunk;
	std::vector<PoolBall*> m_sunkSolids;
	std::vector<PoolBall*> m_sunkStripes;
	std::vector<Box*> m_kinematics;
	float DegreeToRadian(float degree);

	std::string m_PlayerOneColour = "";
	std::string m_PlayerTwoColour = "";
	std::string m_winner = "";
	int m_shotsleft = 1;
	int m_playersTurn;

	bool m_ballTypeAssigned;
	bool m_readyToShoot;
	bool m_changeTurn = false;
	bool m_canPlayerOneWin = false;
	bool m_canPlayerTwoWin = false;
	bool m_gameWon = false;
	bool m_whiteBallSunk = false;
	bool m_isWhiteInZone;
	bool m_firstShotTaken = false;
	
};