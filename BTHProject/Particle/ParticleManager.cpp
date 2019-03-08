#include "ParticleManager.h"
#include <iostream>

ParticleManager::ParticleManager(const glm::mat4& projectionMatrix)
{
	m_shader = new ParticleShader();
	m_shader->use();
	m_shader->setProjectionMatrix(projectionMatrix);
	m_shader->unuse();

	setupBuffers();
}

ParticleManager::~ParticleManager()
{
	delete m_shader;
}
void ParticleManager::update(float dt)
{
	m_centerVector.clear();
	m_colorVector.clear();

	for (int i = 0; i < this->m_nrOfActiveParticles; i++)
	{
		Particle& p = this->particles[i];

		p.lifetime -= dt;

		if (p.lifetime <= 0.0f)
		{
			// Swap the dead one with the last one
			if (i != this->m_nrOfActiveParticles - 1)
				swap(p, this->particles[this->m_nrOfActiveParticles - 1]);
			this->m_nrOfActiveParticles--;

			// Go to the next Particle
			continue;
		}

		// If not dead, then update position 
		p.spawnVelocity += glm::vec3(0.f, -.3f, 0.f) * dt;
		p.position += p.spawnVelocity;
		p.color.a = p.lifetime / p.initialLifetime;
		m_centerVector.push_back(p.position);
		m_colorVector.push_back(p.color);
	}

	updateBuffer();
}

void ParticleManager::render(const FPSCamera* camera)
{
	glEnable(GL_BLEND);

	glBindVertexArray(m_vertex_array_object);
	
	for (GLuint i = 0; i < 3; i++)
		glEnableVertexAttribArray(i);

	m_shader->use();
	m_shader->setViewMatrix(camera->getViewMatrix());
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, this->m_nrOfActiveParticles);
	m_shader->unuse();

	for (GLuint i = 0; i < 3; i++)
		glDisableVertexAttribArray(i);

	glBindVertexArray(NULL);
	glDisable(GL_BLEND);

}

void ParticleManager::spawnParticle(const glm::vec3 & position, const glm::vec3 & spawnVelocity, const float & lifetime, const glm::vec4& color)
{
	if (this->m_nrOfActiveParticles != MAX_PARTICLES)
	{
		Particle& p = this->particles[this->m_nrOfActiveParticles];
		p.position = position;
		p.spawnVelocity = spawnVelocity;
		p.color = color;
		p.lifetime = lifetime;
		p.initialLifetime = lifetime;
		this->m_nrOfActiveParticles++;
	}
}

void ParticleManager::setupBuffers()
{
	glGenVertexArrays(1, &m_vertex_array_object);
	glGenBuffers(1, &m_vertex_buffer_vertices);
	glGenBuffers(1, &m_vertex_buffer_center);
	glGenBuffers(1, &m_vertex_buffer_color);

	glBindVertexArray(m_vertex_array_object);
	
	// Vertices buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertex_buffer_data), m_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, NULL, NULL);

	// Buffer for the center positions
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_center);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(GL_FLOAT) * 3, NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, NULL, NULL);
	//glVertexAttribDivisor(0, 0); // always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1);


	// Buffer for the center positions
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_color);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(GL_FLOAT) * 4, NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, NULL, NULL);
	//glVertexAttribDivisor(0, 0); // always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(2, 1);

}

void ParticleManager::updateBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_center);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(GL_FLOAT) * 3, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_centerVector.size() * sizeof(GL_FLOAT) * 3, m_centerVector.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_color);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(GL_FLOAT) * 4, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_colorVector.size() * sizeof(GL_FLOAT) * 4, m_colorVector.data());

}

void ParticleManager::swap(Particle & p1, Particle & p2)
{
	Particle temp = p1;
	p1 = p2;
	p2 = temp;
}

const int ParticleManager::nrOfParticles() const
{
	return m_nrOfActiveParticles;
}
