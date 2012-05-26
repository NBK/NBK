#include "ShaderObject.h"

namespace shaders
{
	CShaderObject::CShaderObject()
	{
		handle=0;
	}

	CShaderObject::CShaderObject(GLhandleARB handle)
	{
		this->handle=handle;
	}

	CShaderObject::~CShaderObject()
	{
	}

	GLint CShaderObject::getUniformLocation(const char *variableName)
	{
		return glGetUniformLocationARB(handle,variableName);
	}

	GLvoid CShaderObject::setUniform1i(GLint variablePos, GLint val)
	{
		glUniform1iARB(variablePos,val);
	}

	GLvoid CShaderObject::setUniform1i(const char *variableName, GLint val)
	{
		glUniform1iARB(getUniformLocation(variableName),val);
	}

	GLvoid CShaderObject::setUniform1f(GLint variablePos, GLfloat val)
	{
		glUniform1fARB(variablePos,val);
	}

	GLvoid CShaderObject::setUniform1f(const char *variableName, GLfloat val)
	{
		glUniform1fARB(getUniformLocation(variableName),val);
	}
}