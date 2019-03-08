#include "Terrain.h"
#include <iostream>
#include "Utility/MatrixCreator.h"
#include "Vendor/stb/stb_image.h"

Terrain::Terrain(Loader* loader, TerrainChunkManager* chunkManager, QuadTree* quadtree)
{
	m_loader = loader;
	m_quadTree = quadtree;
	m_chunkManager = chunkManager;
	m_terrainWidth = 64;
	m_terrainHeight = 64;
	m_modelMatrix = MatrixCreator::createNewModelMatrix(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	m_size = 1.f;
	m_heightScaleDivider = 32;
	GLint width, height, channels;
	const char* filename = "Resources/Textures/hmap.png";
	stbi_set_flip_vertically_on_load(0);
	unsigned char* textureData = stbi_load(filename, &width, &height, &channels, STBI_rgb);

	if (textureData && width == m_terrainWidth && height == m_terrainHeight)
	{
		loadHeightsFromHeightmap(textureData, width,height,channels);
		smoothNormals();
		generateTerrain();

		//createVerticesAndNormals(textureData, width, height, channels);

		m_loadedSuccessful = true;
	}
	else
	{
		printf("Failed to load texture %s or wrong dimensions\n", filename);
		m_loadedSuccessful = false;
	}

	stbi_image_free(textureData);

	delete m_data;
}

float Terrain::getHeight(const glm::vec3& position)
{
	// Make this smoother jeesus
	if (position.x > 0.f && position.x < (float)m_terrainWidth - 1 && position.z > 0.f && position.z < (float)m_terrainWidth - 1)
	{
		int x = static_cast<int>(((float)position.x / m_terrainWidth) * m_terrainWidth);
		int z = static_cast<int>((1.f - (position.z / m_terrainWidth)) * m_terrainWidth);
		float y = m_heightmapCoords[x + (z * m_terrainWidth)].y;
		return (y); // 0.5f offset to make it a little bit above ground

	}
	return 0.f;
}

const glm::mat4 & Terrain::getModelMatrix() const
{
	return m_modelMatrix;
}


Terrain::~Terrain()
{
	delete m_heightmapCoords;
}

void Terrain::loadHeightsFromHeightmap(unsigned char * textureData, int width, int height, int channels)
{
	m_heightmapCoords = new TerrainData[width * height];

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = (x + width * y);
			unsigned char* pixelOffset = textureData + index * channels;
			float height = pixelOffset[0];
			
			m_heightmapCoords[index].x = (float)x;
			m_heightmapCoords[index].z = -(float)y;
			// Make z positive range
			m_heightmapCoords[index].z += (float)(m_terrainHeight - 1);

			m_heightmapCoords[index].y = height / m_heightScaleDivider;
		}
	}

}

glm::vec3 Terrain::calculateNormals(TerrainData v1, TerrainData v2, TerrainData v3)
{
	glm::vec3 vec1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 vec2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
	glm::vec3 normal = glm::cross(vec2, vec1);
	normal = glm::normalize(normal);
	return normal;
}

void Terrain::smoothNormals()
{
	// Calculate normals
	for (size_t y = 0; y < m_terrainHeight; y++)
	{
		for (size_t x = 0; x < m_terrainWidth; x++)
		{
			size_t ul_index = (m_terrainWidth * (y)) + (x);
			size_t bl_index = (m_terrainWidth * (y + 1)) + (x);
			size_t br_index = (m_terrainWidth * (y + 1)) + (x + 1);

			glm::vec3 normal;
			if (y >= m_terrainHeight - 1 && x >= m_terrainWidth - 1)
			{
				bl_index = (m_terrainWidth * (y - 1)) + (x);
				br_index = (m_terrainWidth * (y - 1)) + (x - 1);
				normal = calculateNormals(m_heightmapCoords[br_index], m_heightmapCoords[ul_index], m_heightmapCoords[bl_index]);
			}
			else if (y >= m_terrainHeight - 1)
			{
				bl_index = (m_terrainWidth * (y - 1)) + (x);
				br_index = (m_terrainWidth * (y - 1)) + (x + 1);
				normal = calculateNormals(m_heightmapCoords[bl_index], m_heightmapCoords[ul_index], m_heightmapCoords[br_index]);
			}
			else if (x >= m_terrainWidth - 1)
			{
				br_index = (m_terrainWidth * (y + 1)) + (x - 1);
				normal = calculateNormals(m_heightmapCoords[br_index], m_heightmapCoords[bl_index], m_heightmapCoords[ul_index]);
			}
			else
			{
				normal = calculateNormals(m_heightmapCoords[ul_index], m_heightmapCoords[bl_index], m_heightmapCoords[br_index]);
			}

			m_heightmapCoords[ul_index].nx = normal.x;
			m_heightmapCoords[ul_index].ny = normal.y;
			m_heightmapCoords[ul_index].nz = normal.z;
		}
	}
}

