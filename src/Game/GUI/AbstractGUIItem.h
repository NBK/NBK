#ifndef ABSTRACT_GUI_ITEM
#define ABSTRACT_GUI_ITEM

#include "../system.h"
#include <GL/gl.h>

#include "CPrimitives.h"

#define MAX_NAMES	5

namespace GUI
{
	class CAbstractGUIItem
	{
	public:
		CAbstractGUIItem(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height);
		virtual ~CAbstractGUIItem();

		struct EXTENT
		{
			GLfloat	x_pos,
					y_pos,
					z_pos,
					width,
					height,
					pixel_pos_x,
					pixel_pos_y,
					pixel_width,
					pixel_height;

			EXTENT()
			{
				x_pos=y_pos=z_pos=width=height=0.0f;
				pixel_pos_x=pixel_pos_y=0;
			}

			EXTENT(GLfloat	x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat	height)
			{
				this->x_pos=x_pos;
				this->y_pos=y_pos;
				this->z_pos=z_pos;
				this->width=width;
				this->height=height;		
			}

			GLvoid set(EXTENT &ex)
			{
				x_pos=ex.x_pos;
				y_pos=ex.y_pos;
				width=ex.width;
				height=ex.height;
				pixel_pos_x=ex.pixel_pos_x;
				pixel_pos_y=ex.pixel_pos_y;
				pixel_width=ex.pixel_width;
				pixel_height=ex.pixel_height;
			}
		};

		/* if we want to change the scope of mouse detection we use this */
		struct MOUSE_DETECTION_EXTENT_OFFSET
		{
			GLfloat	correction_from_left,
					correction_from_rigth,
					correction_from_top,
					correction_from_bottom,
					pixel_correction_from_left,
					pixel_correction_from_rigth,
					pixel_correction_from_top,
					pixel_correction_from_bottom;

			MOUSE_DETECTION_EXTENT_OFFSET()
			{
				correction_from_left=correction_from_rigth=0;
				correction_from_top=correction_from_bottom=0;

				pixel_correction_from_left=pixel_correction_from_rigth=0;
				pixel_correction_from_top=pixel_correction_from_bottom=0;
			}

			GLvoid set(GLfloat pcfl, GLfloat pcfr, GLfloat pcft, GLfloat pcfb)
			{
				correction_from_left=pcfl;
				correction_from_rigth=pcfr;
				correction_from_top=pcft;
				correction_from_bottom=pcfb;
			}
		};

		virtual GLvoid on_mouse_over();
		virtual GLvoid on_not_mouse_over();
		virtual GLvoid on_mouse_out();
		virtual GLvoid on_mouse_down();
		virtual GLvoid on_mouse_up();
		virtual GLvoid on_mouse_click();
		virtual GLvoid draw();
		virtual GLvoid update();
		virtual GLvoid init(GLint screen_width, GLint screen_height);
		virtual GLvoid init(GLint screen_width, GLint screen_height, EXTENT *extent);
		virtual GLvoid init();
		virtual GLvoid init(EXTENT *extent);
		virtual GLvoid set_extent(EXTENT extent);

		EXTENT *get_extent();
		MOUSE_DETECTION_EXTENT_OFFSET *get_mouse_detection_extent();

		GLvoid set_color(GLfloat r, GLfloat g, GLfloat b);	
		GLvoid scale(GLfloat scale_x, GLfloat scale_y);
		GLvoid scale(GLfloat scale_x, GLfloat scale_y, EXTENT *extent);
		GLvoid translate(GLfloat x, GLfloat y, GLfloat z);
		GLvoid translate(GLfloat x, GLfloat y, GLfloat z, EXTENT *extent);
		GLvoid push_state();
		GLvoid push_state(EXTENT *extent);
		GLvoid pop_state();
		GLvoid pop_state(EXTENT *extent);

		GLvoid set_mouse_check(bool check_mouse);
		bool do_check_mouse();

		GLuint get_index_number();

		GLvoid set_name(GLint name, GLint name_index);
		GLint get_name(GLint name_index);

		// CGUI is the parent 
		GLvoid *get_parent();
		GLvoid set_parent(GLvoid *parent);

		GLvoid set_class_name(const char *class_name);
		char *get_class_name();

		GLvoid set_visible(bool visible);
		bool is_visible();

		GLvoid set_ready(bool ready);
		bool is_ready();

		GLvoid set_width(GLfloat width);
		GLvoid set_height(GLfloat height);

		GLvoid set_x_pos(GLfloat x_pos);
		GLvoid set_y_pos(GLfloat y_pos);		

		enum MOUSE_BUTTON_PRESSED{MBP_LEFT=0, MBP_RIGHT, MBP_NONE};	

		GLvoid set_mouse_button(MOUSE_BUTTON_PRESSED mouse_button_pressed);
		MOUSE_BUTTON_PRESSED get_mouse_button();

	protected:	
		EXTENT extent;
		EXTENT tmp_extent;
		CDKPrimitives *shape;
		GLint	screen_width, 
				screen_height,
				name[MAX_NAMES];

		bool	check_mouse,
				visible,
				ready,
				lbm;

		static GLuint index_number_total;
		GLuint index_number;

		GLvoid *parent;

		char *class_name;

		MOUSE_DETECTION_EXTENT_OFFSET mouse_detection_offset;

		MOUSE_BUTTON_PRESSED mouse_button_pressed;
	};
}

#endif // ABSTRACT_GUI_ITEM