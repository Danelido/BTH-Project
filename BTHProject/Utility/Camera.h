#ifndef _CAMERA_H
#define _CAMERA_H
#include <GLM/glm.hpp>

class Camera {

public:
	Camera();
	~Camera();

	void update(float dt);
	void setY(float y);
	void setFreeLook(bool b);

	const glm::mat4& getViewMatrix() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getCameraUp() const;

private:
	void snapMouseToMiddle();
	void movement();
	void mouseLook();
	void updateVectors();
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_pitch;
	float m_yaw;
	float m_movementSpeed;
	float m_mouseSensitivity;
	bool m_freeLook;


};


#endif
