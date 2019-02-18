#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define CAPACITY 250000

Parser::Parser()
{
}

Parser::~Parser()
{
	cleanUp(); // Will run if it hasn't been called already
}

ParserData * Parser::parseFile(const std::string & filepath)
{
	extractFilepath(filepath);

	std::vector<GLfloat> tempVertexBuffer;
	tempVertexBuffer.reserve(CAPACITY);

	std::vector<GLfloat> tempUVBuffer;
	tempUVBuffer.reserve(CAPACITY);

	std::vector<GLfloat> tempNormalBuffer;
	tempNormalBuffer.reserve(CAPACITY);

	std::ifstream objFile(filepath);
	if (!objFile.is_open())
	{
		printf("Error opening obj file %s\n", filepath.c_str());
		return nullptr;
	}

	ParserData* data = new ParserData();
	std::string line;
	std::string MTLfile = "";
	GLuint indexCount = 0;


	while (std::getline(objFile, line))
	{
		std::vector<std::string> attribs = split(line, ' ');

		if (attribs.size() == 0)
			continue;

		if (attribs[0] == "mtllib")
		{
			MTLfile = attribs[1];
		}
		else if (attribs[0] == "v")
		{
			for(int i = 1; i < 4; i++)
				tempVertexBuffer.emplace_back(std::stof(attribs[i]));
			
		}
		else if (attribs[0] == "vt")
		{
			for (int i = 1; i < 3; i++)
				tempUVBuffer.emplace_back(std::stof(attribs[i]));
		}
		else if (attribs[0] == "vn")
		{
			for (int i = 1; i < 4; i++)
				tempNormalBuffer.emplace_back(std::stof(attribs[i]));
			
		}
		else if (attribs[0] == "f")
		{
			std::vector<std::string> attributes = split(line, ' ');
			for (int i = 0; i < 3; i++)
			{
				std::vector<std::string> currFace = split(attributes[i + 1], '/');
				GLuint x = static_cast<GLuint>(std::stoi(currFace[0]));
				GLuint y = static_cast<GLuint>(std::stoi(currFace[1]));
				GLuint z = static_cast<GLuint>(std::stoi(currFace[2]));
				processFace(x, y, z, indexCount, tempVertexBuffer, tempUVBuffer, tempNormalBuffer, data);
			}
		}


	}

	objFile.close();
	tempVertexBuffer.clear();
	tempUVBuffer.clear();
	tempNormalBuffer.clear();

	parseMTL(MTLfile, data);
	m_tracker.emplace_back(data);
	
	return data;
}

void Parser::cleanUp()
{
	for (size_t i = 0; i < m_tracker.size(); i++)
	{
		delete m_tracker[i];
	}
	m_tracker.clear();
}


std::vector<std::string> Parser::split(const std::string & line, const char c)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(line);
	while (std::getline(tokenStream, token, c))
	{
		tokens.emplace_back(token);
	}
	return tokens;
}

bool Parser::parseMTL(const std::string & filename, ParserData* parserData)
{
	
	std::ifstream mtlFile(m_filepath + filename);
	if (!mtlFile.is_open())
	{
		printf("Error opening mtl file %s\n", filename.c_str());
		return false;
	}
	std::string line;
	while (std::getline(mtlFile, line))
	{

		std::vector<std::string> attribs = split(line, ' ');
		if (attribs.size() == 0)
			continue;


		if (attribs[0] == "Ka")
		{
			parserData->setAmbientColor(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
		}
		else if (attribs[0] == "Kd")
		{
			parserData->setDiffuseColor(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
		}
		else if (attribs[0] == "Ks")
		{
			parserData->setSpecularColor(std::stof(attribs[1]), std::stof(attribs[2]), std::stof(attribs[3]));
		}
		else if (attribs[0] == "Ns")
		{
			parserData->setShininess(std::stof(attribs[1]));
		}
		else if (attribs[0] == "map_Kd")
		{
			parserData->addTextureFilename(m_textureFilePath + attribs[1]);
		}

	}

	mtlFile.close();

	return true;
}

void Parser::extractFilepath(const std::string & filepath)
{
	m_filepath = "";
	std::vector<std::string> splittedPath = split(filepath, '/');
	if (splittedPath.size() > 0)
	{
		for (size_t i = 0; i < splittedPath.size() - 1; i++)
		{
			m_filepath += splittedPath[i] + "/";
		}
	}

}

void Parser::processFace(GLuint val1, GLuint val2, GLuint val3, GLuint & indexCounter,
	const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& uvs, const std::vector<GLfloat>& normals,
	ParserData* parserData)
{
	int vertexStartPos = (val1 - 1) * 3;
	int uvStartPos = (val2 - 1) * 2;
	int normalStartPos = (val3 - 1) * 3;
	
	parserData->addIndices(indexCounter++);
	parserData->addVertex(vertices[vertexStartPos], vertices[vertexStartPos + 1], vertices[vertexStartPos + 2]);
	parserData->addUV(uvs[uvStartPos], uvs[uvStartPos + 1]);
	parserData->addNormal(normals[normalStartPos], normals[normalStartPos + 1], normals[normalStartPos + 2]);
}
