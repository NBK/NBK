#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "glew.h"
#include <stdio.h>
#include <windows.h>
#include <gl\gl.h>

#include <map>
#include <string>
#include "ShaderObject.h"

namespace shaders
{
	class CShaderManager
	{
	public:
		CShaderManager();
		~CShaderManager();
		
		GLvoid	useShaderProgram(GLint shaderIndex);
		GLvoid	useShaderProgram(GLhandleARB shaderHandle);
		GLvoid	useFFPipeline();

		bool			addShader(GLint shaderIndex, std::string vertexFile, std::string fragmentFile);
		GLhandleARB		getShaderProgram(GLint shaderIndex);
		CShaderObject	*getShaderObject(GLint shaderIndex);

		std::string getLastError();

	private:

		// variables:

		std::string lastError;

		struct ShaderData
		{
			GLhandleARB handle;
			CShaderObject object;

			ShaderData()
			{
				handle=NULL;
				object=NULL;
			}

			ShaderData(GLhandleARB handle)
			{
				this->handle=handle;
				this->object=CShaderObject(handle);
			}
		};

		std::map<GLint, ShaderData> shaderPrograms;

		// functions:
		std::string textFileRead(std::string fileName);
		bool printInfoLog(GLhandleARB obj);

	};
};

#endif // SHADER_MANAGER_H