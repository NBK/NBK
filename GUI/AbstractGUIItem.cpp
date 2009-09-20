#include "AbstractGUIItem.h"

namespace GUI
{

	GLuint CAbstractGUIItem::index_number_total=0;

	CAbstractGUIItem::CAbstractGUIItem(GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, GLfloat width, GLfloat height)
	{
		extent.x_pos=x_pos;
		extent.y_pos=y_pos;
		extent.z_pos=z_pos;
		extent.width=width;
		extent.height=height;
		shape = new CDKPrimitives();
		check_mouse=true;
		index_number=index_number_total++;

		for (GLint i=0; i<MAX_NAMES; i++)
		{
			name[i]=-1;
		}
		class_name=NULL;
		visible=true;
		ready=true;

		mouse_button_pressed=MBP_NONE;
	}

	CAbstractGUIItem::~CAbstractGUIItem()
	{
		delete shape;
		if (class_name)
		{
			delete [] class_name;
		}
	}

	GLvoid CAbstractGUIItem::draw()
	{
		shape->draw_quad(extent.pixel_pos_x,extent.pixel_pos_y,extent.pixel_width,extent.pixel_height);
	}

	GLvoid CAbstractGUIItem::on_mouse_over()
	{
	}
	
	GLvoid CAbstractGUIItem::on_not_mouse_over()
	{
	}

	GLvoid CAbstractGUIItem::on_mouse_down()
	{
	}

	GLvoid CAbstractGUIItem::on_mouse_up()
	{
	}

	GLvoid CAbstractGUIItem::on_mouse_out()
	{
	}

	GLvoid CAbstractGUIItem::on_mouse_click()
	{
	}

	CAbstractGUIItem::EXTENT *CAbstractGUIItem::get_extent()
	{
		return &extent;
	}

	CAbstractGUIItem::MOUSE_DETECTION_EXTENT_OFFSET *CAbstractGUIItem::get_mouse_detection_extent()
	{
		return &mouse_detection_offset;
	}

	/* converts values from logical to pixel */
	GLvoid CAbstractGUIItem::init(GLint screen_width, GLint screen_height)
	{
		this->screen_width=screen_width;
		this->screen_height=screen_height;
		init();
	}

	GLvoid CAbstractGUIItem::init()
	{
		extent.pixel_pos_x=extent.x_pos*(GLfloat)screen_width;
		extent.pixel_pos_y=extent.y_pos*(GLfloat)screen_height;
		extent.pixel_width=extent.width*(GLfloat)screen_width;
		extent.pixel_height=extent.height*(GLfloat)screen_height;
		
		mouse_detection_offset.pixel_correction_from_left=mouse_detection_offset.correction_from_left*(GLfloat)screen_width;
		mouse_detection_offset.pixel_correction_from_rigth=mouse_detection_offset.correction_from_rigth*(GLfloat)screen_width;
		mouse_detection_offset.pixel_correction_from_top=mouse_detection_offset.correction_from_top*(GLfloat)screen_height;
		mouse_detection_offset.pixel_correction_from_bottom=mouse_detection_offset.correction_from_bottom*(GLfloat)screen_height;

		shape->set_screen_extent(screen_width,screen_height);
	}

	GLvoid CAbstractGUIItem::init(GLint screen_width, GLint screen_height, EXTENT *extent)
	{
		this->screen_width=screen_width;
		this->screen_height=screen_height;
		init(extent);		
	}

	GLvoid CAbstractGUIItem::init(EXTENT *extent)
	{
		extent->pixel_pos_x=extent->x_pos*(GLfloat)screen_width;
		extent->pixel_pos_y=extent->y_pos*(GLfloat)screen_height;
		extent->pixel_width=extent->width*(GLfloat)screen_width;
		extent->pixel_height=extent->height*(GLfloat)screen_height;

		shape->set_screen_extent(screen_width,screen_height);
	}

	GLvoid CAbstractGUIItem::set_color(GLfloat r, GLfloat g, GLfloat b)
	{
		shape->set_color(r,g,b);		
	}

