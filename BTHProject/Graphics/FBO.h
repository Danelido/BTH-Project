#ifndef _FRAMEBUFFER_OBJECT_H
#define _FRAMEBUFFER_OBJECT_H
#include <GL/glew.h>

class FBO
{
public:
	FBO();
	~FBO();

	void genFrameBuffer();

	void bindShadowFramebuffer();
	void unbindShadowFramebuffer();

	void bindDeferredFramebuffer();
	void unbindDeferredFramebuffer();
	void bindDeferredTextures();
	void unbindDeferredTextures();

private:
	void createDeferredShadingBuffers();
	void createShadowMappingBuffers();
	void genRenderBuffer();
	
	GLuint m_fbo;
	GLuint m_shadowFbo;
	GLuint m_texColorBuffer;
	GLuint m_texDepthBuffer;
	GLuint m_rbo;

	GLuint m_gPosition;
	GLuint m_gNormal;
	GLuint m_gAlbedoSpec;
	GLuint m_depthMap; // For shadow-mapping
};

#endif
