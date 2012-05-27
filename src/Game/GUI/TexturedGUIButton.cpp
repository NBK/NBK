#include "TexturedGUIButton.h"

namespace GUI
{
	CTexturedGUIButton::CTexturedGUIButton(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, char *caption, GLuint texture)
	: CBasicGUIButton(x_pos,y_pos,z_pos,width,height,caption)
	{
		this->texture=texture;
		set_color(1.0f,1.0f,1.0f);
		set_caption_color(1.0f,1.0f,1.0f);
	}

	CTexturedGUIButton::~CTexturedGUIButton()
	{
	}

	GLvoid CTexturedGUIButton::on_mouse_over()
	{
		CBasicGUIButton::on_mouse_over();
	}

	GLvoid CTexturedGUIButton::on_mouse_down()
	{
		CBasicGUIButton::on_mouse_down();
	}

	GLvoid CTexturedGUIButton::on_mouse_up()
	{
		CBasicGUIButton::on_mouse_up();
	}

	GLvoid CTexturedGUIButton::on_mouse_out()
	{
		CBasicGUIButton::on_mouse_out();
	}

	GLvoid CTexturedGUIButton::on_mouse_click()
	{
		CBasicGUIButton::on_mouse_click();
	}

	GLvoid CTexturedGUIButton::draw()
	{
		shape->draw_textured_quad(extent.pixel_pos_x,extent.pixel_pos_y,extent.pixel_width,extent.pixel_height,texture);
		display_caption();
	}

	GLvoid CTexturedGUIButton::set_texture(GLuint texture)
	{
		this->texture=texture;
	}
}