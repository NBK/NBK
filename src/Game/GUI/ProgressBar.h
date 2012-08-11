#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include "AbstractGUIItem.h"

namespace GUI
{
	class CProgressBar: public CAbstractGUIItem
	{
	public:
		CProgressBar(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height);
		virtual ~CProgressBar();

		virtual GLvoid update();
		virtual GLvoid set_extent(EXTENT extent);

		GLvoid set_min_value(GLfloat min_value);
		GLvoid set_max_value(GLfloat max_value);
		GLvoid set_increment(GLfloat increment);

		virtual GLvoid on_max_position();
		virtual GLvoid reset();		

		enum PROGRESS_BAR_STYLE{PBS_FROM_LEFT, PBS_FROM_RIGHT};

		GLvoid set_style(PROGRESS_BAR_STYLE style);

		GLvoid set_inverted(bool inverted);

	private:
		GLfloat color[3];
		GLfloat min_value,
				max_value,
				position,
				increment,
				temp_start_x;

		PROGRESS_BAR_STYLE style;

		GLvoid set_width_pos();

		/* if true, the bar starts at max and goes to minimum */
		bool inverted;
	};
}

#endif // PROGRESS_BAR_H