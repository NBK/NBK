#include "MainMenu.h"

namespace DK_GUI
{
	CMainMenu::CMainMenu(GLint screen_width, GLint screen_height, CDKTextureList *game_textures)
	{
		selected_menu=SM_MAIN;

		GLuint menu_button = game_textures->get_texture_by_name("MENU_BUTTON");
		GLuint menu_button_effect = game_textures->get_texture_by_name("MENU_BUTTON_EFFECT");
		GLuint main_menu_background = game_textures->get_texture_by_name("MAIN_MENU_BACKGROUND");

		GLfloat button_width = 0.58f;
		GLfloat button_height = 0.078f;

		main_menu = new CGUI(screen_width,screen_height);
		main_menu->add_item(new CGUIBackground(0.0f,0.0f,0.0f,1.0f,1.0f,main_menu_background));
		main_menu->add_item(new CMenuButton(0.0f,0.072f,0.0f,button_width,button_height,"Main menu",menu_button,menu_button_effect));
		main_menu->add_item(new CMenuButton(0.0f,0.224f,0.0f,button_width,button_height,"Start New Game",menu_button,menu_button_effect));
		main_menu->add_item(new CMenuButton(0.0f,0.329f,0.0f,button_width,button_height,"Continue Game",menu_button,menu_button_effect));
		main_menu->add_item(new CMenuButton(0.0f,0.435f,0.0f,button_width,button_height,"Load Game",menu_button,menu_button_effect));
		main_menu->add_item(new CMenuButton(0.0f,0.536f,0.0f,button_width,button_height,"Multiplayer",menu_button,menu_button_effect));
		main_menu->add_item(new CMenuButton(0.0f,0.645f,0.0f,button_width,button_height,"Options",menu_button,menu_button_effect));
		main_menu->add_item(new CMenuButton(0.0f,0.746f,0.0f,button_width,button_height,"High Score Table",menu_button,menu_button_effect));
		main_menu->add_item(new CMenuButton(0.0f,0.848f,0.0f,button_width,button_height,"Quit",menu_button,menu_button_effect));
		main_menu->init();
		main_menu->force_vertical_alignment();

		menus[0]=main_menu;
		menus[1]=main_menu_new_game_continue;
		menus[2]=main_menu_load_game;
		menus[3]=main_menu_multiplayer;
		menus[4]=main_menu_options;
		menus[5]=main_menu_high_score_table;
	}

	CMainMenu::~CMainMenu()
	{
		delete main_menu;
	}

	GLvoid CMainMenu::draw_and_do_actions()
	{
		menus[selected_menu]->draw();
		menus[selected_menu]->do_actions();
	}

	CGUI *CMainMenu::get_active_gui()
	{
		return menus[selected_menu];
	}
}