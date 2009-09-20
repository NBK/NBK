#ifndef PLAY_GUI_H
#define PLAY_GUI_H

#define MAX_GUI_PARTS	64
#define TAB_COUNT		5

#include "DKcommons.h"

#include "GUI.h"
#include "GUIBackground.h"
using namespace GUI;

#include "DKGUILabel.h"
#include "DKRoomInfoTexture.h"
#include "TabButton.h"
#include "ItemButton.h"
#include "SpeedButton.h"
#include "AnimatedSpeedButton.h"
#include "DKProgressBar.h"
#include "MoneyLabel.h"
using namespace DK_GUI;

#include "DKTextureList.h"
#include <vector>

using namespace std;

namespace DK_GUI
{
	class CPlayGUI
	{
	public:
		CPlayGUI(GLint screen_width, GLint screen_height, CDKTextureList *game_textures);
		~CPlayGUI();

		GLvoid draw_and_do_actions();

		// called from children
		GLvoid set_action(bool clicked, bool event, GLint message, GLint message_group, GLvoid *sender);

		ACTION_EVENT *get_last_action();
		GLvoid reset_action();
		CGUI *get_active_gui();

		bool is_mouse_over_gui();

		CDKGUILabel *get_room_count_label(GLint player);
		CDKGUILabel *get_creature_count_label(GLint player);

		/* some things are connected with right mouse button */
		GLvoid on_rmb_down();

		/* and some things are connected with left mouse button */
		GLvoid on_lmb_down();

		GLvoid set_parent(GLvoid *parent);

		// updates the room info labels, should be called when eg; selling a room, creating a room, reloading level....
		GLvoid updateRoomInfo();

	private:
		CGUI *temp_doors;
		ACTION_EVENT action_event;	

		CGUI *play_gui;

		enum GUI_PART{GP00=0, GP01, GP10, GP11, GP20, GP21, GP30, GP31};
		enum TAB_CONTROL_PART{TCP_QUERY=0, TCP_BUILD, TCP_RESEARCH, TCP_WORKSHOP, TCP_CREATURE};

		/* play gui construction parts */
		CGUIBackground *gui_parts[MAX_GUI_PARTS];

		/* the TAB buttons */
		CTabButton *tab_button[TAB_COUNT];

		/* tells which tab control is activated */
		struct TAB_CONTROL
		{
			vector<CAbstractGUIItem*> items;
			bool active;

			TAB_CONTROL()
			{
				active=false;
			}

			GLvoid add_item(CAbstractGUIItem *item)
			{
				items.push_back(item);
			}
		};

		/* we have 5 tabs */
		TAB_CONTROL tab_control[5];

		/* activates the specified control, disables the others */
		GLvoid activate_control(GLint control);

		GLvoid init_tab(bool create_new);

		CDKTextureList *game_textures;

		GLvoid add_QUERY_widgets();
		GLvoid add_BUILD_widgets();
		GLvoid add_RESEARCH_widgets();
		GLvoid add_WORKSHOP_widgets();
		GLvoid add_CREATURE_widgets();

		static char *tab_item_class_name[];

		/* room and creature cound control variables */
		CDKGUILabel *room_count_label[4];
		CDKGUILabel *creature_count_label[4];

		/* keeping the control of the payday */
		CDKGUILabel *payday_wage_size;

		/* simple function for adding components */
		GLvoid add(CAbstractGUIItem *item, TAB_CONTROL_PART tcp, bool add_to_gui=true);

		/* used for adjusting payday length... */
		CDKProgressBar *payday_progress_bar;

		/* showing the time to new library research */
		CDKProgressBar *research_progress_bar;

		/* showing the time to new library workshop */
		CDKProgressBar *workshop_progress_bar;

		/* quick access to the room buttons */
		CItemButton *room_buttons[MAX_BUTTONS];

		/* a progress bar showing the the room data: ocupation */
		CDKProgressBar *room_data_progress_bar;

		/* the room info texture */
		CDKRoomInfoTexture *room_info_texture;

		/* the gfx for room data (progress bar) */
		CGUIBackground *room_info_panel;

		/* the label that displays how many rooms are there in the dk_map os same type*/
		CDKGUILabel *build_room_count_label;

		/* the label that displays the selected room cost */
		CDKGUILabel *build_room_cost_label;

		/* if we are building something then this is set to what we're building*/
		enum GUI_ACTION{GA_ROOMS,GA_TRAPS,GA_DOORS,GA_SPELLS,GA_NONE};
		GUI_ACTION action;

		/* holds the last thing while we're building */
		GLint last_built;

		/* quick access to the trap buttons */
		CItemButton *workshop_buttons[MAX_BUTTONS];

		/* info about traps and doors */
		CDKRoomInfoTexture *trap_door_info_texture;
		CDKGUILabel *build_trap_door_count_label;
		CDKGUILabel * build_trap_door_cost_label;

		/* quick access to the spell buttons */
		CItemButton *spell_buttons[MAX_BUTTONS];

		CDKRoomInfoTexture *spell_info_texture;
		CDKGUILabel *spell_cost_label;

		/* the (in)famous money label */
		CMoneyLabel *money_label;

		/* form minimap zoom (in/out) */
		CSpeedButton	*minimap_zoom_in,
						*minimap_zoom_out;

		GLvoid *parent;
	};
}

#endif // PLAY_GUI_H