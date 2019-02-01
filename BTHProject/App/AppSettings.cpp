#include "AppSettings.h"

int AppSettings::m_srcWidth;
int AppSettings::m_srcHeight;
int AppSettings::m_maxLights = 330;
bool AppSettings::m_debugLayer = false; // Only works properly in debug mode for strange some reason..

void AppSettings::setSrcWidth(int width)
{
	m_srcWidth = width;
}

void AppSettings::setSrcHeight(int height)
{
	m_srcHeight = height;
}

void AppSettings::setDebugLayer(bool b)
{
	m_debugLayer = b;
}

void AppSettings::setMaxLights(int maxLights)
{
	m_maxLights = maxLights;
}
