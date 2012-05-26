#include <stdio.h>
#include "GUIButton.h"

namespace GUI
{
	CBasicGUIButton::CBasicGUIButton(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height, char *caption)
	: CAbstractGUIItem(x_pos,y_pos,z_pos,width,height)
	{
		set_color(0.4f,0.4f,0.4f);
		text = new CDKText();
		set_caption(caption);
		set_caption_color(0.0f,0.0f,0.0f);
	}

	CBasicGUIButton::~CBasicGUIButton()
	{
		delete text;
	}

	GLvoid CBasicGUIButton::on_mouse_over()
	{
		CAbstractGUIItem::on_mouse_over();
	}

	GLvoid CBasicGUIButton::on_mouse_down()
	{
		CAbstractGUIItem::on_mouse_down();
	}

	GLvoid CBasicGUIButton::on_mouse_up()
	{
		CAbstractGUIItem::on_mouse_up();
	}

	GLvoid CBasicGUIButton::on_mouse_out()
	{
		CAbstractGUIItem::on_mouse_out();
	}

	GLvoid CBasicGUIButton::on_mouse_click()
	{
		CAbstractGUIItem::on_mouse_click();
	}

	GLvoid CBasicGUIButton::calculate_caption_pos()
	{
		text_x_pos = extent.pixel_width/2 - text->get_text_size(caption)/2;
		text_y_pos = extent.pixel_height/2 + text->get_character_size()/2.0f;
	}

	GLvoid CBasicGUIButton::display_caption()
	{
		calculate_caption_pos();
		text->print((GLint)(extent.pixel_pos_x+text_x_pos),(GLint)(extent.pixel_pos_y+text_y_pos),caption);
	}

	GLvoid CBasicGUIButton::draw()
	{
		CAbstractGUIItem::draw();
		display_caption();
	}

	GLvoid CBasicGUIButton::set_caption_color(GLfloat r, GLfloat g, GLfloat b)
	{
		text->set_color(r,g,b);
	}

	GLvoid CBasicGUIButton::set_caption_color(const GLfloat *rgb)
	{
		text->set_color(rgb[0],rgb[1],rgb[2]);
	}

	GLvoid CBasicGUIButton::set_caption_size(GLint size)
	{
		text->set_character_size(size);
	}

	GLint CBasicGUIButton::get_caption_size()
	{
		return text->get_character_size();
	}

	GLvoid CBasicGUIButton::init(GLint screen_width, GLint screen_height)
	{
		CAbstractGUIItem::init(screen_width,screen_height);
		text->set_screen_extent(screen_width,screen_height);
	}

	GLvoid CBasicGUIButton::set_caption(char *caption)
	{
		strcpy(this->caption,caption);
	}

	GLvoid CBasicGUIButton::set_caption(const char *fmt, ...)
	{
		char string[128];
		va_list	ap;

		if (fmt == NULL)
		{
			return;
		}

		va_start(ap, fmt);
		vsprintf(string, fmt, ap);
		va_end(ap);

		strcpy(caption,string);
	}

	char *CBasicGUIButton::get_caption()
	{
		return caption;
	}
}