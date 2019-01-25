#ifndef _FPS_CAMERA_H
#define _FPS_CAMERA_H
#include <GLM/glm.hpp>

class FPSCamera
{
public:
	FPSCamera(const glm::vec3& position);
	~FPSCamera();
	void update(float dt);
	const glm::mat4& getViewMatrix() const;
	const glm::vec3& getPosition() const;
	void setY(float y);
	void setFreeLook(bool b);
	const bool& isActive() const;
private:
	void additionalControls();
	void lookAround(float dt);
	void move(float dt);
	void snapMouseToMiddle();
	void updateViewMatrix();
private:
	glm::dvec2 m_currentMousePos;
	glm::dvec2 m_lastMousePos;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::mat4 m_viewMatrix;
	float m_pitch;
	float m_yaw;
	float m_sensitivity;
	float m_cameraSpeed;
	bool m_cameraActive;
	bool m_freeLook;
};

#endif
