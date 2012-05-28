#include "DKcommons.h"
#include "MoneyLabel.h"

namespace DK_GUI
{
	CMoneyLabel::CMoneyLabel(): CAbstractGUIItem(0.0f,0.0f,0.0f,0.0f,0.0f)
	{
		set_class_name(MONEY_LABEL);

		number_texture[0]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_0);
		number_texture[1]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_1);
		number_texture[2]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_2);
		number_texture[3]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_3);
		number_texture[4]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_4);
		number_texture[5]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_5);
		number_texture[6]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_6);
		number_texture[7]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_7);
		number_texture[8]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_8);
		number_texture[9]=GLOBAL_TEXTURE_LIST->get_texture_by_name(NUMBER_TEXTURE_9);

		value=12345;

		check_updated_value = true;
	}

	CMoneyLabel::~CMoneyLabel()
	{
		
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

		string_value = value;
		count = string_value.size();
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