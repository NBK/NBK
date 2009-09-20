#include "GUIBackground.h"

namespace GUI
{
	CGUIBackground::CGUIBackground(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, GLuint texture)
	: CTexturedGUIButton(x_pos,y_pos,z_pos,width,height,"",texture)
	{
		check_mouse=false;
	}

	CGUIBackground::~CGUIBackground()
	{
	}
}