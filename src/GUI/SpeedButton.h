#ifndef SPEED_BUTTON_H
#define SPEED_BUTTON_H

#include "TexturedGUIButton.h"
using namespace GUI;

#include "DKTextureList.h"

namespace DK_GUI
{
	class CSpeedButton: public CTexturedGUIButton
	{
	public:
		CSpeedButton(CDKTextureList *game_textures, GLint name);
		~CSpeedButton();

		virtual GLvoid on_mouse_up();
		virtual GLvoid on_mouse_down();
		virtual GLvoid on_mouse_click();
		virtual GLvoid draw();	

	private:
		GLuint	texture_up,
				texture_down;
	};
}

#endif // SPEED_BUTTON_H