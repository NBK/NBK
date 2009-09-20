#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "GUI.h"
#include "GUIBackground.h"
#include "MenuButton.h"
#include "DKTextureList.h"

namespace DK_GUI
{
	class CMainMenu
	{
	public:
		CMainMenu(GLint screen_width, GLint screen_height, CDKTextureList *game_textures);
		~CMainMenu();

		GLvoid draw_and_do_actions();

		CGUI *get_active_gui();

		enum SELECTED_MENU{SM_MAIN=0,SM_NEW,SM_LOAD,SM_MULTI,SM_OPTIONS,SM_SCORE};

	private:
		CGUI	*main_menu,
				*main_menu_new_game_continue,
				*main_menu_load_game,
				*main_menu_multiplayer,
				*main_menu_options,
				*main_menu_high_score_table;

		CGUI *menus[6];

		SELECTED_MENU selected_menu;
	};
}

#endif // MAIN_MENU_H