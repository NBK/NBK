#include "ProgressBar.h"

namespace GUI
{
	CProgressBar::CProgressBar(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height)
	: CAbstractGUIItem(x_pos,y_pos,z_pos,0.0f,height)
	{
		min_value=position=0.0f;
		increment=1.0f;
		set_width(position);
		style=PBS_FROM_LEFT;
		max_value=1.0f;
		temp_start_x=0.0f;
		inverted=false;
	}

	CProgressBar::~CProgressBar()
	{
	}

	GLvoid CProgressBar::set_min_value(GLfloat min_value)
	{
		this->min_value=min_value;
	}

	GLvoid CProgressBar::set_max_value(GLfloat max_value)
	{
		this->max_value=max_value;
	}

	GLvoid CProgressBar::set_increment(GLfloat increment)
	{
		this->increment=increment;
	}

	GLvoid CProgressBar::on_max_position()
	{
		reset();
	}

	GLvoid CProgressBar::update()
	{
		position+=increment;
		if (style==PBS_FROM_LEFT)
		{			
			set_width(position);
		}
		else if (style==PBS_FROM_RIGHT)
		{
			set_width_pos();
		}

		if (!inverted && position>=max_value)
		{
			on_max_position();
		}
		else if (inverted && position<=min_value)
		{
			on_max_position();
		}
	}

	GLvoid CProgressBar::reset()
	{
		if (!inverted)
		{
			position=0.0f;
		}
		else
		{
			position=max_value;
		}

		if (style==PBS_FROM_LEFT)
		{			
			set_width(position);
		}
		else if (style==PBS_FROM_RIGHT)
		{
			set_width_pos();
		}
	}

	GLvoid CProgressBar::set_style(PROGRESS_BAR_STYLE style)
	{
		this->style=style;
	}

	GLvoid CProgressBar::set_width_pos()
	{
		set_width(position);
		set_x_pos(temp_start_x-position);
	}

	GLvoid CProgressBar::set_inverted(bool inverted)
	{
		this->inverted=inverted;
		position=max_value;
		increment=-increment;
	}

	GLvoid CProgressBar::set_extent(EXTENT extent)
	{
		if (style==PBS_FROM_RIGHT)
		{
			extent.x_pos+=extent.width;			
			max_value=extent.width;
			position=max_value;
			extent.width=0.0f;
			temp_start_x=extent.x_pos;
		}
		CAbstractGUIItem::set_extent(extent);
	}
}