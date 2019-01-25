#ifndef _PARSER_DATA_H
#define _PARSER_DATA_H
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
#include <string>

class Terrain;

class ParserData
{
public:
	ParserData();
	~ParserData();

	void addIndices(GLuint index);
	void addVertex(GLfloat x, GLfloat y, GLfloat z);
	void addUV(GLfloat u, GLfloat v);
	void addNormal(GLfloat x, GLfloat y, GLfloat z);
	void addTextureFilename(std::string filename);
	void setDiffuseColor(float r, float g, float b);
	void setSpecularColor(float r, float g, float b);
	void setAmbientColor(float r, float g, float b);
	void setShininess(float factor);

	const std::vector<GLuint>& getIndices() const;
	const std::vector<GLfloat>& getVertices() const;
	const std::vector<GLfloat>& getUvs() const;
	const std::vector<GLfloat>& getNormals() const;
	const std::vector<std::string>& getTextureFilenames() const;
	const glm::vec3& getDiffuseColor() const;
	const glm::vec3& getSpecularColor() const;
	const glm::vec3& getAmbientColor() const;
	const float& getShininess() const;

private:
	std::vector<GLuint> m_indices;
	std::vector<GLfloat> m_vertices;
	std::vector<GLfloat> m_uvs;
	std::vector<GLfloat> m_normals;
	std::vector<std::string> m_textureNames;
	
	glm::vec3 m_ambientColor;
	glm::vec3 m_diffuseColor;
	glm::vec3 m_specularColor;
	
	float m_shininess;

	friend class Terrain; // Terrain class needs to modify the vertices array

};

#endif
