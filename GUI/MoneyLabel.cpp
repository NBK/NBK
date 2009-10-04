#include "DKcommons.h"
#include "MoneyLabel.h"

namespace DK_GUI
{
	CMoneyLabel::CMoneyLabel(): CAbstractGUIItem(0.0f,0.0f,0.0f,0.0f,0.0f)
	{
		set_class_name(MONEY_LABEL);

		char *NUMBER_TEXTURE = strdup(NUMBER_TEXTURE_0);

		/* get the number textures. BEWARE: map object must already exist */
		for (GLint i=0; i<10; i++)
		{
			NUMBER_TEXTURE[strlen(NUMBER_TEXTURE)-1]=(char)(i+(GLint)'0');
			number_texture[i]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE);
		}

		delete NUMBER_TEXTURE;

		value=12345;

		string_value=NULL;

		check_updated_value=true;
	}

	CMoneyLabel::~CMoneyLabel()
	{
		if (string_value)
		{
			delete []string_value;
		}
	}

	GLvoid CMoneyLabel::draw()
	{
		/* if money changed we have to update this */
		if (check_updated_value && value!=PLAYER0_MONEY)
		{
			set_value(PLAYER0_MONEY);
		}

		GLfloat tx=extent.pixel_pos_x + extent.pixel_width / 2.0f - (20.0f*(GLfloat)count)/2.0f;
		for (GLint i=0; i<count; i++)
		{
			glPushMatrix();
			shape->draw_textured_quad(tx,extent.pixel_pos_y,20.0f,extent.pixel_height,number_texture[(GLint)(string_value[i])-(GLint)'0']);
			glPopMatrix();

			tx+=20.0f;
		}
	}

	GLvoid CMoneyLabel::update(GLint amount)
	{
		value+=amount;

		if (string_value)
		{
			delete []string_value;
		}
		string_value = IntToStrDK(value);
		count = strlen(string_value);
	}

	GLvoid CMoneyLabel::set_value(GLint amount)
	{
		value=amount;
		update(0);
	}

	GLvoid CMoneyLabel::set_extent(EXTENT extent)
	{
		CAbstractGUIItem::set_extent(extent);
		update(0);
	}

	GLvoid CMoneyLabel::set_check_updated_value(bool check_updated_value)
	{
		this->check_updated_value=check_updated_value;
	}
}