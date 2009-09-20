#include "commons.h"
#include "Primitives.h"
#include "OGLUtils.h"

using namespace utils;
using namespace game_utils;

namespace utils
{
	CPrimitives::CPrimitives()
	{
	}

	CPrimitives::~CPrimitives()
	{
	}

	GLvoid CPrimitives::drawQuad(CPrimitives::sQuadDescriptor &quadDescriptor)
	{
		// setup ortho
		COGLUtils::toGLOrtho(CV_SETTINGS_WINDOW_WIDTH,CV_SETTINGS_WINDOW_HEIGHT,true);

		if  (quadDescriptor.blend)
		{
			glEnable(GL_BLEND);
		}

		glColor4f(quadDescriptor.color[0],quadDescriptor.color[1],quadDescriptor.color[2],quadDescriptor.alpha);

		glPushMatrix();

		glTranslatef(quadDescriptor.position[0],quadDescriptor.position[1],0.0f);

		glBegin(GL_QUADS);
		{
			glVertex2f(0.0f,0.0f);
			glVertex2f(quadDescriptor.size[0],0.0f);
			glVertex2fv(&quadDescriptor.size[0]);
			glVertex2f(0.0f,quadDescriptor.size[1]);
		}
		glEnd();

		glPopMatrix();

		glColor3f(1.0f,1.0f,1.0f);

		if  (quadDescriptor.blend)
		{
			glDisable(GL_BLEND);

		}

		COGLUtils::restoreProjection();
	}
}