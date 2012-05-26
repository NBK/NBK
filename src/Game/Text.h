#ifndef TEXT_H
#define TEXT_H

namespace utils
{
	class CText  
	{
	public:
		CText();
		~CText();

		GLvoid	setColor(GLfloat r, GLfloat g, GLfloat b);
		GLvoid	setAlpha(GLfloat alpha);
		GLvoid	setBlend(bool blend);
		GLint	setCharacterSize(GLint characterSize);

		GLfloat getAlpha();
		bool	doBlend();
		GLint	getCharacterSize();
		GLint	getCharacterSpacing();
		GLint	getTextSize(const char *string);

		GLvoid print(GLfloat x, GLfloat y, const char *fmt, ...);		

	private:

		GLvoid	buildNormalFont();
		GLvoid	killNormalFont();

		GLuint	fontTexture,
				base;

		GLfloat r,g,b,
				alpha;

		GLint	characterSize, 
				characterSpacing;

		bool blend;
	};
};

#endif // TEXT_H