	GLvoid CAbstractGUIItem::scale(GLfloat scale_x, GLfloat scale_y)
	{
		extent.width*=scale_x;
		extent.height*=scale_y;
		init();
	}

	GLvoid CAbstractGUIItem::scale(GLfloat scale_x, GLfloat scale_y, EXTENT *extent)
	{
		extent->width*=scale_x;
		extent->height*=scale_y;
		init(extent);
	}

	GLvoid CAbstractGUIItem::translate(GLfloat x, GLfloat y, GLfloat z)
	{
		extent.x_pos+=x;
		extent.y_pos+=y;
		extent.z_pos+=z;
		init();
	}

	GLvoid CAbstractGUIItem::translate(GLfloat x, GLfloat y, GLfloat z, EXTENT *extent)
	{
		extent->x_pos+=x;
		extent->y_pos+=y;
		extent->z_pos+=z;
		init(extent);
	}

	GLvoid CAbstractGUIItem::push_state()
	{
		tmp_extent.set(extent);
	}

	GLvoid CAbstractGUIItem::push_state(EXTENT *extent)
	{
		tmp_extent.set(*extent);
	}

	GLvoid CAbstractGUIItem::pop_state()
	{
		extent.set(tmp_extent);
	}

	GLvoid CAbstractGUIItem::pop_state(EXTENT *extent)
	{
		extent->set(tmp_extent);
	}

	bool CAbstractGUIItem::do_check_mouse()
	{
		return check_mouse;
	}

	GLvoid CAbstractGUIItem::set_mouse_check(bool check_mouse)
	{
		this->check_mouse=check_mouse;
	}

	GLuint CAbstractGUIItem::get_index_number()
	{
		return index_number;
	}

	GLvoid CAbstractGUIItem::set_name(GLint name, GLint name_index)
	{
		if (name_index>=0 && name_index<MAX_NAMES)
		{
			this->name[name_index]=name;
		}
	}

	GLint CAbstractGUIItem::get_name(GLint name_index)
	{
		return name[name_index];
	}

	GLvoid *CAbstractGUIItem::get_parent()
	{
		return parent;
	}

	GLvoid CAbstractGUIItem::set_parent(GLvoid *parent)
	{
		this->parent=parent;
	}

	GLvoid CAbstractGUIItem::set_class_name(char *class_name)
	{
		this->class_name = new char[strlen(class_name)+1];
		strcpy(this->class_name,class_name);
	}

	char *CAbstractGUIItem::get_class_name()
	{
		return class_name;
	}

	GLvoid CAbstractGUIItem::set_visible(bool visible)
	{
		this->visible=visible;
	}

	bool CAbstractGUIItem::is_visible()
	{
		return visible;
	}

	GLvoid CAbstractGUIItem::set_ready(bool ready)
	{
		this->ready=ready;
	}

	bool CAbstractGUIItem::is_ready()
	{
		return ready;
	}

	GLvoid CAbstractGUIItem::update()
	{
	}

	GLvoid CAbstractGUIItem::set_width(GLfloat width)
	{
		extent.width=width;
		init();
	}

	GLvoid CAbstractGUIItem::set_height(GLfloat height)
	{
		extent.height=height;
		init();
	}

	GLvoid CAbstractGUIItem::set_x_pos(GLfloat x_pos)
	{
		extent.x_pos=x_pos;
		init();
	}

	GLvoid CAbstractGUIItem::set_y_pos(GLfloat y_pos)
	{
		extent.y_pos=y_pos;
		init();
	}

	GLvoid CAbstractGUIItem::set_extent(EXTENT extent)
	{
		this->extent.set(extent);
		init();
	}

	GLvoid CAbstractGUIItem::set_mouse_button(MOUSE_BUTTON_PRESSED mouse_button_pressed)
	{
		this->mouse_button_pressed=mouse_button_pressed;
	}

	CAbstractGUIItem::MOUSE_BUTTON_PRESSED CAbstractGUIItem::get_mouse_button()
	{
		return mouse_button_pressed;
	}
}