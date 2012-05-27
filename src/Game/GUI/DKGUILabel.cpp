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
		// a integer is max 10 char's big but its okey for now...
		char tmp[20];

		GLint new_val = StrToIntDK(get_caption())+x;
		sprintf(tmp, "%d", new_val);

		set_caption(tmp);
	}

	GLvoid CDKGUILabel::int_set(GLint x)
	{
		set_caption("0");
		int_update(x);
	}
}