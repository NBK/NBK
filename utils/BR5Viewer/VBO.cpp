#include "VBO.h"
#include "OGLUtils.h"

using namespace utils;
using namespace std;

namespace rendering
{
	CVBO::CVBO(BUFFER_TYPE bufferType, bool clean)
	{
		this->bufferType=bufferType;	
		this->clean=clean;

		for (GLint i=0; i<MAX_ID; i++)
		{
			VBOs[i].id=0;
			reUpload[i]=true;
		}

		enumMode = EM_TRIANGLES;

		glewInit();
	}

	CVBO::~CVBO()
	{
		for (GLint i=0; i<MAX_ID; i++)
		{
			if (VBOs[i].id>0)
			{
				glDeleteBuffersARB(1,&VBOs[i].id);

				if (clean && VBOs[i].data)
				{
					delete VBOs[i].data;
				}
			}
		}
	}

	GLvoid CVBO::setVertexData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType)
	{
		setData(elementsCount,elementLength,elementSizeInBytes,data,dataType,IDT_vertex);
	}

	GLvoid CVBO::setColorData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType)
	{
		setData(elementsCount,elementLength,elementSizeInBytes,data,dataType,IDT_color);
	}

	GLvoid CVBO::setTextureData(TEXTURE_UNIT textureUnit, GLuint texture, GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType)
	{	
		textures[textureUnit]=texture;
		setData(elementsCount,elementLength,elementSizeInBytes,data,dataType,(ID_TYPE)((GLint)IDT_texture0+(GLint)textureUnit));	
	}

	GLvoid CVBO::setTextureOnly(TEXTURE_UNIT textureUnit, GLuint texture)
	{	
		textures[textureUnit]=texture;
	}

	GLvoid CVBO::setNormalData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType)
	{
		setData(elementsCount,elementLength,elementSizeInBytes,data,dataType,IDT_normal);
	}

	GLvoid CVBO::setData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType, ID_TYPE ID_type)
	{
		/*if (VBOs[ID_type].id!=0)
		{
			glDeleteBuffersARB(1,&VBOs[ID_type].id);
			VBOs[ID_type].id=0;
		}*/

		//if (VBOs[ID_type].id==0)
		{
			glGenBuffersARB(1,&VBOs[ID_type].id);
		}

		VBOs[ID_type].elementsCount=elementsCount;
		VBOs[ID_type].elementLength=elementLength;
		VBOs[ID_type].elementSizeInBytes=elementSizeInBytes;
		VBOs[ID_type].data=data;	
		VBOs[ID_type].dataType=dataType;	
		VBOs[ID_type].calcSize();

		//if (bufferType==BT_STATIC_DRAW) needed with dynamic where only sub data gets replaced!
		{
			// if it's static then we just need to load it once
			if (ID_type==IDT_vertex)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].id);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].size, VBOs[ID_type].data, bufferType);
			}
			else if (ID_type==IDT_color)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].id);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].size, VBOs[ID_type].data, bufferType);
			}
			else if (ID_type==IDT_normal)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].id);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].size, VBOs[ID_type].data, bufferType);		
			}
			else if (ID_type>=IDT_texture0 && ID_type<=IDT_texture7)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].id);
				glBufferDataARB(GL_ARRAY_BUFFER_ARB, VBOs[ID_type].size, VBOs[ID_type].data, bufferType);
			}
		}
	}

	GLvoid CVBO::draw()
	{
		VBOData *vColData = &VBOs[IDT_color];
		VBOData *vNormData = &VBOs[IDT_normal];
		VBOData *vVertData = &VBOs[IDT_vertex];

		bool reupload = (bufferType==BT_STREAM_DRAW || bufferType==BT_DYNAMIC_DRAW);

		bool col = vColData->id>0;

		if (col)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vColData->id);
			if (reupload && reUpload[IDT_color])
			{
				//glBufferDataARB(GL_ARRAY_BUFFER_ARB, vColData->size, vColData->data, bufferType);
				glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, vColData->size, vColData->data);
			}
			glColorPointer(vColData->elementLength, vColData->dataType, 0, 0);
			glEnableClientState(GL_COLOR_ARRAY);
		}	

		bool tex=false;
		for (GLint i=IDT_texture0; i<=IDT_texture7; i++)
		{
			VBOData *vTexData = &VBOs[i];
			bool tex_ = vTexData->id>0;

			if (tex_)
			{
				glActiveTextureARB(GL_TEXTURE0_ARB+(i-IDT_texture0));
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textures[(i-IDT_texture0)]);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glBindBufferARB(GL_ARRAY_BUFFER_ARB, vTexData->id);
				if (reupload && reUpload[i])
				{
					//glBufferDataARB(GL_ARRAY_BUFFER_ARB, vTexData->size, vTexData->data, bufferType);
					glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, vTexData->size, vTexData->data);
				}
				glTexCoordPointer(vTexData->elementLength, vTexData->dataType, 0, 0);

				tex=true;
			}
		}

		bool norm = vNormData->id>0;

		if (norm && reUpload[IDT_normal])
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vNormData->id);
			if (reupload)
			{
				//glBufferDataARB(GL_ARRAY_BUFFER_ARB, vNormData->size, vNormData->data, bufferType);		
				glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, vNormData->size, vNormData->data);
			}
			glNormalPointer(vNormData->dataType, 0, 0);
			glEnableClientState(GL_NORMAL_ARRAY);
		}

		bool vert = vVertData->id>0;

		if (vert)
		{
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, vVertData->id);
			if (reupload && reUpload[IDT_vertex])
			{
				//glBufferDataARB(GL_ARRAY_BUFFER_ARB, vVertData->size, vVertData->data, bufferType);
				glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, vVertData->size, vVertData->data);
			}
			glVertexPointer(vVertData->elementLength, vVertData->dataType, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glDrawArrays(enumMode, 0, vVertData->elementsCount);
		}

		if (norm)
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		if (tex)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}

		if (col)
		{
			glDisableClientState(GL_COLOR_ARRAY);
		}

		if (vert)
		{
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		for (GLint i=IDT_texture7; i>=IDT_texture0; i--)
		{
			if (VBOs[i].id>0)
			{
				glActiveTextureARB(GL_TEXTURE0_ARB+(i-IDT_texture0));
				glDisable(GL_TEXTURE_2D);
			}
		}

		string eErr=COGLUtils::errorToString(glGetError());
		glClientActiveTexture(GL_TEXTURE0);
	}

	GLvoid CVBO::setReUpload(CVBO::ID_TYPE id, bool val)
	{
		reUpload[id]=val;
	}

	GLvoid CVBO::setElementsCount(ID_TYPE id, GLint count)
	{
		/*if (VBOs[id].elementsCount==count)
		{
			return;
		}*/
		VBOs[id].elementsCount = count;
		VBOs[id].calcSize();
	}

	GLvoid CVBO::setEnumMode(ENUM_MODE enumMode)
	{
		this->enumMode=enumMode;
	}
};