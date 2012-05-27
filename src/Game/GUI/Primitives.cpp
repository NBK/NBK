#include "../system.h"
#include <GL/gl.h>

#include "Primitives.h"

CDKPrimitives::CDKPrimitives()
{
	r=g=b=alpha=1.0f;
	z=0.0f;
	blend=false;

	stx=0.0f;
	etx=1.0f;
	sty=0.0f;
	ety=1.0f;
}

GLvoid CDKPrimitives::set_screen_extent(GLint screen_width, GLint screen_height)
{
	this->screen_width=screen_width;
	this->screen_height=screen_height;
}

CDKPrimitives::~CDKPrimitives()
{
}

GLvoid CDKPrimitives::set_color(GLfloat r, GLfloat g, GLfloat b)
{
	this->r=r;
	this->g=g;
	this->b=b;
}

GLvoid CDKPrimitives::set_color(GLfloat *rgb)
{
	r=rgb[0];
	g=rgb[1];
	b=rgb[2];
}

GLvoid CDKPrimitives::set_alpha(GLfloat alpha)
{
	this->alpha=alpha;
}

GLfloat CDKPrimitives::get_alpha()
{
	return alpha;
}

GLvoid CDKPrimitives::set_z(GLfloat z)
{
	this->z=z;
}

GLfloat CDKPrimitives::get_z()
{
	return z;
}

GLvoid CDKPrimitives::set_blend(bool blend)
{
	this->blend=blend;
}

bool CDKPrimitives::get_blend()
{
	return blend;
}

GLvoid CDKPrimitives::setup_ortho_projection()
{
//	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,screen_width,0,screen_height,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

GLvoid CDKPrimitives::restore_perspective_projection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
//	glEnable(GL_DEPTH_TEST);
}

GLvoid CDKPrimitives::draw_quad(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
{
	glColor4f(r,g,b,alpha);

	if (blend)
	{
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND);
	}

	setup_ortho_projection();

	glTranslatef(x,screen_height-y,z);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(stx,ety);
		glVertex2f(0,0);
		glTexCoord2f(stx,sty);
		glVertex2f(0,-height);
		glTexCoord2f(etx,sty);
		glVertex2f(width,-height);
		glTexCoord2f(etx,ety);
		glVertex2f(width,0);
	}
	glEnd();

	restore_perspective_projection();

	if (blend)
	{
		glDisable(GL_BLEND);
	}
}

GLvoid CDKPrimitives::draw_textured_quad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	draw_quad(x,y,width,height);
	glDisable(GL_TEXTURE_2D);
}

GLvoid CDKPrimitives::set_texture_offset(GLfloat stx, GLfloat etx, GLfloat sty, GLfloat ety)
{
	this->stx=stx;
	this->etx=etx;
	this->sty=sty;
	this->ety=ety;
}