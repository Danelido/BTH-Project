#ifndef _QUAD_TREE_H
#define _QUAD_TREE_H

#include <vector>
#include "DataStructs.h"
#include "Utility/FPSCamera.h"
#include <GL/glew.h>
#include "Shader/QuadTreeDebugShader.h"
#include "QuadTreeObject.h"

class QuadTree
{
public:
	const int QT_NODE_CAPACITY = 4;

	QuadTree(AABB boundary, FPSCamera* camera);
	~QuadTree();

	bool insert(Entity* entity);
	bool insert(TerrainChunk* chunk);
	std::vector<QuadTreeObject*> query();
	void draw(QuadTreeDebugShader* shader);
private:
	void subDivide();
	void createGLDrawable(); // Debug purpose
private:
	AABB m_boundary;
	FPSCamera* m_camera;
	std::vector<QuadTreeObject*> m_treeObjects;

	// Debug
	GLuint m_vao;
	GLuint m_vbo;
	int m_numVertices;

	// Children
	QuadTree* m_northWest;
	QuadTree* m_northEast;
	QuadTree* m_southWest;
	QuadTree* m_southEast;

};


#endif

