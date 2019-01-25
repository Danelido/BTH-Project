#include "Shader.h"
#include <fstream>

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::loadShaders(const char * vertex, const char * fragment, const char * geometry)
{
	std::string vsData = getData(vertex);
	GLuint vs = createShader(GL_VERTEX_SHADER, vsData.c_str());
	if (checkError(vs))
		return false;


	std::string fsData = getData(fragment);
	GLuint fs = createShader(GL_FRAGMENT_SHADER, fsData.c_str());
	if (checkError(fs))
		return false;

	if (geometry != nullptr) {
		std::string gsData = getData(geometry);
		GLuint gs = createShader(GL_GEOMETRY_SHADER, gsData.c_str());
		if (checkError(gs))
			return false;

		createAndLink(&vs, &fs, &gs);
	}
	else
	{
		createAndLink(&vs, &fs, nullptr);
	}
	

	return true;
}

void Shader::use()
{
	glUseProgram(m_program);
}

void Shader::unuse()
{
	glUseProgram(NULL);
}

const GLuint & Shader::program() const
{
	return m_program;
}

bool Shader::checkError(GLuint shader)
{
	// Error checking
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shader, 512, 0, buffer);
		printf("%s\n", buffer);
		return true;
	}

	return false;
}

void Shader::createAndLink(const GLuint* vs, const GLuint* fs, const GLuint* gs)
{
	this->m_program = glCreateProgram();
	glAttachShader(m_program, *vs);
	glAttachShader(m_program, *fs);
	if(gs != nullptr)
		glAttachShader(m_program, *gs);
	glLinkProgram(m_program);

	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);
		printf("%s\n", infoLog.data());
		// The program is useless now. So delete it.
		glDeleteProgram(m_program);

	}
}

std::string Shader::getData(const char * shaderfile)
{
	std::ifstream file(shaderfile);
	if (!file.is_open())
	{
		printf("%sFailed to open file: \n", shaderfile);
		return "\0";
	}
	else
	{
		std::string content;
		std::string line;
		while (std::getline(file, line)) {
			content += line + "\n";
		}

		file.close();

		return content;
	}
}

GLuint Shader::createShader(GLenum shaderType, const char * data)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &data, 0);
	glCompileShader(shader);

	return shader;
}
