#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "GUIButton.h"

namespace GUI
{
	class CGUILabel: public CBasicGUIButton
	{
	public:
		CGUILabel(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, char *caption);
		~CGUILabel();

		virtual GLvoid on_mouse_over();
		virtual GLvoid on_mouse_down();
		virtual GLvoid on_mouse_up();
		virtual GLvoid on_mouse_out();
		virtual GLvoid on_mouse_click();
		virtual GLvoid draw();
	};
}

#endif // GUI_LABEL_H