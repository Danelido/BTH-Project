#include "Application.h"
#include "Utility/Input.h"
#include <iostream>
#include <sstream>
#include "Vendor/ImGui/imgui.h"
#include "Vendor/ImGui/imgui_impl_glfw.h"
#include "Vendor/ImGui/imgui_impl_opengl3.h"


Application::Application()
{
}

Application::~Application()
{
	delete m_input;
	delete m_meshMemoryCollector;
	delete m_mainGame;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

bool Application::createWindow(std::string title, int width, int height)
{
	if (!glfwInit())
	{
		printf("Failed to initialize glfw\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	m_appSetting.setSrcWidth(width);
	m_appSetting.setSrcHeight(height);

	if (!m_window)
	{
		glfwTerminate();
		printf("Failed to create glfwWindow\n");
		return false;
	}

	glfwMakeContextCurrent(m_window);

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		printf("Failed to initialize glew\n");
		return false;
	}

	// Vsync enabled
	glfwSwapInterval(1);

	glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 410 core");
	ImGui::StyleColorsDark();
	
	
	m_meshMemoryCollector = new MeshMemoryCollector();
	m_input = new Input();
	m_mainGame = new MainGame();
	glViewport(0, 0, AppSettings::SRCWIDTH(), AppSettings::SRCHEIGHT());
	return true;
}

void Application::run()
{
	m_deltaArraySize = 0;
	m_FilledArray = false;
	m_maxDeltaSamples = 10;
	while (!glfwWindowShouldClose(m_window)) 
	{
		m_input->clearKeys();
		glfwPollEvents();
		
		if (Input::isKeyPressed(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(m_window, 1);
		}
		
		smoothDelta();
		m_mainGame->update(m_deltaTime);
		m_mainGame->render();

		// IMGUI STUFF
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		m_mainGame->renderImGUI(m_deltaTime);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(m_window);
	}

}

void Application::smoothDelta()
{
	m_currentTime = glfwGetTime();
	double dt = m_currentTime - m_lastTime;
	m_lastTime = m_currentTime;

	if (m_deltaArraySize % m_deltaArray.size() == 0)
	{
		m_deltaArraySize = 0;
		m_FilledArray = true;
	}

	m_deltaArray[m_deltaArraySize++] = dt;


	if (!m_FilledArray)
	{
		for (size_t i = 0; i < m_deltaArraySize; i++)
		{
			m_deltaTime += m_deltaArray[i];
		}
		m_deltaTime /= (double)m_deltaArraySize;
	}
	else
	{
		for (size_t i = 0; i < m_deltaArray.size(); i++)
		{
			m_deltaTime += m_deltaArray[i];
		}
		m_deltaTime /= (double)(m_deltaArray.size());
	}

}
