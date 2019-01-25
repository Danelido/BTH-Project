#include "Terrain.h"
#include <iostream>
#include "Utility/MatrixCreator.h"
#include "Vendor/stb/stb_image.h"

Terrain::Terrain(Loader* loader)
{
	m_loader = loader;
	m_terrainMesh = nullptr;
	m_terrainWidth = 256;
	m_terrainHeight = 256;
	m_modelMatrix = MatrixCreator::createNewModelMatrix(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
	m_size = 1.f;
	m_heightScaleDivider = 14;
	GLint width, height, channels;
	const char* filename = "Resources/Textures/heightmap.png";
	stbi_set_flip_vertically_on_load(0);
	unsigned char* textureData = stbi_load(filename, &width, &height, &channels, STBI_rgb);

	if (textureData && width == m_terrainWidth && height == m_terrainHeight)
	{
		loadHeightsFromHeightmap(textureData, width,height,channels);
		setTerrainCoordinates();
		smoothNormals();
		generateTerrain();
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

Mesh * Terrain::getTerrainMesh()
{
	return m_terrainMesh;
}

float Terrain::getHeight(const glm::vec3& position)
{
	// Make this smoother jeesus
	if (position.x > 0.f && position.x < (float)m_terrainWidth - 1 && position.z > 0.f && position.z < (float)m_terrainWidth - 1)
	{
		int x = ((float)position.x / m_terrainWidth) * m_terrainWidth;
		int z = (1.f - (position.z / m_terrainWidth)) * m_terrainWidth;
		float y = m_heightmapCoords[x + (z * m_terrainWidth)].y;
		return (y + .5f); // 0.5f offset to make it a little bit above ground

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
			m_heightmapCoords[index].y = height;
		}
	}

}

void Terrain::setTerrainCoordinates()
{
	for (int y = 0; y < m_terrainHeight; y++)
	{
		for (int x = 0; x < m_terrainWidth; x++)
		{
			int index = x + (m_terrainWidth * y);
			m_heightmapCoords[index].x = (float)x;
			m_heightmapCoords[index].z = -(float)y;

			// Make z positive range
			m_heightmapCoords[index].z += (float)(m_terrainHeight - 1);
			
			// scale height
			m_heightmapCoords[index].y /= m_heightScaleDivider;
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
	for (int y = 0; y < m_terrainHeight; y++)
	{
		for (int x = 0; x < m_terrainWidth; x++)
		{
			int ul_index = (m_terrainWidth * (y)) + (x);
			int bl_index = (m_terrainWidth * (y + 1)) + (x);
			int br_index = (m_terrainWidth * (y + 1)) + (x + 1);

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
	int ul_index; // upper left
	int bl_index; // bottom left
	int br_index; // bottom right
	int ur_index; // upper right

	for (int y = 0; y < m_terrainHeight - 1; y++)
	{
		for (int x = 0; x < m_terrainWidth - 1; x++)
		{
			ul_index = (m_terrainWidth * (y)) + (x);
			bl_index = (m_terrainWidth * (y + 1)) + (x);
			br_index = (m_terrainWidth * (y + 1)) + (x + 1);
			ur_index = (m_terrainWidth * (y)) + (x + 1);
				
			// FIRST TRIANGLE
			// Upper left
			m_data->addVertex(m_heightmapCoords[ul_index].x, m_heightmapCoords[ul_index].y, m_heightmapCoords[ul_index].z);
			m_data->addUV(
				(float)((x) / (float)(m_terrainWidth - 1.f)),
				1.f - (float)((y) / (float)(m_terrainWidth - 1.f))
			);
			//std::cout << "UV: " << (float)((x) / (float)m_terrainWidth) << ", " << 1.f - (float)((y) / (float)m_terrainWidth) << "\n";
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

			// SECOND TRIANGLE
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
	textures.emplace_back("blendmap2.png");
	textures.emplace_back("grass.png");
	textures.emplace_back("muddy.png");
	textures.emplace_back("stone.png");
	textures.emplace_back("granite.png");

	m_terrainMesh = m_loader->createTerrainMesh(m_data, textures);

}
