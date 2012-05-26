#include "AnimatedSpeedButton.h"

namespace DK_GUI
{
	CAnimatedSpeedButton::CAnimatedSpeedButton(CDKTextureList *game_textures, GLint name, GLint texture_count, const char **texture_names)
	: CSpeedButton(game_textures,name)
	{
		on=false;
		first=true;

		this->texture_count=texture_count;
		textures = new GLuint[texture_count];

		for (GLint i=0; i<texture_count; i++)
		{
			textures[i]=game_textures->get_texture_by_name(texture_names[i]);
		}
		tex_pos=0;
		tex_change=100.0f;
		tex_change_speed=5.0f;
	}

	CAnimatedSpeedButton::~CAnimatedSpeedButton()
	{
		delete [] textures;
	}	

	GLvoid CAnimatedSpeedButton::on_mouse_up()
	{
		if (first)
		{
			on=true;
			first=false;
		}
		else
		{
			first=true;
		}
		CSpeedButton::on_mouse_up();
	}

	GLvoid CAnimatedSpeedButton::on_mouse_down()
	{	
		if (!first)
		{
			on=false;			
		}
		CSpeedButton::on_mouse_down();
	}

	GLvoid CAnimatedSpeedButton::draw()
	{
		if (on)
		{
			set_texture(textures[tex_pos]);

			tex_change-=tex_change_speed;

			if (tex_change<0.0f)
			{				
				tex_change=100.0f;
				tex_pos++;
			}

			if (tex_pos>=texture_count)
			{
				tex_pos=0;
			}
		}
		CSpeedButton::draw();
	}
}