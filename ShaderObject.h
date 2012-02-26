#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "glew.h"
#include "system.h"

namespace shaders
{
	class CShaderObject
	{
	public:
		CShaderObject();
		CShaderObject(GLhandleARB handle);
		~CShaderObject();

		GLint getUniformLocation(const char *variableName);

		GLvoid setUniform1i(GLint variablePos, GLint val);
		GLvoid setUniform1i(const char *variableName, GLint val);

		GLvoid setUniform1f(GLint variablePos, GLfloat val);
		GLvoid setUniform1f(const char *variableName, GLfloat val);

	private:
		GLhandleARB handle;
	};
};

#endif // SHADER_OBJECT_H