#include "InstancedMesh.h"

InstancedMesh::InstancedMesh(GLuint vao, std::vector<GLuint> textureIDS, const glm::vec3& ambientColor,
	const glm::vec3 diffuseColor, const glm::vec3& specularColor,
	float shininess, GLuint indicesSize, GLuint emptyBufferID) :
	m_vao(vao),
	m_textureIDs(textureIDS),
	m_ambientColor(ambientColor),
	m_diffuseColor(diffuseColor),
	m_specularColor(specularColor),
	m_shininess(shininess),
	m_indicesSize(indicesSize),
	m_modelBufferID(emptyBufferID)

{
}

InstancedMesh::~InstancedMesh()
{
}

const GLuint & InstancedMesh::getVao() const
{
	return m_vao;
}

const std::vector<GLuint> & InstancedMesh::getTexIDs() const
{
	return m_textureIDs;
}

const GLuint & InstancedMesh::getIndicesSize() const
{
	return m_indicesSize;
}

const glm::vec3 & InstancedMesh::getDiffuseColor() const
{
	return m_diffuseColor;
}

const glm::vec3 & InstancedMesh::getSpecularColor() const
{
	return m_specularColor;
}

const glm::vec3 & InstancedMesh::getAmbientColor() const
{
	return m_ambientColor;
}

const float & InstancedMesh::getShininess() const
{
	return m_shininess;
}

const GLuint & InstancedMesh::getModelBufferID() const
{
	return m_modelBufferID;
}
