#ifndef _QUAD_H
#define _QUAD_H
#include <GL/glew.h>

class Quad
{
public:
	Quad();
	~Quad();
	
	void create();
	void bind();
	const GLuint& getNumVertices() const;
	void unbind();

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_numVertices;

private:

	float data[24] = {
		   // Pos        //Uv
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,
	};
};
#endif
