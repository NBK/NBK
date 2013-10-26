#include "../system.h"
#include <GL/gl.h>
#include <stdio.h>
#include "DKText.h"
#include "DKTextureLoader.h"

CDKText::CDKText()
{
	character_size=20;
	character_spacing=character_size-2;
	r=g=b=1.0f;
	alpha=1.0f;
	blend=false;
	CDKTextureLoader tl;
	std::string font = "data/resources/font/font.bmp";
	tl.build_texture(font, font_texture,true);
	base=0;
	build_normal_font();
}

GLvoid CDKText::set_screen_extent(GLint screen_width, GLint screen_height)
{
	this->screen_width=screen_width;
	this->screen_height=screen_height;
}

CDKText::~CDKText()
{
	kill_normal_font();
}

GLvoid CDKText::set_color(GLfloat r, GLfloat g, GLfloat b)
{
	this->r=r;
	this->g=g;
	this->b=b;
}

GLvoid CDKText::set_alpha(GLfloat alpha)
{
	this->alpha=alpha;
}

GLfloat CDKText::get_alpha()
{
	return alpha;
}

GLvoid CDKText::print(GLint x, GLint y, const char *fmt, ...)
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

	glColor3f(r,g,b);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	//glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,screen_width,0,screen_height,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,screen_height-y,0);
	glListBase(base-32+(128));
	glCallLists((GLsizei)strlen(string),GL_UNSIGNED_BYTE,string);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}

GLvoid CDKText::build_normal_font()
{
	float	cx;
	float	cy;

	if (base!=0)
	{
		glDeleteLists(base,256);
		base=0;
	}

	base=glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	for (int loop=0; loop<256; loop++)
	{
		cx=float(loop%16)/16.0f;
		cy=float(loop/16)/16.0f;

		glNewList(base+loop,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx,1-cy-0.0625f);
				glVertex2i(0,0);
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);
				glVertex2i(character_size,0);
				glTexCoord2f(cx+0.0625f,1-cy);
				glVertex2i(character_size,character_size);
				glTexCoord2f(cx,1-cy);
				glVertex2i(0,character_size);
			glEnd();
			glTranslated(character_spacing,0,0);
		glEndList();
	}
}

GLvoid CDKText::kill_normal_font()
{
	glDeleteLists(base,256);
}

GLvoid CDKText::set_blend(bool blend)
{
	this->blend=blend;
}

bool CDKText::get_blend()
{
	return blend;
}

GLint CDKText::get_character_size()
{
	return character_size;
}

GLint CDKText::get_character_spacing()
{
	return character_spacing;
}

GLint CDKText::get_text_size(const char *string)
{
	return (GLint)strlen(string)*character_spacing;
}

GLint CDKText::set_character_size(GLint character_size)
{
	GLint old_size = this->character_size;

	this->character_size=character_size;
	character_spacing=character_size-2;
	build_normal_font();

	return old_size;
}
