#include "AppSettings.h"

int AppSettings::m_srcWidth;
int AppSettings::m_srcHeight;
int AppSettings::m_maxLights = 300;
bool AppSettings::m_quadTreeDbgLayer = false; // Only works properly in debug mode for strange some reason..

void AppSettings::setSrcWidth(int width)
{
	m_srcWidth = width;
}

void AppSettings::setSrcHeight(int height)
{
	m_srcHeight = height;
}

void AppSettings::setQuadTreeDbgLayer(bool b)
{
	m_quadTreeDbgLayer = b;
}

void AppSettings::setMaxLights(int maxLights)
{
	m_maxLights = maxLights;
}
