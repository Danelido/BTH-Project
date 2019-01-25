#include "Loader.h"
#include "Vendor/stb/stb_image.h"
#include "Utility/MeshMemoryCollector.h"

Loader::Loader()
{
}

Loader::~Loader()
{
	for (auto &id : this->m_texIDs) {
		glDeleteTextures(1, id);
	}

	for (auto &id : this->m_vbos) {
		glDeleteBuffers(1, id);
	}

	for (auto &id : this->m_vaos) {
		glDeleteVertexArrays(1, id);
	}
}

Mesh * Loader::createMesh(ParserData * data)
{
	GLuint VAO = createAndBindVAO();
	bindIndices(data->getIndices());
	storeDataInAttributeList(0, 3, data->getVertices());
	storeDataInAttributeList(1, 3, data->getNormals());
	storeDataInAttributeList(2, 2, data->getUvs());
	unbindVAO();

	std::vector<GLuint> textures;
	for (int i = 0; i < data->getTextureFilenames().size(); i++) {
		textures.emplace_back(createTexture(data->getTextureFilenames()[i]));
	}

	Mesh* mesh = new Mesh(VAO, textures, data->getAmbientColor(), data->getDiffuseColor(), data->getSpecularColor(), data->getShininess(), (GLuint)data->getIndices().size());
	MeshMemoryCollector::registerMesh(mesh);
	return mesh;
}

Mesh * Loader::createTerrainMesh(ParserData * data, std::vector<std::string> textureNames)
{
	GLuint VAO = createAndBindVAO();
	bindIndices(data->getIndices());
	storeDataInAttributeList(0, 3, data->getVertices());
	storeDataInAttributeList(1, 3, data->getNormals());
	storeDataInAttributeList(2, 2, data->getUvs());
	unbindVAO();

	std::vector<GLuint> textures;
	for (int i = 0; i < textureNames.size(); i++) {
		textures.emplace_back(createTexture("Resources/Textures/" + textureNames[i]));
	}
	Mesh* mesh = new Mesh(VAO, textures, data->getAmbientColor(), data->getDiffuseColor(), data->getSpecularColor(), data->getShininess(), (GLuint)data->getIndices().size());
	MeshMemoryCollector::registerMesh(mesh);
	return mesh;
}

InstancedMesh * Loader::createInstancedMesh(ParserData * data)
{
	GLuint VAO = createAndBindVAO();
	bindIndices(data->getIndices());
	storeDataInAttributeList(0, 3, data->getVertices());
	storeDataInAttributeList(1, 3, data->getNormals());
	storeDataInAttributeList(2, 2, data->getUvs());
	GLuint emptyVBO = createEmptyModelMatrixBuffer(3);
	unbindVAO();

	std::vector<GLuint> textures;
	for (int i = 0; i < data->getTextureFilenames().size(); i++) {
		textures.emplace_back(createTexture(data->getTextureFilenames()[i]));
	}

	InstancedMesh* instancedMesh = new InstancedMesh(VAO, textures, data->getAmbientColor(), data->getDiffuseColor(), data->getSpecularColor(), data->getShininess(), (GLuint)data->getIndices().size(), emptyVBO);
	MeshMemoryCollector::registerMesh(instancedMesh);
	return instancedMesh;
}

GLuint Loader::createAndBindVAO()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	m_vaos.emplace_back(&VAO);
	return VAO;
}

GLuint Loader::createTexture(std::string filename)
{
	GLint width, height, channels;
	GLuint textureID;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* textureData = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb);

	if (!textureData)
	{
		printf("Failed to load texture %s\n", filename.c_str());
		return -1;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, NULL);
	stbi_image_free(textureData);
	m_texIDs.emplace_back(&textureID);

	return textureID;

}

void Loader::bindIndices(const std::vector<GLuint>& indices)
{
	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	m_vbos.emplace_back(&IBO);
}

void Loader::storeDataInAttributeList(const GLuint & attributeNumber, const GLuint & dataPerVertex, const std::vector<GLfloat>& data)
{
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, dataPerVertex, GL_FLOAT, GL_FALSE, NULL, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	m_vbos.emplace_back(&VBO);
}

GLuint Loader::createEmptyModelMatrixBuffer(const GLuint & attributeNumber)
{
	
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (16) * sizeof(float) * 10, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(attributeNumber + 0, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
	glVertexAttribPointer(attributeNumber + 1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribPointer(attributeNumber + 2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
	glVertexAttribPointer(attributeNumber + 3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));

	glVertexAttribDivisor(attributeNumber + 0, 1);
	glVertexAttribDivisor(attributeNumber + 1, 1);
	glVertexAttribDivisor(attributeNumber + 2, 1);
	glVertexAttribDivisor(attributeNumber + 3, 1);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	m_vbos.emplace_back(&VBO);
	return VBO;

}

void Loader::unbindVAO()
{
	glBindVertexArray(NULL);
}
