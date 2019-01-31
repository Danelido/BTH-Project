#ifndef _LIGHT_MANAGER_H
#define _LIGHT_MANAGER_H
#include "Acceleration/QuadTreeObject.h"
#include <vector>

class MasterRenderer;
class FPSCamera;

class LightManager
{
public:
	LightManager();
	~LightManager();

	void addLight(Light* light);
	void update(MasterRenderer* renderer, FPSCamera* camera ,std::vector<QuadTreeObject*> objects);
	void setIgnoreQuadTree(bool b);
	void reserveVectorMemory(int capacity);
	const unsigned int& getNrOfLightsProcessed() const;


private:
	std::vector<Light*> m_lights;
	bool m_ignoreQuadTree;
	unsigned int m_processedLights;
};


#endif
