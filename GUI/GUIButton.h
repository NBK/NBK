#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <stdarg.h>
#include <string.h>
#include "AbstractGUIItem.h"
#include "DKText.h"

namespace GUI
{
	class CBasicGUIButton: public CAbstractGUIItem
	{
	public:
		CBasicGUIButton(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, char *caption);
		virtual ~CBasicGUIButton();

		virtual GLvoid on_mouse_over();
		virtual GLvoid on_mouse_down();
		virtual GLvoid on_mouse_up();
		virtual GLvoid on_mouse_out();
		virtual GLvoid on_mouse_click();
		virtual GLvoid draw();
		virtual GLvoid init(GLint screen_width, GLint screen_height);

		GLvoid set_caption_color(GLfloat r, GLfloat g, GLfloat b);
		GLvoid set_caption_color(const GLfloat *rgb);
		GLvoid set_caption_size(GLint size);
		GLint get_caption_size();

		GLvoid set_caption(char *caption);
		GLvoid set_caption(const char *fmt, ...);
		char *get_caption();

	protected:
		char caption[128];
		CDKText *text;
		GLfloat text_x_pos,
				text_y_pos;

		GLvoid calculate_caption_pos();
		GLvoid display_caption();
	};
}

#endif // GUI_BUTTON_H