void Terrain::generateTerrain()
{
	m_data = new ParserData();
	int index = 0;
	size_t ul_index; // upper left
	size_t bl_index; // bottom left
	size_t br_index; // bottom right
	size_t ur_index; // upper right
	
	for (size_t y = 0; y < m_terrainHeight - 1; y++)
	{
		for (size_t x = 0; x < m_terrainWidth - 1; x++)
		{
			ul_index = (m_terrainWidth * (y)) + (x);
			bl_index = (m_terrainWidth * (y + 1)) + (x);
			br_index = (m_terrainWidth * (y + 1)) + (x + 1);
			ur_index = (m_terrainWidth * (y)) + (x + 1);
		
			// ---------------FIRST TRIANGLE---------------
			// Upper left
			m_data->addVertex(m_heightmapCoords[ul_index].x, m_heightmapCoords[ul_index].y, m_heightmapCoords[ul_index].z);
			m_data->addUV(
				(float)((x) / (float)(m_terrainWidth - 1.f)),
				1.f - (float)((y) / (float)(m_terrainWidth - 1.f))
			);
			
			m_data->addNormal(m_heightmapCoords[ul_index].nx, m_heightmapCoords[ul_index].ny, m_heightmapCoords[ul_index].nz);
			m_data->addIndices(index++);

			// bottom left
			m_data->addVertex(m_heightmapCoords[bl_index].x, m_heightmapCoords[bl_index].y, m_heightmapCoords[bl_index].z);
			m_data->addUV(
				(float)((x) / (float)(m_terrainWidth - 1.f)),
				1.f - (float)((y + 1) / (float)(m_terrainWidth - 1.f))
			);
			m_data->addNormal(m_heightmapCoords[bl_index].nx, m_heightmapCoords[bl_index].ny, m_heightmapCoords[bl_index].nz);
			m_data->addIndices(index++);

			// bottom right
			m_data->addVertex(m_heightmapCoords[br_index].x, m_heightmapCoords[br_index].y, m_heightmapCoords[br_index].z);
			m_data->addUV(
				(float)((x + 1) / (float)(m_terrainWidth - 1.f)),
				1.f - (float)((y + 1) / (float)(m_terrainWidth - 1.f))
			);
			m_data->addNormal(m_heightmapCoords[br_index].nx, m_heightmapCoords[br_index].ny, m_heightmapCoords[br_index].nz);
			m_data->addIndices(index++);



			//--------------- SECOND TRIANGLE---------------
			// bottom right
			m_data->addVertex(m_heightmapCoords[br_index].x, m_heightmapCoords[br_index].y, m_heightmapCoords[br_index].z);
			m_data->addUV(
				(float)((x + 1) / (float)(m_terrainWidth - 1.f)),
				1.f - (float)((y + 1) / (float)(m_terrainWidth - 1.f))
			);
			m_data->addNormal(m_heightmapCoords[br_index].nx, m_heightmapCoords[br_index].ny, m_heightmapCoords[br_index].nz);
			m_data->addIndices(index++);

			// Upper right
			m_data->addVertex(m_heightmapCoords[ur_index].x, m_heightmapCoords[ur_index].y, m_heightmapCoords[ur_index].z);
			m_data->addUV(
				(float)((x + 1) / (float)(m_terrainWidth - 1.f)),
				1.f - (float)((y) / (float)(m_terrainWidth - 1.f))
			);
			m_data->addNormal(m_heightmapCoords[ur_index].nx, m_heightmapCoords[ur_index].ny, m_heightmapCoords[ur_index].nz);
			m_data->addIndices(index++);

			// Upper left
			m_data->addVertex(m_heightmapCoords[ul_index].x, m_heightmapCoords[ul_index].y, m_heightmapCoords[ul_index].z);
			m_data->addUV(
				(float)((x) / (float)(m_terrainWidth - 1.f)),
				1.f - (float)((y) / (float)(m_terrainWidth - 1.f))
			);
			m_data->addNormal(m_heightmapCoords[ul_index].nx, m_heightmapCoords[ul_index].ny, m_heightmapCoords[ul_index].nz);
			m_data->addIndices(index++);
		}

	}

	std::vector<std::string> textures;
	textures.emplace_back("Resources/Textures/bmap.png");
	textures.emplace_back("Resources/Textures/grass.png");
	textures.emplace_back("Resources/Textures/muddy.png");
	textures.emplace_back("Resources/Textures/stone.png");
	textures.emplace_back("Resources/Textures/granite.png");
	std::vector<GLuint> textureIDS = m_loader->createTexture(textures);
	
	int cellWidth = 8;
	int cellHeight = 8;
	int cellCount = (m_terrainWidth) / (cellWidth);
	m_chunkManager->reserveVectorMemory(cellCount * cellCount);

	for (int z = 0; z < cellCount; z++)
	{
		for (int x = 0; x < cellCount; x++)
		{
			generateTerrainCells(x, z, cellWidth, cellHeight, cellCount, textureIDS);
		}
	}


}

