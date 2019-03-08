#ifndef _MAINGAME_H
#define _MAINGAME_H
#include "Parser/Parser.h"
#include "Graphics/Loader.h"
#include "Entity/Entity.h"
#include "Renderers/MasterRenderer.h"
#include "Entity/EntityManager.h"
#include "Utility/FPSCamera.h"
#include "Light/LightManager.h"
#include "Terrain/Terrain.h"
#include "Acceleration/QuadTree.h"
#include "Terrain/TerrainChunkManager.h"
#include "Particle/ParticleManager.h"
#include "Utility/MousePicking.h"


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
	ParticleManager* m_particleManager;
	EntityManager* m_entityManager;
	TerrainChunkManager* m_terrainChunkManager;
	Terrain* m_terrain;
	LightManager* m_lightManager;
	QuadTree* m_quadTree;
	MousePicking* m_mousePicking;

private:
	//GPU info
	GLint m_total_mem_kb = 0;
	GLint m_cur_avail_mem_kb = 0;

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

	// ImGui
	bool m_infoTab = true;
	bool m_controlsTab = false;
};

#endif

