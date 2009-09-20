#include "ShaderObject.h"

namespace shaders
{
	CShaderObject::CShaderObject()
	{
		handle=NULL;
	}

	CShaderObject::CShaderObject(GLhandleARB handle)
	{
		this->handle=handle;
	}

	CShaderObject::~CShaderObject()
	{
	}

	GLint CShaderObject::getUniformLocation(char *variableName)
	{
		return glGetUniformLocationARB(handle,variableName);
	}

	GLvoid CShaderObject::setUniform1i(GLint variablePos, GLint val)
	{
		glUniform1iARB(variablePos,val);
	}

	GLvoid CShaderObject::setUniform1i(char *variableName, GLint val)
	{
		glUniform1iARB(getUniformLocation(variableName),val);
	}

	GLvoid CShaderObject::setUniform1f(GLint variablePos, GLfloat val)
	{
		glUniform1fARB(variablePos,val);
	}

	GLvoid CShaderObject::setUniform1f(char *variableName, GLfloat val)
	{
		glUniform1fARB(getUniformLocation(variableName),val);
	}
}