#ifndef _LOADER_H
#define _LOADER_H
#include <vector>
#include <GL/glew.h>
#include "Parser/ParserData.h"
#include "Meshes/Mesh.h"
#include "Meshes/InstancedMesh.h"

class Loader
{
public:
	Loader();
	~Loader();

	Mesh* createMesh(ParserData* data);
	Mesh* createTerrainMesh(ParserData* data, std::vector<std::string> textureNames);
	InstancedMesh* createInstancedMesh(ParserData* data);

private:
	GLuint createAndBindVAO();
	GLuint createTexture(std::string filename);
	void bindIndices(const std::vector<GLuint>& indices);
	void storeDataInAttributeList(const GLuint& attributeNumber, const GLuint& dataPerVertex, const std::vector<GLfloat>& data);
	GLuint createEmptyModelMatrixBuffer(const GLuint& attributeNumber);
	void unbindVAO();

	// For memory management (openGL)
	std::vector<GLuint*> m_vaos;
	std::vector<GLuint*> m_vbos;
	std::vector<GLuint*> m_texIDs;

};

#endif
