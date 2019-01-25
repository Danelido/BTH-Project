#include "ParserData.h"

#define CAPACITY 250000

ParserData::ParserData()
{
	m_indices.reserve(CAPACITY);
	m_vertices.reserve(CAPACITY);
	m_uvs.reserve(CAPACITY);
	m_normals.reserve(CAPACITY);

	m_textureNames.reserve(10);
	m_ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	m_diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
	m_specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_shininess = 0.0f;
}

ParserData::~ParserData()
{
}

void ParserData::addIndices(GLuint index)
{
	m_indices.emplace_back(index);
}

void ParserData::addVertex(GLfloat x, GLfloat y, GLfloat z)
{
	m_vertices.emplace_back(x);
	m_vertices.emplace_back(y);
	m_vertices.emplace_back(z);
}

void ParserData::addUV(GLfloat u, GLfloat v)
{
	m_uvs.emplace_back(u);
	m_uvs.emplace_back(v);
}

void ParserData::addNormal(GLfloat x, GLfloat y, GLfloat z)
{
	m_normals.emplace_back(x);
	m_normals.emplace_back(y);
	m_normals.emplace_back(z);
}

void ParserData::addTextureFilename(std::string filename)
{
	m_textureNames.emplace_back(filename);
}

void ParserData::setDiffuseColor(float r, float g, float b)
{
	m_diffuseColor = glm::vec3(r,g,b);
}

void ParserData::setSpecularColor(float r, float g, float b)
{
	m_specularColor = glm::vec3(r, g, b);
}

void ParserData::setAmbientColor(float r, float g, float b)
{
	m_ambientColor = glm::vec3(r, g, b);
}

void ParserData::setShininess(float factor)
{
	m_shininess = factor;
}

const std::vector<GLuint>& ParserData::getIndices() const
{
	return m_indices;
}

const std::vector<GLfloat>& ParserData::getVertices() const
{
	return m_vertices;
}

const std::vector<GLfloat>& ParserData::getUvs() const
{
	return m_uvs;
}

const std::vector<GLfloat>& ParserData::getNormals() const
{
	return m_normals;
}

const std::vector<std::string>& ParserData::getTextureFilenames() const
{
	return m_textureNames;
}

const float & ParserData::getShininess() const
{
	return m_shininess;
}

const glm::vec3 & ParserData::getDiffuseColor() const
{
	return m_diffuseColor;
}

const glm::vec3 & ParserData::getSpecularColor() const
{
	return m_specularColor;
}

const glm::vec3 & ParserData::getAmbientColor() const
{
	return m_ambientColor;
}
