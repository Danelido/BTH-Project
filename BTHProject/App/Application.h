#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utility/MeshMemoryCollector.h"
#include "MainGame.h"
#include "Utility/Input.h"
#include "AppSettings.h"
#include <array>

class Application
{
public:
	Application();
	~Application();
	bool createWindow(std::string title, int width, int height);
	void run();
private:
	float m_currentTime;
	float m_deltaTime;
	float m_lastTime;
	int m_deltaArraySize;
	int m_maxDeltaSamples;
	std::array<float, 50> m_deltaArray;
	void smoothDelta();
	bool m_FilledArray;
private:
	GLFWwindow* m_window;
	Input* m_input;
	MainGame* m_mainGame;
	AppSettings m_appSetting;
	MeshMemoryCollector* m_meshMemoryCollector;
};

#endif