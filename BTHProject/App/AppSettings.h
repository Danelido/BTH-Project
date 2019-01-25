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

	inline static int MAXLIGHTS()
	{
		return m_maxLights;
	}

	static void setMaxLights(int maxLights);

private:
	friend class Application;

	void setSrcWidth(int width);
	void setSrcHeight(int height);
	
	static int m_srcWidth;
	static int m_srcHeight;
	static int m_maxLights;

};

#endif
