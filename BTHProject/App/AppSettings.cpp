#include "AppSettings.h"

int AppSettings::m_srcWidth;
int AppSettings::m_srcHeight;
int AppSettings::m_maxLights = 300;

void AppSettings::setSrcWidth(int width)
{
	m_srcWidth = width;
}

void AppSettings::setSrcHeight(int height)
{
	m_srcHeight = height;
}

void AppSettings::setMaxLights(int maxLights)
{
	m_maxLights = maxLights;
}
