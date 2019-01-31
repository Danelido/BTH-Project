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
#include "Acceleration/QuadTree.h"
#include "Terrain/TerrainChunkManager.h"

class MainGame
{
public:
	MainGame();
	~MainGame();

	void update(float dt);
	void render();
	void renderImGUI(float dt);
private:
	void queryTreeAndUpdateManagers(float dt);
	void jumpFunc(float dt);
	void moveSunFunc(float dt);
	void spawnObjects();
private:
	Loader* m_loader;
	Parser* m_parser;
	MasterRenderer* m_masterRenderer;
	FPSCamera * m_fpsCamera;
	FPSCamera* m_dbgCamera;
	FPSCamera* m_activeCamera; // Just points to the active camera
	EntityManager* m_entityManager;
	TerrainChunkManager* m_terrainChunkManager;
	Terrain* m_terrain;
	QuadTree* m_quadTree;

private:
	bool m_vSync;
	bool m_dbgCameraActive;
	bool m_ignoreQuadtree;

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

