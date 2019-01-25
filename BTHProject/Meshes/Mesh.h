#ifndef _MESH_H
#define _MESH_H
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <vector>
class Mesh
{
public:
	Mesh(GLuint vao, std::vector<GLuint> textureIDS, const glm::vec3& ambientColor,
		const glm::vec3 diffuseColor, const glm::vec3& specularColor,
		float shininess, GLuint indicesSize);
	~Mesh();

	const GLuint& getVao() const;
	const std::vector<GLuint>& getTexIDs() const;
	const GLuint& getIndicesSize() const;
	const glm::vec3& getDiffuseColor() const;
	const glm::vec3& getSpecularColor() const;
	const glm::vec3& getAmbientColor() const;
	const float& getShininess() const;

private:
	GLuint m_vao;
	std::vector<GLuint> m_textureIDS;
	GLuint m_indicesSize;

	glm::vec3 m_ambientColor;
	glm::vec3 m_diffuseColor;
	glm::vec3 m_specularColor;

	float m_shininess;

};

#endif
