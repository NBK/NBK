#ifndef DKPRIMITIVES_H
#define DKPRIMITIVES_H

class CDKPrimitives
{
public:
	CDKPrimitives();
	~CDKPrimitives();

	GLvoid set_color(GLfloat r, GLfloat g, GLfloat b);
	GLvoid set_color(GLfloat *rgb);

	GLvoid set_alpha(GLfloat alpha);
	GLfloat get_alpha();

	GLvoid set_z(GLfloat z);
	GLfloat get_z();

	GLvoid set_blend(bool blend);
	bool get_blend();

	GLvoid draw_quad(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	GLvoid draw_textured_quad(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLuint texture);

	GLvoid set_screen_extent(GLint screen_width, GLint screen_height);

	GLvoid set_texture_offset(GLfloat stx, GLfloat etx, GLfloat sty, GLfloat ety);

	GLvoid setup_ortho_projection();
	GLvoid restore_perspective_projection();

private:
	GLfloat r,g,b,alpha,z;
	bool blend;

	GLint	screen_width,
			screen_height;

	GLfloat stx, etx, sty, ety;
};

#endif // DKPRIMITIVES_H