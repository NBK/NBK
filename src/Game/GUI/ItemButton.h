#ifndef ITEM_BUTTON_H
#define ITEM_BUTTON_H

#include "TexturedGUIButton.h"
using namespace GUI;

#include "DKTextureList.h"

namespace DK_GUI
{
	class CItemButton: public CTexturedGUIButton
	{
	public:
		CItemButton(CDKTextureList *game_textures, GLint name);
		virtual ~CItemButton();

		virtual GLvoid on_mouse_over();
		virtual GLvoid on_not_mouse_over();
		virtual GLvoid on_mouse_down();
		virtual GLvoid on_mouse_up();
		virtual GLvoid on_mouse_out();
		virtual GLvoid on_mouse_click();
		virtual GLvoid draw();	
		virtual GLvoid init(GLint screen_width, GLint screen_height);
		virtual GLvoid set_extent(EXTENT extent);

		GLvoid set_top_texture(GLint top_texture);

		GLvoid fix_top_texture();

		/* 
		this buttons can be in different states:
		 - unavailable: empty frame
		 - needs to be researched/discovered
		 - available but not used
		 - available and used
		*/
		enum BUTTON_STATE{BS_UNAVAILABLE=0, BS_RESEARCH, BS_AVAILABLE_NOT_USED, BS_AVAILABLE_USED };

		GLvoid set_button_state(BUTTON_STATE button_state);
		BUTTON_STATE get_button_state();

	private:
		GLuint	normal_texture,
				mouse_over_texture,
				mouse_down_texture,
				top_texture;

		EXTENT top_texture_extent;

		bool first_down;

		BUTTON_STATE button_state;

		CDKTextureList *game_textures;
	};
}

#endif // ITEM_BUTTON_H