void Terrain::generateTerrainCells(int nodeX, int nodeY, int cellWidth, int cellHeight, int cellCount, std::vector<GLuint> textureIDs)
{
	ParserData* cellData = new ParserData();

	int actualWidth = (m_terrainWidth - 1) * 18; // 6 vertices adn 3 floats per vertices = 18
	int actualCellwidth = cellWidth * 18;

	int uvWidth = (m_terrainWidth - 1) * 12; // 6 vertices and 2 floats
	int uvCellWidth = cellWidth * 12;

	int currentIndex = ((nodeX * actualCellwidth) + actualWidth * (nodeY * cellHeight));
	int currentUVIndex = ((nodeX * uvCellWidth) + uvWidth * (nodeY * cellHeight));
	int index = 0;

	int row = cellWidth;
	int col = cellHeight;
	
	for (int z = 0; z < cellHeight; z++)
	{
		// To keep it from oversampling on the absolute last iteration.
		// I don't know why it does that..
		if (nodeX == cellCount - 1 && z == cellHeight - 1)
			row = cellWidth - 1;

		if (nodeY == cellCount - 1 && z == cellHeight - 1)
			break;

		for (int x = 0; x < row; x++) {
			// The absolute last iteration is somehow oversampling outside the vector bounds so just break
			if (nodeX == cellCount - 1 && x == row - 1 && nodeY == cellCount - 1 && z == cellHeight - 2)
				break;


			// This is 1 by 1 square
			cellData->addVertex(m_data->getVertices().at(currentIndex), m_data->getVertices().at(currentIndex + 1), m_data->getVertices().at(currentIndex + 2));
			cellData->addVertex(m_data->getVertices().at(currentIndex + 3), m_data->getVertices().at(currentIndex + 4), m_data->getVertices().at(currentIndex + 5));
			cellData->addVertex(m_data->getVertices().at(currentIndex + 6), m_data->getVertices().at(currentIndex + 7), m_data->getVertices().at(currentIndex + 8));

			cellData->addVertex(m_data->getVertices().at(currentIndex + 9), m_data->getVertices().at(currentIndex + 10), m_data->getVertices().at(currentIndex + 11));
			cellData->addVertex(m_data->getVertices().at(currentIndex + 12), m_data->getVertices().at(currentIndex + 13), m_data->getVertices().at(currentIndex + 14));
			cellData->addVertex(m_data->getVertices().at(currentIndex + 15), m_data->getVertices().at(currentIndex + 16), m_data->getVertices().at(currentIndex + 17));
			//---------------------------

			cellData->addNormal(m_data->getNormals().at(currentIndex), m_data->getNormals().at(currentIndex + 1), m_data->getNormals().at(currentIndex + 2));
			cellData->addNormal(m_data->getNormals().at(currentIndex + 3), m_data->getNormals().at(currentIndex + 4), m_data->getNormals().at(currentIndex + 5));
			cellData->addNormal(m_data->getNormals().at(currentIndex + 6), m_data->getNormals().at(currentIndex + 7), m_data->getNormals().at(currentIndex + 8));

			cellData->addNormal(m_data->getNormals().at(currentIndex + 9), m_data->getNormals().at(currentIndex + 10), m_data->getNormals().at(currentIndex + 11));
			cellData->addNormal(m_data->getNormals().at(currentIndex + 12), m_data->getNormals().at(currentIndex + 13), m_data->getNormals().at(currentIndex + 14));
			cellData->addNormal(m_data->getNormals().at(currentIndex + 15), m_data->getNormals().at(currentIndex + 16), m_data->getNormals().at(currentIndex + 17));

			cellData->addUV(m_data->getUvs().at(currentUVIndex), m_data->getUvs().at(currentUVIndex + 1));
			cellData->addUV(m_data->getUvs().at(currentUVIndex + 2), m_data->getUvs().at(currentUVIndex + 3));
			cellData->addUV(m_data->getUvs().at(currentUVIndex + 4), m_data->getUvs().at(currentUVIndex + 5));
			
			cellData->addUV(m_data->getUvs().at(currentUVIndex + 6), m_data->getUvs().at(currentUVIndex + 7));
			cellData->addUV(m_data->getUvs().at(currentUVIndex + 8), m_data->getUvs().at(currentUVIndex + 9));
			cellData->addUV(m_data->getUvs().at(currentUVIndex + 10), m_data->getUvs().at(currentUVIndex + 11));


			cellData->addIndices(index++);
			cellData->addIndices(index++);
			cellData->addIndices(index++);

			cellData->addIndices(index++);
			cellData->addIndices(index++);
			cellData->addIndices(index++);
			currentIndex += 18;
			currentUVIndex += 12;
		}
		currentIndex += (actualWidth - actualCellwidth);
		currentUVIndex += (uvWidth - uvCellWidth);
	}
	
	TerrainChunk* chunk = new TerrainChunk(cellData, m_loader, textureIDs, cellWidth);
	m_chunkManager->addChunk(chunk); 
	m_quadTree->insert(chunk);
}
