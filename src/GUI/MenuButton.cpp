#include "MenuButton.h"

namespace DK_GUI
{
	CMenuButton::CMenuButton(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, char *caption, GLuint normal_texture, GLuint effect_texture)
	: CTexturedGUIButton(x_pos,y_pos,z_pos,width,height,caption,normal_texture)
	{
		set_caption_color(0.9f,0.4f,0.1f);
		set_caption_size(25);
		red=0.0f;
		red_speed=0.02f;
		over=false;

		this->normal_texture=normal_texture;
		this->effect_texture=effect_texture;
	}

	GLvoid CMenuButton::on_mouse_over()
	{
		set_caption_color(1.0f,1.0f,0.0f);
		over=true;
	}

	GLvoid CMenuButton::on_mouse_down()
	{		
	}

	CMenuButton::~CMenuButton()
	{
	}

	GLvoid CMenuButton::draw()
	{	
		if (over)
		{
			push_state();
			GLfloat old_w = extent.width;
			GLfloat old_h = extent.height;
			scale(1.012f,1.12f);
			translate(-(extent.width-old_w)/2.0f,-(extent.height-old_h)/2.0f,0.0f);

			glPushMatrix();	
			set_color(red,0.0f,0.0f);
			red+=red_speed;
			if (red>1.0f || red<0.0f)
			{
				red_speed=-red_speed;
			}
			over=false;

			set_texture(effect_texture);
			CTexturedGUIButton::draw();
			glPopMatrix();

			pop_state();	
		}
		else
		{
			red=0.0f;
		}		

		set_color(1.0f,1.0f,1.0f);
		set_texture(normal_texture);
		CTexturedGUIButton::draw();
		set_caption_color(1.0f,0.0f,0.0f);
	}
}