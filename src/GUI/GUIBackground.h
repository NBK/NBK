#ifndef GUI_BACKGROUND
#define GUI_BACKGROUND

#include "TexturedGUIButton.h"

namespace GUI
{
	class CGUIBackground: public CTexturedGUIButton
	{
	public:
		CGUIBackground(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, GLuint texture);
		~CGUIBackground();
	};
}

#endif // GUI_BACKGROUND