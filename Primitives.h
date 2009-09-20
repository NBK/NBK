#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <cml/cml.h>
#include <windows.h>
#include <gl/gl.h>

namespace utils
{
	class CPrimitives
	{
	public:
		CPrimitives();
		~CPrimitives();

		struct sQuadDescriptor
		{
			cml::vector3f	color;

			cml::vector2f	position,
							size;

			bool			blend;

			GLfloat			alpha;

			sQuadDescriptor()
			{
				color =		cml::vector3f(1.0f,1.0f,1.0f);
				position =	cml::vector2f(0.0f,0.0f);
				size =		cml::vector2f(1.0f,1.0f);
				blend =		false;
				alpha =		1.0f;
			}
		};

		static GLvoid drawQuad(sQuadDescriptor &quadDescriptor);
	};
};

#endif /// PRIMITIVES_H