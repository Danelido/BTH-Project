#ifndef _PARSER_OBJ_H
#define _PARSER_OBJ_H
#include "ParserData.h"


class Parser
{
public:
	Parser();
	~Parser();

	ParserData* parseFile(const std::string& filepath);
	void cleanUp();

private:

	std::vector<std::string> split(const std::string& line, const char c);
	bool parseMTL(const std::string& filename, ParserData* parserData);
	void extractFilepath(const std::string& filepath);
	void processFace(GLuint val1, GLuint val2, GLuint val3, GLuint& indexCounter,
		const std::vector<GLfloat>& vertices,
		const std::vector<GLfloat>& uvs,
		const std::vector<GLfloat>& normals,
		ParserData* parserData);

private:
	std::string m_filepath;
	std::string m_textureFilePath = "Resources/Textures/";
	// Used to keep all the pointer in one place to make it easy to delete them all
	std::vector<ParserData*> m_tracker;


};

#endif
