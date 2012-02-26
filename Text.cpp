#include "commons.h"
#include "system.h"
#include <GL/gl.h>
#include <stdio.h>
#include <stdio.h>
#include "Text.h"
#include "TextureLoader.h"
#include "OGLUtils.h"

using namespace loaders;
using namespace utils;
using namespace game_utils;

namespace utils
{
	CText::CText()
	{
		characterSize=20;
		characterSpacing=characterSize-2;
		r=g=b=1.0f;
		alpha=1.0f;
		blend=false;
		CTextureLoader::buildTexture((CV_RESOURCES_DIRECTORY+string("font/font.bmp")).c_str(),fontTexture,false,GL_LINEAR,false,0,0,0,true);
		base=0;
		buildNormalFont();	
	}

	CText::~CText()
	{
		killNormalFont();
	}

	GLvoid CText::setColor(GLfloat r, GLfloat g, GLfloat b)
	{
		this->r=r;
		this->g=g;
		this->b=b;
	}

	GLvoid CText::setAlpha(GLfloat alpha)
	{
		this->alpha=alpha;
	}

	GLfloat CText::getAlpha() 
	{
		return alpha;
	}

	GLvoid CText::print(GLfloat x, GLfloat y, const char *fmt, ...)
	{
		char string[256];
		va_list	ap;

		if (fmt == NULL)
		{
			return;
		}

		va_start(ap, fmt);
		vsprintf(string, fmt, ap);
		va_end(ap);

		if (blend)
		{
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glEnable(GL_BLEND);
		}

		glColor3f(r,g,b);
		glEnable(GL_TEXTURE_2D);	
		glBindTexture(GL_TEXTURE_2D, fontTexture);			

		utils::COGLUtils::toGLOrtho(CV_SETTINGS_WINDOW_WIDTH,CV_SETTINGS_WINDOW_HEIGHT);
		glTranslatef(x,CV_SETTINGS_WINDOW_HEIGHT-y,0);					
		glListBase(base-32+(128));					
		glCallLists((GLsizei)strlen(string),GL_UNSIGNED_BYTE,string);

		utils::COGLUtils::restoreProjection();
		glDisable(GL_TEXTURE_2D);	
		glColor3f(1.0f,1.0f,1.0f);
	
		if (blend)
		{
			glDisable(GL_BLEND);
		}
	}

	GLvoid CText::buildNormalFont()
	{
		float	cx;	
		float	cy;

		if (base!=0)
		{
			glDeleteLists(base,256);
			base=0;
		}

		base=glGenLists(256);
		glBindTexture(GL_TEXTURE_2D, fontTexture);
		for (int loop=0; loop<256; loop++)
		{
			cx=float(loop%16)/16.0f;
			cy=float(loop/16)/16.0f;

			glNewList(base+loop,GL_COMPILE);	
				glBegin(GL_QUADS);					
					glTexCoord2f(cx,1-cy-0.0625f);	
					glVertex2i(0,0);	
					glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	
					glVertex2i(characterSize,0);
					glTexCoord2f(cx+0.0625f,1-cy);
					glVertex2i(characterSize,characterSize);
					glTexCoord2f(cx,1-cy);		
					glVertex2i(0,characterSize);
				glEnd();							
				glTranslated(characterSpacing,0,0);
			glEndList();				
		}
	}

	GLvoid CText::killNormalFont()
	{
		glDeleteLists(base,256);
	}

	GLvoid CText::setBlend(bool blend)
	{
		this->blend=blend;
	}

	bool CText::doBlend()
	{
		return blend;
	}

	GLint CText::getCharacterSize()
	{
		return characterSize;
	}

	GLint CText::getCharacterSpacing()
	{
		return characterSpacing;
	}

	GLint CText::getTextSize(const char *string)
	{
		return (GLint)strlen(string)*characterSpacing;
	}

	GLint CText::setCharacterSize(GLint characterSize)
	{
		GLint oldSize = this->characterSize;

		this->characterSize=characterSize;
		characterSpacing=characterSize-2;
		buildNormalFont();

		return oldSize;
	}
};