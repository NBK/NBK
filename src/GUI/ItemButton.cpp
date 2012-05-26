#include "ItemButton.h"

#include "PlayGUI.h"

namespace DK_GUI
{
	CItemButton::CItemButton(CDKTextureList *game_textures, GLint name)
	: CTexturedGUIButton(0.0f,0.0f,0.0f,0.0f,0.0f,"",game_textures->get_texture_by_name("BS_UNAVAILABLE"))
	{
		this->game_textures=game_textures;

		top_texture = 0;
		//fix_top_texture();

		set_name(name,0);

		first_down=false;

		/* because some button parts on the panel overdraw some other button parts we have to change this */
		mouse_detection_offset.set(0.00625f*2.0f,-0.003125f*2.0f,0.003125f*2.0f,-0.009375f*2.0f);

		button_state=BS_UNAVAILABLE;

		normal_texture = game_textures->get_texture_by_name("BS_UNAVAILABLE");
		mouse_over_texture= game_textures->get_texture_by_name("BS_AVAILABLE_OVER");
		mouse_down_texture	= game_textures->get_texture_by_name("BS_UNAVAILABLE");

		this->name[1]=AEMG_NONE;
		this->name[2]=AEMG_NONE;
	}

	CItemButton::~CItemButton()
	{
	}

	GLvoid CItemButton::set_top_texture(GLint top_texture)
	{
		this->top_texture=top_texture;
	}

	GLvoid CItemButton::init(GLint screen_width, GLint screen_height)
	{
		CAbstractGUIItem::init(screen_width,screen_height);
		CAbstractGUIItem::init(screen_width,screen_height,&top_texture_extent);
	}

	GLvoid CItemButton::on_mouse_over()
	{
		if (button_state>=BS_AVAILABLE_NOT_USED)
		{
			shape->set_z(1.0f);
			set_texture(mouse_over_texture);
		}
	}

	GLvoid CItemButton::on_mouse_out()
	{
		shape->set_z(0.0f);
		set_texture(normal_texture);

		if (first_down)
		{
			pop_state(&top_texture_extent);
			first_down=false;
		}
	}

	GLvoid CItemButton::on_not_mouse_over()
	{
		on_mouse_out();
	}

	GLvoid CItemButton::on_mouse_down()
	{
		set_texture(mouse_down_texture);		

		if (!first_down)
		{
			push_state(&top_texture_extent);			
			scale(0.9f,0.9f,&top_texture_extent);
			first_down=true;
		}
	}

	GLvoid CItemButton::on_mouse_up()
	{
		if (first_down)
		{
			pop_state(&top_texture_extent);
			first_down=false;
		}
	}

	GLvoid CItemButton::on_mouse_click()
	{
		if (mouse_button_pressed==MBP_LEFT)
		{
			((CPlayGUI*)parent)->set_action(true,true,name[0],name[1],(GLvoid*)this);
		}
		else if (mouse_button_pressed==MBP_RIGHT)
		{
			((CPlayGUI*)parent)->set_action(true,true,name[0],name[2],(GLvoid*)this);
		}
	}

	/* if GUI used forced alignment use this to fix position of the top texture */
	GLvoid CItemButton::fix_top_texture()
	{
		top_texture_extent.set(extent);
		/*top_texture_extent.x_pos+=extent.width/4.5f;
		top_texture_extent.y_pos+=extent.height/8.0f;
		top_texture_extent.width-=(extent.width/5.0f)*2.0f;
		top_texture_extent.height-=(extent.height/8.0f)*3.0f;*/
		CAbstractGUIItem::init(screen_width,screen_height,&top_texture_extent);
	}

	GLvoid CItemButton::draw()
	{
		CTexturedGUIButton::draw();

		if (button_state>BS_RESEARCH)
		{
			/* draw the second texture on-top of button texture */
			GLfloat pz=shape->get_z()/10.0f;
			shape->draw_textured_quad(top_texture_extent.pixel_pos_x,top_texture_extent.pixel_pos_y,top_texture_extent.pixel_width,top_texture_extent.pixel_height,top_texture);
		}
	}

	GLvoid CItemButton::set_button_state(BUTTON_STATE button_state)
	{
		this->button_state=button_state;

		if (button_state==BS_UNAVAILABLE || button_state==BS_AVAILABLE_NOT_USED)
		{
			set_mouse_check(false);
			normal_texture = game_textures->get_texture_by_name("BS_UNAVAILABLE");
		}
		else if (button_state==BS_RESEARCH)
		{
			set_mouse_check(false);
			normal_texture = game_textures->get_texture_by_name("BS_RESEARCH");
		}
		else if (button_state==BS_AVAILABLE_USED)
		{
			set_mouse_check(true);
			normal_texture = game_textures->get_texture_by_name("BS_AVAILABLE_USED");
		}

		set_texture(normal_texture);
	}

	CItemButton::BUTTON_STATE CItemButton::get_button_state()
	{
		return button_state;
	}

	GLvoid CItemButton::set_extent(EXTENT extent)
	{
		CAbstractGUIItem::set_extent(extent);
		//CAbstractGUIItem::init(&top_texture_extent);
		fix_top_texture();
	}
}