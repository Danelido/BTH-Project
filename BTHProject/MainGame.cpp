#include "MainGame.h"
#include "Utility/Input.h"
#include "Utility/RandomNum.h"
#include "Vendor/ImGui/imgui.h"
#include "Vendor/ImGui/imgui_impl_opengl3.h"
#include "Vendor/ImGui/imgui_impl_glfw.h"
#include "App/AppSettings.h"

MainGame::MainGame()
{
	m_parser = new Parser();
	m_loader = new Loader();
	m_fpsCamera = new FPSCamera(glm::vec3(128.0f, 15.0f, 128.f));
	m_fpsCamera->initDebugMode();
	m_dbgCamera = new FPSCamera(glm::vec3(128.0f, 15.0f, 128.f));
	m_dbgCameraActive = false;
	m_activeCamera = m_fpsCamera;
	m_masterRenderer = new MasterRenderer(m_activeCamera);
	m_entityManager = new EntityManager();
	m_terrainChunkManager = new TerrainChunkManager();
	m_lightManager = new LightManager();
	m_lightManager->reserveVectorMemory(AppSettings::MAXLIGHTS());
	m_vSync = true;
	m_terrainWalk = false;
	m_gravity = -20.f;
	m_upAcceleration = 0.f;
	m_jumpForce = 9.0f;
	m_canJump = true;
	m_ignoreQuadtree = false;
	
	spawnObjects();
	
	m_entityManager->registerQuadtree(m_quadTree);
	m_masterRenderer->registerQuadTree(m_quadTree);
}

MainGame::~MainGame()
{
	delete m_parser;
	delete m_loader;
	delete m_masterRenderer;
	delete m_fpsCamera;
	delete m_entityManager;
	delete m_terrain;
	delete m_quadTree;
	delete m_dbgCamera;
	delete m_terrainChunkManager;
	delete m_lightManager;
}

void MainGame::spawnObjects()
{
	// Parse some data
	ParserData* boxData = m_parser->parseFile("Resources/Models/box.obj");
	ParserData* treeData = m_parser->parseFile("Resources/Models/tree.obj");

	// Choose if the entities is rendered with instancing or just regular ( 1 draw call per object )
#define instanced 1
#if instanced
	InstancedMesh* m_treeMesh = m_loader->createInstancedMesh(treeData);
#else
	Mesh* m_treeMesh = m_loader->createMesh(treeData);
#endif

	// Mesh for the sun
	Mesh* boxMesh = m_loader->createMesh(boxData);

	// Setup a sun ( Visualized with a box )
	m_sun = new Entity(boxMesh, glm::vec3(-1.f, 10.f, -1.f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f));
	m_sunMoveSpeed = 10.f;
	m_entityManager->add(m_sun);

	// Setup quadTree and terrain
	XYZ p(128.f, 12.f, 128.f);
	AABB boundary(p, 128.f);
	m_quadTree = new QuadTree(boundary, m_fpsCamera);
	m_terrain = new Terrain(m_loader, m_terrainChunkManager, m_quadTree);

	float maxDist = 251;

	// Entities
	for (int i = 0; i < 1000; i++)
	{
		float x = RandomNum::single(5.f, maxDist);
		float z = RandomNum::single(5.f, maxDist);
		float y = m_terrain->getHeight(glm::vec3(x, 0.f, z));
		//float y = 0.f;
		Entity* entity = new Entity(m_treeMesh,
			glm::vec3(x, y - 0.5f, z),
			glm::vec3(1.f, 1.f, 1.f),
			glm::vec3(0.f, 0.f, 0.f));

		m_entityManager->add(entity);
		m_quadTree->insert(entity);
	}

	//Lights
	for (int i = 0; i < AppSettings::MAXLIGHTS(); i++)
	{
		float x = RandomNum::single(5.f, maxDist);
		float z = RandomNum::single(5.f, maxDist);
		float y = m_terrain->getHeight(glm::vec3(x, 0.f, z)) + 3.f;

		Light* light = new Light(
			glm::vec3(x,y,z),
			RandomNum::vec3(0.f, 255.f, 0.f, 255.f, 0.f, 255.f) / 255.f,
			10.f);

		m_lightManager->addLight(light);
		m_quadTree->insert(light);
	}

}

