#ifndef _LIGHT_H
#define _LIGHT_H
#include <GLM/glm.hpp>
#include <GL/glew.h>

struct AABB;

class Light
{
public:
	Light(const glm::vec3& position, const glm::vec3& color, const float& radius);
	~Light();

	void setPosition(const glm::vec3& position);
	void setColor(const glm::vec3& color);
	void setRadius(const float& radius);
	const glm::vec3& getPosition() const;
	const glm::vec3& getColor() const;
	const float& getRadius() const;

	// Debug purpose
	void setupDebugData();
	const GLuint& getDebugVAO() const;

private:
	glm::vec3 m_position;
	glm::vec3 m_color;
	glm::vec3 m_center;
	float m_radius;

// Debug purpose
private:
	AABB* m_boundary;
	GLuint m_vao;
	GLuint m_vbo;

};


#endif
