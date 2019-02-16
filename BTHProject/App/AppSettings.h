#ifndef _APPLICATION_SETTINGS_H
#define _APPLICATION_SETTINGS_H

class Application;

class AppSettings
{
public:
	
	inline static int SRCWIDTH()
	{
		return m_srcWidth;
	}

	inline static int SRCHEIGHT()
	{
		return m_srcHeight;
	}

	inline static int SHADOW_WIDTH()
	{
		return m_shadowWidth;
	}

	inline static int SHADOW_HEIGHT()
	{
		return m_shadowHeight;
	}

	inline static int MAXLIGHTS()
	{
		return m_maxLights;
	}

	inline static bool DEBUG_LAYER() {
		return m_debugLayer;
	}

	static void setMaxLights(int maxLights);

private:
	friend class Application;

	void setSrcWidth(int width);
	void setSrcHeight(int height);
	void setDebugLayer(bool b);

	static int m_shadowWidth;
	static int m_shadowHeight;
	static int m_srcWidth;
	static int m_srcHeight;
	static int m_maxLights;
	static bool m_debugLayer;

};

#endif
