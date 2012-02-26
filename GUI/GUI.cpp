#include "GUI.h"

namespace GUI
{
	CGUI::CGUI(GLint screen_width, GLint screen_height)
	{
		this->screen_width=screen_width;
		this->screen_height=screen_height;
		input = new CDKInput();
		last_visited=NULL;
		tmp_item=NULL;
		down=click=false;
		alignment.init();
		config_control.init();
	}

	CGUI::~CGUI()
	{
		delete input;

		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{
			tmp_item=*item;
			delete tmp_item;
		}

		config_control.clear();
	}

	GLvoid CGUI::init()
	{
		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{
			(*item)->init(screen_width,screen_height);
		}
	}

	GLvoid CGUI::updateRatio(GLint screen_width, GLint screen_height)
	{
		this->screen_width=screen_width;
		this->screen_height=screen_height;
		init();
	}

	GLvoid CGUI::draw()
	{
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_DEPTH_TEST);
		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{
			if ((*item)->is_visible() && (*item)->is_ready())
			{
				(*item)->draw();
			}
		}	
	}

	GLvoid CGUI::do_actions()
	{
		mouse_x=input->get_screen_x();
		mouse_y=input->get_screen_y();

		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{
			tmp_item=*item;
			if (tmp_item->do_check_mouse() && tmp_item->is_visible() && (*item)->is_ready())
			{
				check_mouse_over();
				check_mouse_out();				
				check_mouse_down();
				check_mouse_up();				
				check_mouse_click();
			}

			/* we also uptdate items */
			tmp_item->update();
		}
	}

	bool CGUI::is_mouse_over_gui()
	{
		mouse_x=input->get_screen_x();
		mouse_y=input->get_screen_y();

		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{
			if (mouse_in_item_extent(*item))
			{
				return true;	
			}
		}
		return false;
	}

	GLvoid CGUI::check_mouse_over()
	{
		if (mouse_in_item_extent(tmp_item))
		{
			if (last_visited==NULL)
			{
				last_visited=tmp_item;		
			}
			tmp_item->on_mouse_over();			
		}
		else
		{
			tmp_item->on_not_mouse_over();
		}
	}

	GLvoid CGUI::check_mouse_out()
	{
		if (last_visited!=NULL && !mouse_in_item_extent(last_visited))
		{
			last_visited->on_mouse_out();
			last_visited=NULL;
		}
	}

	GLvoid CGUI::check_mouse_down()
	{
		bool lmb = input->is_lmouse_down();
		bool rmb = input->is_rmouse_down();
		if ((lmb || rmb) && mouse_in_item_extent(tmp_item))
		{			
			if (!down)
			{
				old_index=tmp_item->get_index_number();
			}
			down=true;
			tmp_item->set_mouse_button((lmb?CAbstractGUIItem::MBP_LEFT:CAbstractGUIItem::MBP_RIGHT));
			tmp_item->on_mouse_down();			
		}
	}

	GLvoid CGUI::check_mouse_up()
	{
		if (down && (!input->is_lmouse_down() && !input->is_rmouse_down()) && mouse_in_item_extent(tmp_item))
		{
			tmp_item->on_mouse_up();
			if (down)
			{
				new_index=tmp_item->get_index_number();

				if (old_index==new_index)
				{
					click=true;
				}
			}
			down=false;
		}
	}

	GLvoid CGUI::check_mouse_click()
	{
		if (click)
		{
			tmp_item->on_mouse_click();
			click=false;
		}
	}

	bool CGUI::mouse_in_item_extent(CAbstractGUIItem *item)
	{
		if (!item)
		{
			return false;
		}

		CAbstractGUIItem::EXTENT *extent = item->get_extent();
		CAbstractGUIItem::MOUSE_DETECTION_EXTENT_OFFSET *mouse_detection = item->get_mouse_detection_extent();

		GLfloat ls = extent->pixel_pos_x + mouse_detection->pixel_correction_from_left;
		GLfloat rs = extent->pixel_pos_x + extent->pixel_width + mouse_detection->pixel_correction_from_rigth;
		GLfloat ts = extent->pixel_pos_y + mouse_detection->pixel_correction_from_top;
		GLfloat bs = extent->pixel_pos_y + extent->pixel_height + mouse_detection->pixel_correction_from_bottom;

		bool lr = ((mouse_x > ls) && (mouse_x < rs));
		bool tb = ((mouse_y > ts) && (mouse_y < bs));

		return (lr && tb);
	}

	GLint CGUI::add_item(CAbstractGUIItem *item)
	{
		gui_items.push_back(item);
		return (GLint)gui_items.size();
	}

	CDKInput *CGUI::get_input_controler()
	{
		return input;
	}

	GLvoid CGUI::force_vertical_alignment()
	{
		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{		
			(*item)->get_extent()->x_pos=0.5f - (*item)->get_extent()->width/2.0f;
			(*item)->init();
		}
	}

	GLvoid CGUI::force_horizontal_alignment()
	{
		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{		
			(*item)->get_extent()->y_pos=0.5f - (*item)->get_extent()->height/2.0f;
			(*item)->init();
		}
	}

	bool CGUI::force_file_alignment(const char *file)
	{
		/* first check if we already opened the config file */
		if (!config_control.config_exists(file))
		{
			FILE *f = NULL;

			if (!(f=fopen(file,"r")))
			{
				fprintf(stderr, "Can't read %s file\n", file);
				return false;
			}

			/* 
				the config line has the following structure: names must not contain empty spaces

				item_name x_position y_position width height;
			*/

			/* 10 KB for config file */
			char *tmp_buff = new char[1024*10];

			size_t read_count=fread(tmp_buff,1,1024*10,f);
			fclose(f);

			size_t pos=0;

			while (pos<read_count)
			{
				char line[100];
				GLint tmp_index=0;
				while (pos<read_count && tmp_buff[pos]!=';')
				{
					line[tmp_index++]=tmp_buff[pos++];
				}
				line[tmp_index]='\0';
				pos+=2;

				/* we have the line now we parse it */
				ALIGNMENT_DATA ad;
				sscanf(line,"%s %f %f %f %f",ad.name,&ad.x,&ad.y,&ad.width,&ad.height);
				alignment.add(ad);
			}
			delete [] tmp_buff;
			config_control.add_config_name(file);
		}

		/* we have the data now we can use it */
		for (vector<CAbstractGUIItem*>::iterator item=gui_items.begin(); item!=gui_items.end(); item++)
		{	
			ALIGNMENT_DATA *ad = alignment.get_alignment_data((*item)->get_class_name());

			/* if we found the correct alignment we apply it */
			if (ad)
			{				
				(*item)->set_extent(CAbstractGUIItem::EXTENT(ad->x,ad->y,0.0f,ad->width,ad->height));
			}
		}

		return true;
	}
}