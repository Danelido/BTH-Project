#include "TerrainChunk.h"
#include "Acceleration/DataStructs.h"
#include "App/AppSettings.h"

TerrainChunk::TerrainChunk(ParserData * data, Loader* loader, std::vector<GLuint> ids, int cellSize)
{
	m_chunkMesh = loader->createTerrainMesh(data, ids);
	calculateBoundary(data, cellSize);

	if (AppSettings::DEBUG_LAYER())
		createGLDebugLines();

	delete data;
}

TerrainChunk::~TerrainChunk()
{
	delete m_boundary;
	if (AppSettings::DEBUG_LAYER()) {
		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);
	}
}

const Mesh * TerrainChunk::getMesh() const
{
	return m_chunkMesh;
}

AABB& TerrainChunk::getBoundary() const
{
	return *m_boundary;
}

const GLuint & TerrainChunk::getDebugVAO() const
{
	return m_vao;
}

void TerrainChunk::calculateBoundary(ParserData* data, int cellSize)
{
	m_boundary = new AABB();
	float max_width = -100000.0;
	float min_width = 100000.0;
	float max_height = -100000.0;
	float min_height = 100000.0;
	float max_depth = -100000.0;
	float min_depth = 100000.0;

	for (int i = 0; i < data->getVertices().size(); i+=3)
	{
		float width = data->getVertices()[i];
		float height = data->getVertices()[i + 1];
		float depth = data->getVertices()[i + 2];

		if (width > max_width)
			max_width = width;
		
		else if (width < min_width)
			min_width = width;

		if (height > max_height)
			max_height = height;

		else if (height < min_height)
			min_height = height;

		if (depth > max_depth)
			max_depth = depth;

		else if (depth < min_depth)
			min_depth = depth;

	}

	m_boundary->halfDimensions.x = (cellSize) / 2.f;
	m_boundary->halfDimensions.y = (cellSize) / 2.f;
	m_boundary->halfDimensions.z = (cellSize) / 2.f;
	m_boundary->center.x = ((max_width - min_width)) + min_width - m_boundary->halfDimensions.x; 
	m_boundary->center.y = ((max_height - min_height)) + min_height - m_boundary->halfDimensions.y;
	m_boundary->center.z = ((max_depth - min_depth)) + min_depth - m_boundary->halfDimensions.z;
}

void TerrainChunk::createGLDebugLines()
{
	float data[24 * 3] =
	{
		// FIRST RECTANGLE
		// 1
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,


		// 2
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,


		// 3
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,


		// 4
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,



		// 5
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,


		// 6
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		// 7
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,


		// 8
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		// SECOND RECTANGLE
		// 1
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,


		// 2
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,


		// 3
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,


		// 4
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,



		// 5
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,


		// 6
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		// 7
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,


		// 8
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		// CONNECTORS
		// First to first
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		// Second to second
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z - m_boundary->halfDimensions.z,

		// Third to third
		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,

		m_boundary->center.x - m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,

		// Fourth to fourth
		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y - m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,

		m_boundary->center.x + m_boundary->halfDimensions.x,
		m_boundary->center.y + m_boundary->halfDimensions.y,
		m_boundary->center.z + m_boundary->halfDimensions.z,
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
