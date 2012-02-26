#include "ShaderManager.h"
#include "utils.h"

using namespace std;

namespace shaders
{
	CShaderManager::CShaderManager()
	{
		glewInit();
	}

	CShaderManager::~CShaderManager()
	{
	}

	bool CShaderManager::addShader(GLint shaderIndex, string vertexFile, string fragmentFile)
	{
		GLhandleARB v,f;

		v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

		string _v = textFileRead(vertexFile);
		string _f = textFileRead(fragmentFile);

		if (_v=="" || _f=="")
		{
			lastError = "Shader files missing or invalid!";
			return false;
		}

		const char * vv = _v.c_str();
		const char * ff = _f.c_str();

		glShaderSourceARB(v,1,&vv,NULL);
		glShaderSourceARB(f,1,&ff,NULL);

		glCompileShaderARB(v);
		glCompileShaderARB(f);

		GLhandleARB shaderProgram = glCreateProgramObjectARB();

		glAttachObjectARB(shaderProgram,v);
		glAttachObjectARB(shaderProgram,f);

		glLinkProgramARB(shaderProgram);

		shaderPrograms[shaderIndex]=ShaderData(shaderProgram);

		return printInfoLog(shaderProgram);
	}

	GLvoid CShaderManager::useShaderProgram(GLint shaderIndex)
	{
		glUseProgramObjectARB(shaderPrograms[shaderIndex].handle);
	}

	GLvoid CShaderManager::useShaderProgram(GLhandleARB shaderHandle)
	{
		glUseProgramObjectARB(shaderHandle);
	}

	GLhandleARB CShaderManager::getShaderProgram(GLint shaderIndex)
	{
		return shaderPrograms[shaderIndex].handle;
	}

	CShaderObject *CShaderManager::getShaderObject(GLint shaderIndex)
	{
		return &shaderPrograms[shaderIndex].object;
	}

	string CShaderManager::textFileRead(string fileName)
	{
		FILE *fp;
		char *content = NULL;

		int count=0;
		if (fileName.length()>0)
		{
			const char *fn = fileName.c_str();
			fp = fopen(fn,"rc");

			if (fp != NULL)
			{
				fseek(fp, 0, SEEK_END);
				count = ftell(fp);
				rewind(fp);

				if (count > 0)
				{
					content = new char[sizeof(char) * (count+1)];
					count = fread(content,sizeof(char),count,fp);
					content[count] = '\0';
				}
				fclose(fp);
			}
			else
			{
				fprintf(stderr,"Can't read %s shader file\n",fn);
			}
		}

		string shaderSource = (content?string(content):"");

		delete [] content;

		return shaderSource;
	}

	GLvoid CShaderManager::useFFPipeline()
	{
		glUseProgramObjectARB(0);
	}

	bool CShaderManager::printInfoLog(GLhandleARB obj)
	{
	    int infologLength = 0;
	    int charsWritten  = 0;
	    char *infoLog;

	    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

	    if (infologLength > 1)
	    {
			infoLog = new char[infologLength];
			glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
			//printf("%s\n",infoLog);
			lastError=string(infoLog);
			SAFE_DELETE_ARRAY(infoLog);

			return false;
	    }
		return true;
	}

	string CShaderManager::getLastError()
	{
		return lastError;
	}
};
