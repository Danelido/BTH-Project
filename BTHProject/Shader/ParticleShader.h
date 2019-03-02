#ifndef _PARTICLE_SHADER_H
#define _PARTICLE_SHADER_H
#include "Shader.h"
#include <GLM/glm.hpp>

class ParticleShader : public Shader
{
public:
	ParticleShader();
	~ParticleShader();

	void setViewMatrix(const glm::mat4& view);
	void setProjectionMatrix(const glm::mat4& proj);
	

private:

	GLint m_viewMatrixLocation;
	GLint m_projectionMatrixLocation;

};


#endif
