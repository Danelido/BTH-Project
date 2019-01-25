#ifndef _LIGHT_H
#define _LIGHT_H
#include <GLM/glm.hpp>

class Light
{
public:
	Light(const glm::vec3& position, const glm::vec3& color);
	~Light();

	void setPosition(const glm::vec3& position);
	void setColor(const glm::vec3& color);
	const glm::vec3& getPosition() const;
	const glm::vec3& getColor() const;


private:
	glm::vec3 m_position;
	glm::vec3 m_color;


};


#endif
