#ifndef _FPS_CAMERA_H
#define _FPS_CAMERA_H
#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <Acceleration/DataStructs.h>

class FPSCamera
{
public:
	const float NEAR_CLIPPING = 0.1f;
	const float FAR_CLIPPING = 70.f;
	const float FOV = 45.f;

public:
	FPSCamera(const glm::vec3& position);
	~FPSCamera();
	void update(float dt);
	void debugDraw();
	void initDebugMode();
	bool insideFrustum(AABB& aabb);
	bool insideFrustum(const glm::vec3& pos, float radius);
	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;
	const glm::vec3& getPosition() const;
	const glm::vec3& getCameraUp() const;
	void setY(float y);
	void setFreeLook(bool b);
	const bool& isActive() const;
private:
	void additionalControls();
	void lookAround(float dt);
	void move(float dt);
	void snapMouseToMiddle();
	void updateViewMatrix();
	void calculateCameraUp();
	void calculateFrustumPoints();
	void calculateFrustumPlaneAttributes();
	void calculateFrustumPlanes();
// DEBUG PURPOSE
private:
	void setupFrustumLinesAndLoadBuffer();
	void calculateDebugData();
	float* m_lineData;
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_numVertices;
	bool m_debugCamera;
private:
	glm::dvec2 m_currentMousePos;
	glm::dvec2 m_lastMousePos;
	glm::vec3 m_position;
	glm::vec3 m_walkDirection;
	glm::vec3 m_lookDirection;
	glm::vec3 m_cameraUp;
	glm::vec3 m_right;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	float m_pitch;
	float m_yaw;
	float m_sensitivity;
	float m_cameraSpeed;
	bool m_cameraActive;
	bool m_freeLook;
	// Frustum
	float m_nearPlaneHeight;
	float m_nearPlaneWidth;

	float m_farPlaneHeight;
	float m_farPlaneWidth;

	float m_fov;

	glm::vec3 m_nearPlanePoints[4];
	glm::vec3 m_farPlanePoints[4];

	struct Plane
	{
		glm::vec3 normal;
		float d;

		void setPoints(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
		{
			normal = glm::normalize(glm::cross((p3 - p2), (p1 - p2)));
			d = -glm::dot(normal, p2);
		}

		float distance(const glm::vec3& point)
		{
			return (d + glm::dot(normal, point));
		}

	};

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

	Plane m_planes[6];

};

#endif
