#ifndef _PARTICLE_MANAGER_H
#define _PARTICLE_MANAGER_H
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include <Shader/ParticleShader.h>
#include <Utility/FPSCamera.h>

class ParticleManager 
{

#define MAX_PARTICLES 1000

public:
	ParticleManager(const glm::mat4& projectionMatrix);
	virtual ~ParticleManager();

	void update(float dt);
	void render(const FPSCamera* camera);
	void spawnParticle(const glm::vec3& position, const glm::vec3& spawnVelocity,const float& lifetime, const glm::vec4& color = glm::vec4(1.f,1.f,1.f,1.f));
	const int nrOfParticles() const;

private:

	 GLfloat m_vertex_buffer_data[12] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
	};

	 GLuint m_vertex_array_object;
	 GLuint m_vertex_buffer_center;
	 GLuint m_vertex_buffer_vertices;
	 GLuint m_vertex_buffer_color;

	struct Particle {
		glm::vec3 position;
		glm::vec3 spawnVelocity;
		glm::vec4 color;
		float lifetime = 0.f;
		float initialLifetime = 0.f;
	};

	std::vector<glm::vec3> m_centerVector;
	std::vector<glm::vec4> m_colorVector;
	ParticleShader* m_shader;
	int m_nrOfActiveParticles;
	Particle particles[MAX_PARTICLES];
	void setupBuffers();
	void updateBuffer();
	void swap(Particle& p1, Particle& p2);
};


#endif
