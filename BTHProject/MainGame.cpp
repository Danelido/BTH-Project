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
	m_fpsCamera = new FPSCamera(glm::vec3(55.0f, 10.0f, 50.f));
	m_MasterRenderer = new MasterRenderer(m_fpsCamera);
	m_entityManager = new EntityManager();
	m_temp_processingDistance = 250.f;
	m_vSync = true;
	m_terrainWalk = false;
	m_gravity = -.250f;
	m_entityManager->setMaxProcessingDistance(m_temp_processingDistance);
	m_upAcceleration = 0.f;
	m_jumpForce = .150f;
	m_canJump = true;
	ParserData* boxData = m_parser->parseFile("Resources/Models/box.obj");
	ParserData* treeData = m_parser->parseFile("Resources/Models/tree.obj");
	
	m_terrain = new Terrain(m_loader);
	m_MasterRenderer->submitTerrain(m_terrain);

#define instanced 1
#if instanced
	InstancedMesh* m_treeMesh = m_loader->createInstancedMesh(treeData);
#else
	Mesh* m_treeMesh = m_loader->createMesh(treeData);
#endif

	Mesh* boxMesh = m_loader->createMesh(boxData);
	m_sun = new Entity(boxMesh, glm::vec3(-1.f, 10.f, -1.f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f));
	m_entityManager->add(m_sun);
	m_sunMoveSpeed = 10.f;


	float maxDist = 251.f;
	for (int i = 0; i < 200; i++)
	{
		float x = RandomNum::single(5.f, maxDist);
		float z = RandomNum::single(5.f, maxDist);
		float y = m_terrain->getHeight(glm::vec3(x, 0.f, z));

		m_entityManager->add(m_treeMesh,
			glm::vec3(x,y - 1.5f,z),
			glm::vec3(2.f, 2.f, 2.f));

	}

	for (int i = 0; i < 25; i++)
	{
		m_MasterRenderer->submitLight(new Light(
			RandomNum::vec3(0.f, maxDist, 10.f, 10.f, 0.f, maxDist),
			RandomNum::vec3(0.f, 255.f, 0.f, 255.f, 0.f, 255.f) / 255.f ));
	}

	glfwSwapInterval(1);
}

MainGame::~MainGame()
{
	delete m_parser;
	delete m_loader;
	delete m_MasterRenderer;
	delete m_fpsCamera;
	delete m_entityManager;
	delete m_terrain;
}

void MainGame::update(float dt)
{
	m_fpsCamera->setFreeLook(!m_terrainWalk);
	m_fpsCamera->update(dt);
	if(m_fpsCamera->isActive())
		jumpFunc(dt);
	m_entityManager->setMaxProcessingDistance(m_temp_processingDistance);
	m_entityManager->update(dt, m_fpsCamera, m_MasterRenderer);
	moveSunFunc(dt);

	if(m_vSync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void MainGame::render()
{
	m_MasterRenderer->render();
}

void MainGame::renderImGUI(float dt)
{
	ImGui::Begin("OpenGL");
	ImGui::Text("C = Lock/Unlock FPS camera");
	ImGui::Text("WASD = Move around");
	ImGui::Text("Space & CTRL = Move Up/Down");
	ImGui::Text("Average deltaT: %f", dt);
	ImGui::SliderFloat("Entity processing", &m_temp_processingDistance, 0.f, 1000.f);
	ImGui::Text("Entities processed: %i", m_entityManager->entitiesProcessed());
	ImGui::Text("Number of lights: %i", m_MasterRenderer->getNumberOfLights());
	ImGui::Text("Position: (%f, %f, %f)", m_fpsCamera->getPosition().x, m_fpsCamera->getPosition().y, m_fpsCamera->getPosition().z);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Vsync", &m_vSync);
	ImGui::Checkbox("Terrain walking", &m_terrainWalk);
	ImGui::SliderFloat("JumpForce", &m_jumpForce, 0.05f, 10.0f);
	ImGui::SliderFloat("Gravity", &m_gravity, -10.f, 0.f);
	

	ImGui::End();

	ImGui::Render();
}

void MainGame::jumpFunc(float dt)
{
	if (m_terrainWalk)
	{
		float cameraY = m_fpsCamera->getPosition().y;
		float targetY = m_terrain->getHeight(m_fpsCamera->getPosition());

		cameraY += m_upAcceleration;
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
	timer += dt * 0.4f;
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
	m_MasterRenderer->setSunPosition(pos);

}