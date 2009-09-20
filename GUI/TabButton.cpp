#include "TabButton.h"

#include "PlayGUI.h"

namespace DK_GUI
{
	CTabButton::CTabButton()
	: CTexturedGUIButton(0.0f,0.0f,0.0f,0.0f,0.0f,"",0)
	{
	}

	CTabButton::~CTabButton()
	{
	}

	GLvoid CTabButton::on_mouse_down()
	{
		((CPlayGUI*)parent)->set_action(false,true,name[0],AEMG_TAB_CONTROL,(GLvoid*)this);
	}
}
