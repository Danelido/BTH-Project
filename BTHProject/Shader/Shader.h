#ifndef _SHADER_H
#define _SHADER_H
#include <GL/glew.h>
#include <string>
#include <vector>

class Shader
{
public:
	Shader();
	virtual ~Shader();
	bool loadShaders(const char* vertex, const char* fragment, const char* geometry);
	void use();
	void unuse();
	const GLuint& program() const;

private:
	GLuint m_program;

	bool checkError(GLuint shader);
	void createAndLink(const GLuint* vs, const GLuint* fs, const GLuint* gs);
	std::string getData(const char* shaderfile);
	GLuint createShader(GLenum shaderType, const char* data);
};

#endif