void MainGame::update(float dt)
{
	if (m_dbgCameraActive){
		m_activeCamera = m_dbgCamera;
	}
	else{
		m_activeCamera = m_fpsCamera;
	}
	m_activeCamera->update(dt);
	m_activeCamera->setFreeLook(!m_terrainWalk);

	if (m_activeCamera->isActive())
		jumpFunc(dt);

	m_masterRenderer->changeCamera(m_activeCamera);

	queryTreeAndUpdateManagers(dt);
	moveSunFunc(dt);

	if(m_vSync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void MainGame::render()
{
	m_masterRenderer->render();
}

void MainGame::renderImGUI(float dt)
{
	ImGui::Begin("OpenGL");
	ImGui::Text("C = Lock/Unlock FPS camera");
	ImGui::Text("WASD = Move around");
	ImGui::Text("Space & CTRL = Move Up/Down");
	ImGui::Text("Average deltaT: %f", dt);
	ImGui::Text("Entities processed: %i", m_entityManager->entitiesProcessed());
	ImGui::Text("Number of lights: %i", m_lightManager->getNrOfLightsProcessed());
	ImGui::Text("Position: (%f, %f, %f)", m_fpsCamera->getPosition().x, m_fpsCamera->getPosition().y, m_fpsCamera->getPosition().z);
	ImGui::Text("Camera Up: (%f, %f, %f)", m_fpsCamera->getCameraUp().x, m_fpsCamera->getCameraUp().y, m_fpsCamera->getCameraUp().z);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Vsync", &m_vSync);
	ImGui::Checkbox("Terrain walking", &m_terrainWalk);
	ImGui::Checkbox("Switch to dbg camera", &m_dbgCameraActive);
	ImGui::Checkbox("Ignore quadtree", &m_ignoreQuadtree);
	ImGui::SliderFloat("JumpForce", &m_jumpForce, 0.00f, 20.0f);
	ImGui::SliderFloat("Gravity", &m_gravity, -20.f, 0.f);
	
	ImGui::End();

	ImGui::Render();
}

void MainGame::queryTreeAndUpdateManagers(float dt)
{
	std::vector<QuadTreeObject*> objects = m_quadTree->query();

	m_entityManager->setIgnoreQuadTree(m_ignoreQuadtree);
	m_terrainChunkManager->setIgnoreQuadTree(m_ignoreQuadtree);
	m_lightManager->setIgnoreQuadTree(m_ignoreQuadtree);

	m_entityManager->update(dt, m_activeCamera, m_masterRenderer, objects);
	m_terrainChunkManager->updateChunks(m_masterRenderer, objects);
	m_lightManager->update(m_masterRenderer, m_fpsCamera, objects);
}

void MainGame::jumpFunc(float dt)
{
	if (m_terrainWalk)
	{
		float cameraY = m_fpsCamera->getPosition().y;
		float targetY = m_terrain->getHeight(m_fpsCamera->getPosition()) + 2.f;

		cameraY += m_upAcceleration * dt;
		m_fpsCamera->setY(cameraY);
		m_upAcceleration += m_gravity * dt;

		if (cameraY < targetY)
		{
			m_fpsCamera->setY(targetY);
			m_canJump = true;
		}
	}
	else
	{
		m_canJump = false;
	}

	if (Input::isKeyPressed(GLFW_KEY_SPACE) && m_canJump)
	{
		m_canJump = false;
		m_upAcceleration = m_jumpForce;
	}

}

void MainGame::moveSunFunc(float dt)
{
	glm::vec3 pos = m_sun->getPosition();
	static float timer = 0.f;
	timer += dt * 0.2f;
	pos.x = (cosf(timer) * 260.f + 260.f) / 2.f;
	pos.z = (sinf(timer) * 260.f + 260.f) / 2.f;
	pos.y = 30.f;
	if (Input::isKeyHeldDown(GLFW_KEY_KP_8)){
		pos.x += m_sunMoveSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_KP_6)) {
		pos.z += m_sunMoveSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_KP_2)) {
		pos.x -= m_sunMoveSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_KP_4)) {
		pos.z -= m_sunMoveSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_KP_7)) {
		pos.y -= m_sunMoveSpeed * dt;
	}

	if (Input::isKeyHeldDown(GLFW_KEY_KP_9)) {
		pos.y += m_sunMoveSpeed * dt;
	}

	m_sun->setPosition(pos);
	m_masterRenderer->setSunPosition(pos);

}

