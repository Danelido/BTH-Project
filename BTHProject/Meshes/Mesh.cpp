#include "Mesh.h"

Mesh::Mesh(GLuint vao, std::vector<GLuint> textureIDS, const glm::vec3& ambientColor,
const glm::vec3 diffuseColor, const glm::vec3& specularColor,
float shininess, GLuint indicesSize) :
	m_vao(vao),
	m_textureIDS(textureIDS),
	m_ambientColor(ambientColor),
	m_diffuseColor(diffuseColor),
	m_specularColor(specularColor),
	m_shininess(shininess),
	m_indicesSize(indicesSize)
	
{
}

Mesh::~Mesh()
{
}

const GLuint & Mesh::getVao() const
{
	return m_vao;
}

const std::vector<GLuint> & Mesh::getTexIDs() const
{
	return m_textureIDS;
}

const GLuint & Mesh::getIndicesSize() const
{
	return m_indicesSize;
}

const glm::vec3 & Mesh::getDiffuseColor() const
{
	return m_diffuseColor;
}

const glm::vec3 & Mesh::getSpecularColor() const
{
	return m_specularColor;
}

const glm::vec3 & Mesh::getAmbientColor() const
{
	return m_ambientColor;
}

const float & Mesh::getShininess() const
{
	return m_shininess;
}
