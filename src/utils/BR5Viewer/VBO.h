#ifndef VBO_H
#define VBO_H

#include "glew.h"
#include <windows.h>
#include <gl/gl.h>

#define MAX_ID	11

namespace rendering
{
	class CVBO
	{
	public:

		enum BUFFER_TYPE
		{
			BT_STREAM_DRAW	= GL_STREAM_DRAW_ARB, 
			BT_DYNAMIC_DRAW	= GL_DYNAMIC_DRAW_ARB,
			BT_STATIC_DRAW	= GL_STATIC_DRAW_ARB
		};

		enum DATA_TYPE
		{
			DT_FLOAT			= GL_FLOAT,
			DT_UNSIGNED_BYTE	= GL_UNSIGNED_BYTE
		};

		enum TEXTURE_UNIT
		{
			TU_UNIT0=0,
			TU_UNIT1,
			TU_UNIT2,
			TU_UNIT3,
			TU_UNIT4,
			TU_UNIT5,
			TU_UNIT6,
			TU_UNIT7
		};

		enum ID_TYPE
		{
			IDT_vertex=0, 
			IDT_color, 
			IDT_normal, 
			IDT_texture0,
			IDT_texture1,
			IDT_texture2,
			IDT_texture3,
			IDT_texture4,
			IDT_texture5,
			IDT_texture6,
			IDT_texture7
		};

		enum ENUM_MODE
		{
			EM_TRIANGLES	= GL_TRIANGLES,
			EM_QUADS		= GL_QUADS
		};

		CVBO(BUFFER_TYPE bufferType, bool clean=true);
		~CVBO();

		/*
			elementsCount - number of elements (eg.: number of points we are trying to upload )
			elementLength - how many items construct the element (eg.: triangles - 3, quads - 4, ...)
			elementSizeInBytes - just sizeof(element_data_type) (eg.: sizeof(GLfloat) )
		*/

		GLvoid setVertexData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType);
		GLvoid setNormalData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType);
		GLvoid setColorData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType);
		GLvoid setTextureData(TEXTURE_UNIT textureUnit, GLuint texture, GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType);
		GLvoid setTextureOnly(TEXTURE_UNIT textureUnit, GLuint texture);
		GLvoid draw();
		GLvoid setReUpload(ID_TYPE id, bool val);
		GLvoid setElementsCount(ID_TYPE id, GLint count);
		GLvoid setEnumMode(ENUM_MODE enumMode);

	private:
		bool reUpload[MAX_ID];

		struct VBOData
		{
			GLuint id;

			GLint	elementsCount, 
					elementLength, 
					elementSizeInBytes;

			DATA_TYPE dataType;

			GLvoid *data;

			GLsizei size;

			GLvoid calcSize()
			{
				size = elementsCount*elementLength*elementSizeInBytes;
			}
		};

		VBOData		VBOs[MAX_ID];
		GLuint		textures[MAX_ID];

		BUFFER_TYPE	bufferType;
		bool		clean;

		ENUM_MODE	enumMode;

		/***********************************************/

		GLvoid setData(GLint elementsCount, GLint elementLength, GLint elementSizeInBytes, GLvoid *data, DATA_TYPE dataType, ID_TYPE ID_type);

	};
};

#endif // VBO_H