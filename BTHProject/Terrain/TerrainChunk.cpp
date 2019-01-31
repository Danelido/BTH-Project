#include "TerrainChunk.h"
#include "Acceleration/DataStructs.h"
#include "App/AppSettings.h"

TerrainChunk::TerrainChunk(ParserData * data, Loader* loader, std::vector<GLuint> ids, int cellSize)
{
	m_chunkMesh = loader->createTerrainMesh(data, ids);
	calculateBoundary(data, cellSize);

	if (AppSettings::QUADTREE_DBG())
		createGLDebugLines();

	delete data;
}

TerrainChunk::~TerrainChunk()
{
	delete m_boundary;
}

const Mesh * TerrainChunk::getMesh() const
{
	return m_chunkMesh;
}

AABB& TerrainChunk::getBoundary() const
{
	return *m_boundary;
}

const GLuint & TerrainChunk::getDebugLineVao() const
{
	return m_vao;
}

void TerrainChunk::calculateBoundary(ParserData* data, int cellSize)
{
	m_boundary = new AABB();
	float max_width = -100000.0;
	float min_width = 100000.0;
	float max_depth = -100000.0;
	float min_depth = 100000.0;

	for (int i = 0; i < data->getVertices().size(); i+=3)
	{
		float width = data->getVertices()[i];
		float depth = data->getVertices()[i + 2];

		if (width > max_width)
			max_width = width;
		
		else if (width < min_width)
			min_width = width;

		if (depth > max_depth)
			max_depth = depth;

		else if (depth < min_depth)
			min_depth = depth;

	}

	m_boundary->halfDimension = (cellSize - 2.f) / 2.f;
	m_boundary->center.x = ((max_width - min_width)) + min_width - m_boundary->halfDimension; 
	m_boundary->center.z = ((max_depth - min_depth)) + min_depth - m_boundary->halfDimension;
}

void TerrainChunk::createGLDebugLines()
{
	float data[8 * 3] =
	{
		// 1
		m_boundary->center.x - m_boundary->halfDimension,
		0.f,
		m_boundary->center.z - m_boundary->halfDimension,


		// 2
		m_boundary->center.x - m_boundary->halfDimension,
		0.f,
		m_boundary->center.z + m_boundary->halfDimension,


		// 3
		m_boundary->center.x - m_boundary->halfDimension,
		0.f,
		m_boundary->center.z + m_boundary->halfDimension,


		// 4
		m_boundary->center.x + m_boundary->halfDimension,
		0.f,
		m_boundary->center.z + m_boundary->halfDimension,



		// 5
		m_boundary->center.x + m_boundary->halfDimension,
		0.f,
		m_boundary->center.z + m_boundary->halfDimension,


		// 6
		m_boundary->center.x + m_boundary->halfDimension,
		0.f,
		m_boundary->center.z - m_boundary->halfDimension,

		// 7
		m_boundary->center.x + m_boundary->halfDimension,
		0.f,
		m_boundary->center.z - m_boundary->halfDimension,


		// 8
		m_boundary->center.x - m_boundary->halfDimension,
		0.f,
		m_boundary->center.z - m_boundary->halfDimension,

	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
