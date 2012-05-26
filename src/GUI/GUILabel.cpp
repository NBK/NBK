#include "GUILabel.h"

namespace GUI
{
	CGUILabel::CGUILabel(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, char *caption)
	: CBasicGUIButton(x_pos,y_pos,z_pos,width,height,caption)
	{
	}

	CGUILabel::~CGUILabel()
	{
	}

	GLvoid CGUILabel::on_mouse_over()
	{
	}

	GLvoid CGUILabel::on_mouse_down()
	{
	}

	GLvoid CGUILabel::on_mouse_up()
	{
	}

	GLvoid CGUILabel::on_mouse_out()
	{
	}

	GLvoid CGUILabel::on_mouse_click()
	{
	}

	GLvoid CGUILabel::draw()
	{
		display_caption();
	}
}