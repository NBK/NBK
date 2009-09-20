#ifndef TEXTURED_GUI_BUTTON
#define TEXTURED_GUI_BUTTON

#include "GUIButton.h"

namespace GUI
{
	class CTexturedGUIButton: public CBasicGUIButton
	{
	public:
		CTexturedGUIButton(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, char *caption, GLuint texture);
		virtual ~CTexturedGUIButton();

		virtual GLvoid on_mouse_over();
		virtual GLvoid on_mouse_down();
		virtual GLvoid on_mouse_up();
		virtual GLvoid on_mouse_out();
		virtual GLvoid on_mouse_click();
		virtual GLvoid draw();		

		GLvoid set_texture(GLuint texture);

	private:
		GLuint texture;
	};
}
#endif // TEXTURED_GUI_BUTTON