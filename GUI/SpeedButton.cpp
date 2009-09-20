#include "SpeedButton.h"
#include "PlayGUI.h"

namespace DK_GUI
{
	CSpeedButton::CSpeedButton(CDKTextureList *game_textures, GLint name)
	: CTexturedGUIButton(0.0f,0.0f,0.0f,0.0f,0.0f,"",0)
	{
		set_name(name,0);
		set_name(AEMG_NONE,1);
		if (name==SPEED_BUTTON_QUERY)
		{
			texture_up=game_textures->get_texture_by_name("QUERY_TAB_QUERY_UP");
			texture_down=game_textures->get_texture_by_name("QUERY_TAB_QUERY_DOWN");
		}
		else if (name==SPEED_BUTTON_CHICKEN)
		{
			texture_up=game_textures->get_texture_by_name("QUERY_TAB_CHICKEN_UP");
			texture_down=game_textures->get_texture_by_name("QUERY_TAB_CHICKEN_DOWN");
		}
		else if (name==SPEED_BUTTON_PRISON)
		{
			texture_up=game_textures->get_texture_by_name("QUERY_TAB_PRISON_UP");
			texture_down=game_textures->get_texture_by_name("QUERY_TAB_PRISON_DOWN");
		}
		else if (name==MINIMAP_BUTTON_ZOOM_IN)
		{
			texture_up=game_textures->get_texture_by_name(MINIMAP_ZOOM_IN_TEXTURE_NORMAL);
			texture_down=game_textures->get_texture_by_name(MINIMAP_ZOOM_IN_TEXTURE_PRESSED);
		}
		else if (name==MINIMAP_BUTTON_ZOOM_OUT)
		{
			texture_up=game_textures->get_texture_by_name(MINIMAP_ZOOM_OUT_TEXTURE_NORMAL);
			texture_down=game_textures->get_texture_by_name(MINIMAP_ZOOM_OUT_TEXTURE_PRESSED);
		}

		set_texture(texture_up);
	}

	CSpeedButton::~CSpeedButton()
	{
	}

	GLvoid CSpeedButton::on_mouse_down()
	{
		set_texture(texture_down);
	}

	GLvoid CSpeedButton::on_mouse_up()
	{
		set_texture(texture_up);
	}

	GLvoid CSpeedButton::on_mouse_click()
	{
		((CPlayGUI*)parent)->set_action(true,true,name[0],name[1],(GLvoid*)this);
	}

	GLvoid CSpeedButton::draw()
	{
		CTexturedGUIButton::draw();
	}
}