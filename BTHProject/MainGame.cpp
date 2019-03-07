#include "MainGame.h"
#include "Utility/Input.h"
#include "Utility/RandomNum.h"
#include "Vendor/ImGui/imgui.h"
#include "Vendor/ImGui/imgui_impl_opengl3.h"
#include "Vendor/ImGui/imgui_impl_glfw.h"
#include "App/AppSettings.h"

#include <iostream>

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

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
	m_particleManager = new ParticleManager(m_fpsCamera->getProjectionMatrix());
	m_mousePicking = new MousePicking(m_activeCamera);
	m_lightManager->reserveVectorMemory(AppSettings::MAXLIGHTS());
	m_vSync = true;
	m_terrainWalk = false;
	m_gravity = -20.f;
	m_upAcceleration = 0.f;
	m_jumpForce = 9.0f;
	m_canJump = true;
	m_ignoreQuadtree = false;
	m_shadowBiaz = 0.005f;
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
	delete m_sun;
	delete m_particleManager;
	delete m_mousePicking;
}

void MainGame::spawnObjects()
{
	// Parse some data
	ParserData* boxData = m_parser->parseFile("Resources/Models/box.obj");
	ParserData* treeData = m_parser->parseFile("Resources/Models/tree.obj");
	ParserData* sphereData = m_parser->parseFile("Resources/Models/sphere.obj");

	// Mesh
	Mesh* boxMesh = m_loader->createMesh(boxData);
	Mesh* sphereMesh = m_loader->createMesh(sphereData);

	// Setup a sun ( Visualized with a box )
	m_sun = new Entity(boxMesh, glm::vec3(128.f, 16.f, 255.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f));
	m_sunMoveSpeed = 10.f;
	
	// Setup quadTree and terrain
	XYZ p(128.f, 12.f, 128.f);
	AABB boundary(p, 128.f);
	m_quadTree = new QuadTree(boundary, m_fpsCamera);
	m_terrain = new Terrain(m_loader, m_terrainChunkManager, m_quadTree);

	Entity* boat = new Entity(sphereMesh, glm::vec3(128.f, 5.f, 225.f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.f, 0.f, 0.f));
	//boat->setBoundaryManually(8.f, 10.f, 10.f);
	m_entityManager->add(boat);
	m_quadTree->insert(boat);
	float maxDist = 251;

	// Entites in the shadow mapping area	
	Entity* entity1 = new Entity(boxMesh,
		glm::vec3(123.f, 2.5f, 250.f),
		glm::vec3(1.f, 1.f, 1.f),
		glm::vec3(25.f, 0.f, 60.f));
	m_entityManager->add(entity1);
	m_quadTree->insert(entity1);
	

	Entity* entity2 = new Entity(boxMesh,
		glm::vec3(133.f, 3.f, 250.f),
		glm::vec3(1.2f, 0.75f, 1.f),
		glm::vec3(25.f, 150.f, 60.f));
	m_entityManager->add(entity2);
	m_quadTree->insert(entity2);


	Entity* entity3 = new Entity(boxMesh,
		glm::vec3(123.f, 2.5f, 245.f),
		glm::vec3(0.3f, 1.2f, 1.3f),
		glm::vec3(125.f, 10.f, 160.f));
	m_entityManager->add(entity3);
	m_quadTree->insert(entity3);


	Entity* entity4 = new Entity(boxMesh,
		glm::vec3(133.f, 1.5f, 248.f),
		glm::vec3(.5f, .5f, .5f),
		glm::vec3(50.f, 60.f, 110.f));
	m_entityManager->add(entity4);
	m_quadTree->insert(entity4);
	//-------------------------------------------

	//Lights
	for (int i = 0; i < 50; i++)
	{
		float x = RandomNum::single(5.f, maxDist);
		float z = RandomNum::single(5.f, maxDist);
		float y = m_terrain->getHeight(glm::vec3(x, 0.f, z)) + 3.f;
		//float y = 0;
		Light* light = new Light(
			glm::vec3(x,y,z),
			RandomNum::vec3(0.f, 255.f, 0.f, 255.f, 0.f, 255.f) / 255.f,
			8.f);

		m_lightManager->addLight(light);
		m_quadTree->insert(light);
	}

}
float t = 0.f;
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

	m_masterRenderer->setShadowBiaz(m_shadowBiaz);

	m_particleManager->update(dt);

	t += dt;
	if (t >= 0.1f)
	{
		t = 0.f;
		m_particleManager->spawnParticle(
			glm::vec3(90.f, 25.f, 230.f),
			glm::vec3(RandomNum::single(-5.f, 5.f) / 10.f, 0.2f, RandomNum::single(-5.f, 5.f) / 10.f),
			1.5f,
			glm::vec4(
				RandomNum::single(0.f, 255.f) / 255.f,
				RandomNum::single(0.f, 255.f) / 255.f,
				RandomNum::single(0.f, 255.f) / 255.f,
				1.0f)
		);
	}

	
	
	
}

