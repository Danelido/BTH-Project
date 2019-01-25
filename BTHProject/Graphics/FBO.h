#ifndef _FRAMEBUFFER_OBJECT_H
#define _FRAMEBUFFER_OBJECT_H
#include <GL/glew.h>

class FBO
{
public:
	FBO();
	~FBO();

	void genFrameBuffer();

	void bindFramebuffer();
	void unbindFramebuffer();
	void bindTexture();
	void unbindTexture();

private:
	void createDeferredShadingBuffers();
	void genRenderBuffer();
	GLuint m_fbo;
	GLuint m_texColorBuffer;
	GLuint m_texDepthBuffer;
	GLuint m_rbo;

	GLuint m_gPosition;
	GLuint m_gNormal;
	GLuint m_gAlbedoSpec;
};

#endif
