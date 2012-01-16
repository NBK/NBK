#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include "TexturedGUIButton.h"

using namespace GUI;

namespace DK_GUI
{
	class CMenuButton: public CTexturedGUIButton
	{
	public:
		CMenuButton(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, const char *caption, GLuint normal_texture, GLuint effect_texture);
		~CMenuButton();

		virtual GLvoid on_mouse_over();
		virtual GLvoid on_mouse_down();
		virtual GLvoid draw();			

	protected:
		GLfloat red;
		GLfloat red_speed;
		bool over;
		GLuint	normal_texture,
				effect_texture;
	};
}

#endif // MENU_BUTTON_H