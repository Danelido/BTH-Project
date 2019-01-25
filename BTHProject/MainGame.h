#ifndef _MAINGAME_H
#define _MAINGAME_H
#include "Parser/Parser.h"
#include "Graphics/Loader.h"
#include "Entity/Entity.h"
#include "Renderers/MasterRenderer.h"
#include "Entity/EntityManager.h"
#include "Utility/FPSCamera.h"
#include "Entity/Light.h"
#include "Terrain/Terrain.h"

class MainGame
{
public:
	MainGame();
	~MainGame();

	void update(float dt);
	void render();
	void renderImGUI(float dt);
private:
	void jumpFunc(float dt);
	void moveSunFunc(float dt);
private:
	Loader* m_loader;
	Parser* m_parser;
	MasterRenderer* m_MasterRenderer;
	FPSCamera * m_fpsCamera;
	EntityManager* m_entityManager;
	Terrain* m_terrain;

private:
	float m_temp_processingDistance;
	bool m_vSync;

	// Sun
	Entity* m_sun;
	float m_sunMoveSpeed;

	// Terrain walking/jumping
	bool m_terrainWalk;
	float m_gravity;
	float m_upAcceleration;
	float m_jumpForce;
	bool m_canJump;
};

#endif

