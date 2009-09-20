#include "DKcommons.h"
#include "DKGUILabel.h"


namespace DK_GUI
{
	CDKGUILabel::CDKGUILabel(char *caption)
	: CGUILabel(0.0f,0.0f,0.0f,0.0f,0.0f,caption)
	{
	}

	CDKGUILabel::~CDKGUILabel()
	{
	}

	GLvoid CDKGUILabel::update()
	{
	}

	GLvoid CDKGUILabel::int_update(GLint x)
	{
		GLint new_val = StrToIntDK(get_caption())+x;
		char *tmp = IntToStrDK(new_val);
		set_caption(tmp);
		delete [] tmp;
	}

	GLvoid CDKGUILabel::int_set(GLint x)
	{
		set_caption("0");
		int_update(x);
	}
}