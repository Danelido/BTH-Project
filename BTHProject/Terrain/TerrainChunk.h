#ifndef _TERRAIN_CHUNK_H
#define _TERRAIN_CHUNK_H
#include "Parser/ParserData.h"
#include "Meshes/Mesh.h"
#include "Graphics/Loader.h"

struct AABB;

class TerrainChunk
{
public:

	TerrainChunk(ParserData* data, Loader* loader, std::vector<GLuint> ids, int cellSize);
	~TerrainChunk();
	const Mesh* getMesh() const;
	AABB& getBoundary() const;

	const GLuint& getDebugVAO() const;

private:
	void calculateBoundary(ParserData* data, int cellSize);
	void createGLDebugLines();
private:
	Mesh* m_chunkMesh;
	AABB* m_boundary;

	// Debug
	GLuint m_vao;
	GLuint m_vbo;

};



#endif
