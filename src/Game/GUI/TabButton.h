#ifndef TAB_BUTTON_H
#define TAB_BUTTON_H

#include "TexturedGUIButton.h"

using namespace GUI;

namespace DK_GUI
{
	class CTabButton: public CTexturedGUIButton
	{
	public:
		CTabButton();
		~CTabButton();

		virtual GLvoid on_mouse_down();
	};
}

#endif // TAB_BUTTON_H