void MainGame::render()
{
	m_masterRenderer->render();
	m_particleManager->render(m_activeCamera);
}

void MainGame::renderImGUI(float dt)
{
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX,
		&m_total_mem_kb);

	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX,
		&m_cur_avail_mem_kb);

	ImGui::Begin("GL");

	if (ImGui::Button("Info", ImVec2(90.f, 20.f)))
	{
		m_infoTab = true;
		m_controlsTab = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Controls", ImVec2(90.f, 20.f)))
	{
		m_infoTab = false;
		m_controlsTab = true;
	}

	float fps = ImGui::GetIO().Framerate;

	if (fps < 30.f)
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.f, 1.0f), "FPS: %.1f", fps);
	else if (fps >= 30.0f && fps < 55.0f)
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.f, 1.0f), "FPS: %.1f", fps);
	else
		ImGui::TextColored(ImVec4(0.f, 1.0f, 0.f, 1.0f), "FPS: %.1f", fps);


	if (m_infoTab)
	{
		ImGui::TextColored(ImVec4(204.f / 255.f, 102.f / 255.f, 0.f, 1.0f),
			"Total GPU Memory: %i%s\n%s%i%s\n%s%i%s", m_total_mem_kb / 1024, " MB", "Available GPU Memory: ", m_cur_avail_mem_kb / 1024, " MB",
			"Used GPU Memory: ", (m_total_mem_kb / 1024) - (m_cur_avail_mem_kb / 1024), " MB");

		ImGui::Text("Frame average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("Average delta time: %f", dt);
		ImGui::Text("Entities processed: %i", m_entityManager->entitiesProcessed());
		ImGui::Text("Lights processed: %i", m_lightManager->getNrOfLightsProcessed());
		ImGui::Text("Particles: %i", m_particleManager->nrOfParticles());
		ImGui::Text("Position: (%f, %f, %f)", m_fpsCamera->getPosition().x, m_fpsCamera->getPosition().y, m_fpsCamera->getPosition().z);
		ImGui::Text("Sun position: (%f, %f, %f)",m_sun->getPosition().x, m_sun->getPosition().y, m_sun->getPosition().z);
		ImGui::Text("Camera Up: (%f, %f, %f)", m_fpsCamera->getCameraUp().x, m_fpsCamera->getCameraUp().y, m_fpsCamera->getCameraUp().z);
		ImGui::Text("C = Lock/Unlock camera");
		ImGui::Text("WASD = Move around");
		ImGui::Text("Space & CTRL = Move Up/Down");
	}

	if (m_controlsTab) 
	{
		ImGui::Checkbox("Vsync", &m_vSync);
		ImGui::Checkbox("Terrain walking", &m_terrainWalk);
		ImGui::Checkbox("Switch to dbg camera", &m_dbgCameraActive);
		ImGui::Checkbox("Ignore quadtree", &m_ignoreQuadtree);
		ImGui::SliderFloat("JumpForce", &m_jumpForce, 0.00f, 20.0f);
		ImGui::SliderFloat("Gravity", &m_gravity, -20.f, 0.f);
		ImGui::SliderFloat("Shadow biaz", &m_shadowBiaz, 0.0001f, 1.0f);
	}
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

	if (Input::isMousePressed(GLFW_MOUSE_BUTTON_1) && !m_activeCamera->isActive())
	{
		glm::vec3 ray = m_mousePicking->getRay();
		m_entityManager->checkMousePicking(m_activeCamera->getPosition(), ray, 200.f, objects);
		
		std::cout << "RayDirection: (" << ray.x << ", " << ray.y << ", " << ray.z << ")" << "\n";
	}

}

void MainGame::jumpFunc(float dt)
{
	if (m_terrainWalk && m_terrain)
	{
		float cameraY = m_fpsCamera->getPosition().y;
		float targetY = m_terrain->getHeight(m_fpsCamera->getPosition()) + 3.f;

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
	//pos.x = (cosf(timer) * 260.f + 260.f) / 2.f;
	//pos.z = (sinf(timer) * 260.f + 260.f) / 2.f;
	//pos.y = 30.f;
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

