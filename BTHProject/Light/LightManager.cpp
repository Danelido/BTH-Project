#include "LightManager.h"
#include "Renderers/MasterRenderer.h"
#include "Utility/FPSCamera.h"

LightManager::LightManager(){

}

LightManager::~LightManager(){
	for (size_t i = 0; i < m_lights.size(); i++)
		delete m_lights[i];
}

void LightManager::addLight(Light* light){
	m_lights.emplace_back(light);
}

void LightManager::update(MasterRenderer* renderer, FPSCamera* camera, std::vector<QuadTreeObject*> objects){
	
	m_processedLights = 0;
	
	if (!m_ignoreQuadTree)
	{
		/*for (size_t i = 0; i < objects.size(); i++)
		{
			if (objects[i]->getLight() != nullptr) {
				renderer->submitLight(objects[i]->getLight());
				m_processedLights++;
			}
		}*/

		for (size_t i = 0; i < m_lights.size(); i++)
		{
			if (camera->insideFrustum(m_lights[i]->getPosition(), m_lights[i]->getRadius())) {
				renderer->submitLight(m_lights[i]);
				m_processedLights++;
			}
		}


	}
	else if (m_ignoreQuadTree)
	{
		for (size_t i = 0; i < m_lights.size(); i++)
		{
			renderer->submitLight(m_lights[i]);
			m_processedLights++;
		}
	}
}

void LightManager::setIgnoreQuadTree(bool b){
	m_ignoreQuadTree = b;
}

void LightManager::reserveVectorMemory(int capacity){
	m_lights.reserve(capacity);
}

const unsigned int& LightManager::getNrOfLightsProcessed() const {
	return m_processedLights;
}