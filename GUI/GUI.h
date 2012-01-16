#ifndef GUI_H
#define GUI_H

/* how many config lines can get read */
#define MAX_CONFIG_FROM_FILE 512
#define MAX_CONFIG_FILES 128

#include "AbstractGUIItem.h"
#include "DKInput.h"
#include "../system.h"
#include <GL/gl.h>

#include <vector>

using namespace std;

namespace GUI
{
	class CGUI
	{
	public:
		CGUI(GLint screen_width, GLint screen_height);
		~CGUI();

		GLvoid init();
		GLvoid updateRatio(GLint screen_width, GLint screen_height);
		GLvoid draw();
		GLvoid do_actions();

		GLint add_item(CAbstractGUIItem *item);

		CDKInput *get_input_controler();

		GLvoid force_vertical_alignment();
		GLvoid force_horizontal_alignment();
		bool force_file_alignment(const char *file);

		bool is_mouse_over_gui();

	private:
		GLint screen_width, screen_height;
		CDKInput *input;
		vector<CAbstractGUIItem*> gui_items;

		// mouse checking etc...
		GLint mouse_x, mouse_y;
		CAbstractGUIItem *tmp_item;

		GLvoid check_mouse_over();
		GLvoid check_mouse_down();
		GLvoid check_mouse_up();
		GLvoid check_mouse_out();
		GLvoid check_mouse_click();

		bool mouse_in_item_extent(CAbstractGUIItem *item);

		CAbstractGUIItem *last_visited;

		// mouse click temps
		bool down, click;

		GLuint old_index, new_index;

		struct ALIGNMENT_DATA
		{
			char name[50];
			GLfloat x,y,width,height;
		};

		struct ALIGNMENT
		{
			ALIGNMENT_DATA alignment_data[MAX_CONFIG_FROM_FILE]; 
			GLint configs;

			GLvoid init()
			{
				configs=0;
			}

			bool add(ALIGNMENT_DATA &fad)
			{
				if (configs<MAX_CONFIG_FROM_FILE-1)
				{
					strcpy(alignment_data[configs].name,fad.name);
					alignment_data[configs].x=fad.x;
					alignment_data[configs].y=fad.y;
					alignment_data[configs].width=fad.width;
					alignment_data[configs++].height=fad.height;
				}
				else
				{
					return false;
				}

				return true;
			}

			ALIGNMENT_DATA *get_alignment_data(char *name)
			{
				if (!name)
				{
					return NULL;
				}

				for (GLint i=0; i<configs; i++)
				{
					if (strcmp(alignment_data[i].name,name)==0)
					{
						return &alignment_data[i];
					}
				}
				return NULL;
			}
		};

		ALIGNMENT alignment;

		struct CONFIG_CONTROL
		{
			char *loaded_configs[MAX_CONFIG_FILES];
			GLint config_files_count;
		
			GLvoid init()
			{
				config_files_count=0;
			}

			GLvoid clear()
			{
				for (GLint i=0; i<config_files_count; i++)
				{
					delete [] loaded_configs[i];
				}
			}

			bool add_config_name(const char *config_name)
			{
				if (config_files_count<MAX_CONFIG_FILES-1)
				{
					loaded_configs[config_files_count] = new char[strlen(config_name)+1];
					strcpy(loaded_configs[config_files_count++],config_name);
				}
				else
				{
					return false;
				}
				return true;
			}

			bool config_exists(const char *config_name)
			{
				for (GLint i=0; i<config_files_count; i++)
				{
					if (strcmp(loaded_configs[i],config_name)==0)
					{
						return true;
					}
				}
				return false;
			}
		};

		CONFIG_CONTROL config_control;
	};
}

#endif // GUI_H