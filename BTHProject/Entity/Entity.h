#ifndef _ENTITY_H
#define _ENTITY_H
#include <GLM/glm.hpp>
#include "Meshes/Mesh.h"
#include "Meshes/InstancedMesh.h"

struct AABB;

class Entity
{
public:
	Entity(Mesh* mesh, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);
	Entity(InstancedMesh* instancedMesh, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation);
	~Entity();
	
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);
	void setBoundaryManually(float width, float height, float depth);
	void setSelected(bool condition);
	void updateBoundary();
	GLuint getDebugVAO();
	const glm::vec3& getPosition() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getScale() const;
	const glm::mat4& getModelMatrix() const;
	const bool& isInstanced() const;
	const bool& isSelected() const;
	AABB& getBoundary() const;
	Mesh* getMesh();
	InstancedMesh* getInstancedMesh();

private:
	Mesh* m_mesh;
	InstancedMesh* m_instancedMesh;
	AABB* m_boundary;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	glm::mat4 m_modelMatrix;
	bool m_isInstanced;
	void updateModelMatrix();

	// DEBUG
	bool m_selected;
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	bool m_hasBeenSetUp = false;
	bool m_updateBoundaryDebugLines;
	bool m_hasManuallySetBoundary = false;
};

#endif
