#ifndef DKTEXT_H
#define DKTEXT_H

class CDKText  
{
public:
	CDKText();
	~CDKText();

	GLvoid set_color(GLfloat r, GLfloat g, GLfloat b);

	GLvoid set_alpha(GLfloat alpha);
	GLfloat get_alpha();

	GLvoid print(GLint x, GLint y, const char *fmt, ...);

	GLvoid set_blend(bool blend);
	bool get_blend();

	GLint set_character_size(GLint character_size);

	GLint get_character_size();
	GLint get_character_spacing();

	GLint get_text_size(const char *string);

	GLvoid set_screen_extent(GLint screen_width, GLint screen_height);

private:
	GLvoid build_normal_font();
	GLvoid kill_normal_font();

	GLuint font_texture,base;
	GLfloat r,g,b,alpha;
	GLint character_size, character_spacing;

	bool blend;

	GLint	screen_width, 
			screen_height;
};

#endif // DKTEXT_H
