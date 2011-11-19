#include "../commons.h"
#include "PlayGUI.h"
#include "DKcommons.h"

GLint PLAYER0_MONEY;

using namespace game_utils;

namespace DK_GUI
{

	char *CPlayGUI::tab_item_class_name[] =
	{
		"TI00","TI01","TI02","TI03",
		"TI04","TI05","TI06","TI07",
		"TI08","TI09","TI10","TI11",
		"TI12","TI13","TI14","TI15"
	};

	CPlayGUI::CPlayGUI(GLint screen_width, GLint screen_height, CDKTextureList *game_textures): CInputListener()
	{	
		// TODO: read this from script
		PLAYER0_MONEY=100000;

		// register to handle input
		CV_GAME_MANAGER->getControlManager()->getInput()->registerListener(this);

		this->game_textures=game_textures;

		play_gui = new CGUI(screen_width,screen_height);

		/* creating the GUI parts */
		char class_name[]="GP__";
		GLint part_pos=GP00;
		for (GLint i=0; i<4; i++)
		{
			for (GLint j=0; j<2; j++)
			{
				class_name[2]=(char)((char)i+(int)'0');
				class_name[3]=(char)((char)j+(int)'0');
				gui_parts[part_pos] = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				gui_parts[part_pos]->set_class_name(class_name);
				gui_parts[part_pos]->set_texture(game_textures->get_texture_data_by_name(gui_parts[part_pos]->get_class_name()).texture);
				gui_parts[part_pos]->set_parent(this);
				part_pos++;
			}
		}

		/* add all the gui parts to the gui */
		for (GLint i=GP00; i<=GP31; i++)
		{
			play_gui->add_item(gui_parts[i]);
		}

		/* add the money label */
		money_label = new CMoneyLabel();
		money_label->set_value(PLAYER0_MONEY);
		play_gui->add_item(money_label);

		/* zoom in */
		minimap_zoom_in = new CSpeedButton(game_textures,MINIMAP_BUTTON_ZOOM_IN);
		minimap_zoom_in->set_parent(this);
		minimap_zoom_in->set_class_name(MINIMAP_ZOOM_IN_CLASS_NAME);
		minimap_zoom_in->set_name(AEMG_MINIMAP_CONTROL,1);
		play_gui->add_item(minimap_zoom_in);

		/* zoom out */
		minimap_zoom_out = new CSpeedButton(game_textures,MINIMAP_BUTTON_ZOOM_OUT);
		minimap_zoom_out->set_parent(this);
		minimap_zoom_out->set_class_name(MINIMAP_ZOOM_OUT_CLASS_NAME);
		minimap_zoom_out->set_name(AEMG_MINIMAP_CONTROL,1);
		play_gui->add_item(minimap_zoom_out);

		/* set the active tab */
		tab_control[TCP_QUERY].active=false;
		tab_control[TCP_BUILD].active=false;
		tab_control[TCP_RESEARCH].active=true;
		tab_control[TCP_WORKSHOP].active=false;
		tab_control[TCP_CREATURE].active=false;

		/* init the tabs */
		init_tab(true);

		/* add all the tabs to the gui */
		for (GLint i=TCP_QUERY; i<=TCP_CREATURE; i++)
		{
			play_gui->add_item(tab_button[i]);
		}

		/* create the buttons for the 3 tab controls: build rooms, traps  and spells */
		add_QUERY_widgets();
		add_BUILD_widgets();
		add_RESEARCH_widgets();
		add_WORKSHOP_widgets();
		add_CREATURE_widgets();

		/* init the tabs again, hide newely created items */
		init_tab(false);

		/* now init the GUI */
		play_gui->init();
		play_gui->force_file_alignment("Data/resources/GFX/GUI/alignment.gac");

		/* fix te top texture because of force alignment */
		/*for (GLint i=WOODEN_DOORS_NAME; i<=MAGIC_DOORS_NAME; i++)
		{
			door[i]->fix_top_texture();
		}*/

		action=GA_NONE;
		last_built=-1;		
	}

	GLvoid CPlayGUI::add(CAbstractGUIItem *item, TAB_CONTROL_PART tcp, bool add_to_gui)
	{
		if (add_to_gui)
		{
			play_gui->add_item(item);
		}
		tab_control[tcp].add_item(item);
	}

	GLvoid CPlayGUI::add_QUERY_widgets()
	{
		/* there are 10 simple "panels" on this tab */
		CGUIBackground *simple_panel = NULL;
		char class_name[]="QUERY_TAB_PANEL_";
		for (GLint i=0; i<8; i++)
		{
			class_name[15]=(char)((char)i+(int)'0');
			/* create a a special part of the tab section: the info displayer */
			simple_panel = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
			simple_panel->set_class_name(class_name);
			simple_panel->set_texture(game_textures->get_texture_by_name("QUERY_TAB_SIMPLE_PANEL"));
			simple_panel->set_parent(this);

			add(simple_panel,TCP_QUERY);
		}

		/* the QUERY button */
		CSpeedButton *info = new CSpeedButton(game_textures,SPEED_BUTTON_QUERY);
		info->set_parent(this);
		info->set_class_name("QUERY_TAB_INFO");

		play_gui->add_item(info);
		tab_control[TCP_QUERY].add_item(info);		

		/* add the CHICKEN button */
		char *anim_names[]=	{"QUERY_TAB_CHICKEN_ANIM1","QUERY_TAB_CHICKEN_ANIM2"};
		CAnimatedSpeedButton *chicken = new CAnimatedSpeedButton(game_textures,SPEED_BUTTON_CHICKEN,2,anim_names);
		chicken->set_parent(this);
		chicken->set_class_name("QUERY_TAB_CHICKEN");

		add(chicken,TCP_QUERY);

		/* add the PRISON button */
		char *anim_names_[]= {"QUERY_TAB_PRISON_ANIM1","QUERY_TAB_PRISON_ANIM2"};
		CAnimatedSpeedButton *prison = new CAnimatedSpeedButton(game_textures,SPEED_BUTTON_PRISON,2,anim_names_);
		prison->set_parent(this);
		prison->set_class_name("QUERY_TAB_PRISON");

		add(prison,TCP_QUERY);

		/* add the creature number info and room number info */
		//GLint player_count=GET_MAP->get_player_count(); // TODO
		GLint player_count=1;

		if (player_count>0)
		{
			char class_name_creatures[]="QUERY_TAB_CREATURE_COUNT_PLAYER_";
			char class_name_rooms[]="QUERY_TAB_ROOM_COUNT_PLAYER1";
			char class_name_rooms_label[]="QUERY_TAB_ROOM_COUNT_LABEL_PLAYER_";
			char class_name_creature_label[]="QUERY_TAB_CREATURE_COUNT_LABEL_PLAYER_";

			CGUIBackground *cc = NULL, *rc = NULL;

			for (GLint i=0; i<4; i++)
			{
				/*if (!GET_MAP->is_player_available(i))
				{
					continue;
				}*/

				/* the creature counter */
				class_name_creatures[strlen(class_name_creatures)-1]=(char)((char)i+(int)'0');
				cc = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				cc->set_class_name(class_name_creatures);
				cc->set_texture(game_textures->get_texture_by_name(cc->get_class_name()));
				cc->set_parent(this);

				add(cc,TCP_QUERY);

				/* the room counter */
				class_name_rooms[strlen(class_name_rooms)-1]=(char)((char)i+(int)'0');
				rc = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				rc->set_class_name(class_name_rooms);
				rc->set_texture(game_textures->get_texture_by_name(rc->get_class_name()));
				rc->set_parent(this);

				add(rc,TCP_QUERY);

				/* the room count label */
				class_name_rooms_label[strlen(class_name_rooms_label)-1]=(char)((char)i+(int)'0');
				room_count_label[i] = new CDKGUILabel("0");
				room_count_label[i]->set_caption_color(CV_PLAYER_COLOR[i]);
				room_count_label[i]->set_class_name(class_name_rooms_label);
				room_count_label[i]->set_parent(this);
				room_count_label[i]->set_caption("0");
				room_count_label[i]->int_set(CV_GAME_MANAGER->getRoomManager()->getRoomCount(i));

				add(room_count_label[i],TCP_QUERY);

				/* the creature count label */
				class_name_creature_label[strlen(class_name_creature_label)-1]=(char)((char)i+(int)'0');
				creature_count_label[i] = new CDKGUILabel("0");
				creature_count_label[i]->set_caption_color(CV_PLAYER_COLOR[i]);
				creature_count_label[i]->set_class_name(class_name_creature_label);
				creature_count_label[i]->set_parent(this);
				creature_count_label[i]->set_caption("0");

				// need to create creature count per player
				creature_count_label[i]->int_set(CV_GAME_MANAGER->getCreatureManager()->getCreatureCount(i));

				add(creature_count_label[i],TCP_QUERY);

				/* research progress bar */
				CGUIBackground *research_progress_bar_texture = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				research_progress_bar_texture->set_class_name("QUERY_TAB_RESEARCH_PROGRESS_BAR_TEXTURE");
				research_progress_bar_texture->set_texture(game_textures->get_texture_by_name(research_progress_bar_texture->get_class_name()));
				research_progress_bar_texture->set_parent(this);

				add(research_progress_bar_texture,TCP_QUERY);

				/* research picture  picture*/
				CGUIBackground *research_texture = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				research_texture->set_class_name("QUERY_TAB_RESEARCH_PICTURE");
				research_texture->set_texture(game_textures->get_texture_by_name(research_texture->get_class_name()));
				research_texture->set_parent(this);
				add(research_texture,TCP_QUERY);

				/* research progress bar */
				research_progress_bar = new CDKProgressBar();
				research_progress_bar->set_color(0.0f,0.0f,0.0f);
				research_progress_bar->set_class_name("QUERY_TAB_RESEARCH_PROGRESS_BAR");
				research_progress_bar->set_parent(this);
				research_progress_bar->set_style(CDKProgressBar::PBS_FROM_RIGHT);				
				research_progress_bar->set_increment(0.00001f);
				research_progress_bar->set_inverted(true);
				add(research_progress_bar,TCP_QUERY);

				/* workshop progress bar picture*/
				CGUIBackground *workshop_progress_bar_texture = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				workshop_progress_bar_texture->set_class_name("QUERY_TAB_WORKSHOP_PROGRESS_BAR_TEXTURE");
				workshop_progress_bar_texture->set_texture(game_textures->get_texture_by_name(workshop_progress_bar_texture->get_class_name()));
				workshop_progress_bar_texture->set_parent(this);
				add(workshop_progress_bar_texture,TCP_QUERY);

				/* workshop picture */
				CGUIBackground *workshop_texture = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				workshop_texture->set_class_name("QUERY_TAB_WORKSHOP_PICTURE");
				workshop_texture->set_texture(game_textures->get_texture_by_name(workshop_texture->get_class_name()));
				workshop_texture->set_parent(this);
				add(workshop_texture,TCP_QUERY);

				/* workshop progress bar */
				workshop_progress_bar = new CDKProgressBar();
				workshop_progress_bar->set_color(0.0f,0.0f,0.0f);
				workshop_progress_bar->set_class_name("QUERY_TAB_WORKSHOP_PROGRESS_BAR");
				workshop_progress_bar->set_parent(this);
				workshop_progress_bar->set_style(CDKProgressBar::PBS_FROM_RIGHT);				
				workshop_progress_bar->set_increment(0.00001f);
				workshop_progress_bar->set_inverted(true);
				add(workshop_progress_bar,TCP_QUERY);

				/* payday panel */
				CGUIBackground *payday_panel = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
				payday_panel->set_class_name("QUERY_TAB_PAYDAY_PROGRESS_BAR_PANEL");
				payday_panel->set_texture(game_textures->get_texture_by_name(payday_panel->get_class_name()));
				payday_panel->set_parent(this);
				add(payday_panel,TCP_QUERY);

				/* payday progress bar */
				payday_progress_bar = new CDKProgressBar();
				payday_progress_bar->set_color(0.0f,0.0f,0.0f);
				payday_progress_bar->set_class_name("QUERY_TAB_PAYDAY_PROGRESS_BAR");
				payday_progress_bar->set_parent(this);
				payday_progress_bar->set_style(CDKProgressBar::PBS_FROM_RIGHT);				
				payday_progress_bar->set_increment(0.00001f);
				payday_progress_bar->set_inverted(true);
				add(payday_progress_bar,TCP_QUERY);

				/* payday wage size (value) lablel */
				payday_wage_size = new CDKGUILabel("0");
				payday_wage_size->set_caption_color(0.717647059f,0.180392157f,0.0f);
				payday_wage_size->set_caption_size(20);
				payday_wage_size->set_name(PAYDAY_WAGE_VALUE_LABEL,0);
				payday_wage_size->set_class_name("QUERY_TAB_PAYDAY_WAGE_VALUE");
				payday_wage_size->set_parent(this);
				add(payday_wage_size,TCP_QUERY);
			}
		}
	}

	GLvoid CPlayGUI::add_BUILD_widgets()
	{	
		/* there are 16 item buttons */

		/* can't use class name for getting texture, because class name is reserverd for positioning the item */
		char texture_name[]="ROOM___BUTTON_TOP_TEXTURE";
			
		for (GLint i=ROOM_BUTTON_TREASURE; i<=ROOM_BUTTON_SELL; i++)
		{
			texture_name[4]=(i<10?'0':'1');
			texture_name[5]=(char)((char)(i<10?i:i-10)+(int)'0');
			room_buttons[i] = new CItemButton(game_textures,i);
			room_buttons[i]->set_parent(this);
			room_buttons[i]->set_class_name(tab_item_class_name[i]);
			room_buttons[i]->set_button_state(CItemButton::BS_AVAILABLE_USED);
			room_buttons[i]->set_top_texture(game_textures->get_texture_by_name(texture_name));
			/* name[0] is used for action they contain */
			room_buttons[i]->set_name(i,0);
			/* name[1] is used for group to which this actions belong */
			room_buttons[i]->set_name(AEMG_BUILD_ROOMS,1);
			/* name[2] is used for room locating thingy */
			room_buttons[i]->set_name(AEMG_LOCATE_ROOM,2);

			/* sell button is also in workshop GUI part */
			if (i==ROOM_BUTTON_SELL)
			{
				add(room_buttons[i],TCP_WORKSHOP,false);
			}
			add(room_buttons[i],TCP_BUILD);
		}

		/* some special adjustment */
		room_buttons[ROOM_BUTTON_SELL]->set_name(AEMG_BUILD_ROOMS,1);
		room_buttons[ROOM_BUTTON_SCAVENGER]->set_button_state(CItemButton::BS_UNAVAILABLE);
		room_buttons[ROOM_BUTTON_EMPTY]->set_button_state(CItemButton::BS_UNAVAILABLE);

		/* create a a special part of the tab section: the info displayer */
		CGUIBackground *room_info_panel_below = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
		room_info_panel_below->set_class_name("INFO_PANEL");
		room_info_panel_below->set_texture(game_textures->get_texture_by_name(room_info_panel_below->get_class_name()));
		room_info_panel_below->set_parent(this);
		add(room_info_panel_below,TCP_BUILD);

		/* create a a special part of the tab section: the info displayer */
		room_info_panel = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
		room_info_panel->set_class_name("BUILD_TAB_ROOM_DATA_PROGRESS_BAR_TEXTURE");
		room_info_panel->set_texture(game_textures->get_texture_by_name(room_info_panel->get_class_name()));
		room_info_panel->set_parent(this);
		room_info_panel->set_ready(false);
		add(room_info_panel,TCP_BUILD);

		/* add the room progress bar for showing the room data */
		room_data_progress_bar = new CDKProgressBar();
		room_data_progress_bar->set_color(0.0f,0.0f,0.0f);
		room_data_progress_bar->set_class_name("BUILD_TAB_ROOM_DATA_PROGRESS_BAR");
		room_data_progress_bar->set_parent(this);
		room_data_progress_bar->set_style(CDKProgressBar::PBS_FROM_RIGHT);				
		room_data_progress_bar->set_increment(0.00001f);
		room_data_progress_bar->set_inverted(true);
		room_data_progress_bar->set_ready(false);
		add(room_data_progress_bar,TCP_BUILD);

		/* add the room build texture where the room data gets displayed */
		room_info_texture = new CDKRoomInfoTexture(game_textures);
		room_info_texture->set_class_name("BUILD_TAB_ROOM_INFO_TEXTURE");
		room_info_texture->set_parent(this);
		room_info_texture->set_ready(false);
		add(room_info_texture,TCP_BUILD);
		
		/* add the same type room count */
		build_room_count_label = new CDKGUILabel("x0");
		build_room_count_label->set_caption_color(1.0f,0.0f,0.0f);
		build_room_count_label->set_class_name("ROOM_COUNT_LABEL");
		build_room_count_label->set_parent(this);
		build_room_count_label->set_ready(false);
		add(build_room_count_label,TCP_BUILD);

		/* add the same type room cost */
		build_room_cost_label = new CDKGUILabel("0");
		build_room_cost_label->set_caption_color(1.0f,0.0f,0.0f);
		build_room_cost_label->set_class_name("ROOM_COST_LABEL");
		build_room_cost_label->set_parent(this);
		build_room_cost_label->int_set(0);
		build_room_cost_label->set_ready(false);
		add(build_room_cost_label,TCP_BUILD);

	}

	GLvoid CPlayGUI::add_RESEARCH_widgets()
	{
		CItemButton *item=NULL;
		CGUIBackground *info_panel=NULL;

		char texture_name[]="SPELL___BUTTON_TOP_TEXTURE";

		/* there are 16 item buttons */
		for (GLint i=SPELL_BUTTON_POSSESS_MONSTER; i<=SPELL_BUTTON_DESTROY_WALL; i++)
		{
			texture_name[5]=(i<10?'0':'1');
			texture_name[6]=(char)((char)(i<10?i:i-10)+(int)'0');
			spell_buttons[i] = new CItemButton(game_textures,i);
			spell_buttons[i]->set_parent(this);
			spell_buttons[i]->set_class_name(tab_item_class_name[i]);
			spell_buttons[i]->set_button_state(CItemButton::BS_AVAILABLE_USED);
			spell_buttons[i]->set_top_texture(game_textures->get_texture_by_name(texture_name));
			/* name[0] is used for action they contain */
			spell_buttons[i]->set_name(i,0);
			/* name[1] is used for group to which this actions belong */
			spell_buttons[i]->set_name(AEMG_EXECUTE_SPELL,1);

			/* name[2] is used for room locating thingy */
			spell_buttons[i]->set_name(AEMG_NONE,2);

			add(spell_buttons[i],TCP_RESEARCH);
		}

		/* create a a special part of the tab section: the info displayer */
		info_panel = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
		info_panel->set_class_name("INFO_PANEL");
		info_panel->set_texture(game_textures->get_texture_by_name(info_panel->get_class_name()));
		info_panel->set_parent(this);
		add(info_panel,TCP_RESEARCH);

		/* when trap/door selected its bigger icon will flash */
		spell_info_texture = new CDKRoomInfoTexture(game_textures);
		spell_info_texture->set_class_name("RESEARCH_TAB_SPELL_INFO_TEXTURE");
		spell_info_texture->set_parent(this);
		spell_info_texture->set_ready(false);
		add(spell_info_texture,TCP_RESEARCH);

		/* add the same type trap_door/door cost */
		spell_cost_label = new CDKGUILabel("0");		
		spell_cost_label->set_caption_size(30);
		spell_cost_label->set_caption_color(1.0f,0.8f,0.1f);
		spell_cost_label->set_class_name("SPELL_COST_LABEL");
		spell_cost_label->set_parent(this);
		spell_cost_label->int_set(0);
		spell_cost_label->set_ready(false);
		add(spell_cost_label,TCP_RESEARCH);
	}

	GLvoid CPlayGUI::add_WORKSHOP_widgets()
	{
		CItemButton *item=NULL;
		CGUIBackground *info_panel=NULL;

		/* there are 15 item buttons, last one (sell) gets copied from research tab */
		char texture_name_t[]="TRAP__BUTTON_TOP_TEXTURE";
		char texture_name_d[]="DOOR__BUTTON_TOP_TEXTURE";

		for (GLint i=TRAP_BUTTON_BOULDER; i<15; i++)
		{
			GLint pos = i-(TRAP_BUTTON_LAVA+2+1);

			texture_name_t[4]=(char)((char)(i)+(int)'0');
			texture_name_d[4]=(char)((char)(pos)+(int)'0');
			workshop_buttons[i] = new CItemButton(game_textures,i);
			workshop_buttons[i]->set_parent(this);
			workshop_buttons[i]->set_class_name(tab_item_class_name[i]);

			if (i<=TRAP_BUTTON_LAVA)
			{
				/* here come traps */
				workshop_buttons[i]->set_button_state(CItemButton::BS_AVAILABLE_USED);
				workshop_buttons[i]->set_top_texture(game_textures->get_texture_by_name(texture_name_t));
				/* name[0] is used for action they contain */
				workshop_buttons[i]->set_name(i,0);
				/* name[1] is used for group to which this actions belong */
				workshop_buttons[i]->set_name(AEMG_BUILD_TRAPS,1);
				/* name[2] is used for trap locating thingy */
				workshop_buttons[i]->set_name(AEMG_LOCATE_TRAP,2);
			}
			else if (i>TRAP_BUTTON_LAVA+2 && i<=DOOR_BUTTON_MAGIC+TRAP_BUTTON_LAVA+3)
			{				
				/* here come doors */
				workshop_buttons[i]->set_button_state(CItemButton::BS_AVAILABLE_USED);
				workshop_buttons[i]->set_top_texture(game_textures->get_texture_by_name(texture_name_d));
				/* name[0] is used for action they contain */
				workshop_buttons[i]->set_name(pos,0);
				/* name[1] is used for group to which this actions belong */
				workshop_buttons[i]->set_name(AEMG_BUILD_DOORS,1);
				/* name[2] is used for trap locating thingy */
				workshop_buttons[i]->set_name(AEMG_LOCATE_DOOR,2);
			}
			else
			{ 
				/* everything else is unavailable */
				workshop_buttons[i]->set_button_state(CItemButton::BS_UNAVAILABLE);
				workshop_buttons[i]->set_name(AEMG_NONE,1);
			}
			add(workshop_buttons[i],TCP_WORKSHOP);
		}

		/* create a a special part of the tab section: the info displayer */
		info_panel = new CGUIBackground(0.0f,0.0f,0.0f,0.0f,0.0f,0);
		info_panel->set_class_name("INFO_PANEL");
		info_panel->set_texture(game_textures->get_texture_by_name(info_panel->get_class_name()));
		info_panel->set_parent(this);

		add(info_panel,TCP_WORKSHOP);

		/* when trap/door selected its bigger icon will flash */
		trap_door_info_texture = new CDKRoomInfoTexture(game_textures);
		trap_door_info_texture->set_class_name("WORKSHOP_TAB_TRAP_DOOR_INFO_TEXTURE");
		trap_door_info_texture->set_parent(this);
		trap_door_info_texture->set_ready(false);
		add(trap_door_info_texture,TCP_WORKSHOP);
		
		/* add the same type trap_door/door count */
		build_trap_door_count_label = new CDKGUILabel("x0");
		build_trap_door_count_label->set_caption_color(1.0f,0.0f,0.0f);
		build_trap_door_count_label->set_class_name("TRAP_DOOR_COUNT_LABEL");
		build_trap_door_count_label->set_parent(this);
		build_trap_door_count_label->set_ready(false);
		add(build_trap_door_count_label,TCP_WORKSHOP);

		/* add the same type trap_door/door cost */
		build_trap_door_cost_label = new CDKGUILabel("0");
		build_trap_door_cost_label->set_caption_color(1.0f,0.0f,0.0f);
		build_trap_door_cost_label->set_class_name("TRAP_DOOR_COST_LABEL");
		build_trap_door_cost_label->set_parent(this);
		build_trap_door_cost_label->int_set(0);
		build_trap_door_cost_label->set_ready(false);
		add(build_trap_door_cost_label,TCP_WORKSHOP);
	}

	GLvoid CPlayGUI::add_CREATURE_widgets()
	{
	}

	GLvoid CPlayGUI::init_tab(bool create_new)
	{
		if (create_new)
		{
			/* the tab controls. */
			char class_name[]="TCP_";
			for (GLint i=TCP_QUERY; i<=TCP_CREATURE; i++)
			{
				class_name[3]=(char)((char)i+(int)'0');
				tab_button[i] = new CTabButton();
				tab_button[i]->set_class_name(class_name);
				tab_button[i]->set_parent(this);
				tab_button[i]->set_name(AEM_QUERY+i,0);
			}
		}

		GLint active=0;

		/* check which one is enabled. and for items: first we hide all and then we show only the active ones */
		for (GLint i=TCP_QUERY; i<=TCP_CREATURE; i++)
		{
			char *tab_name = tab_button[TCP_QUERY+i]->get_class_name();
			GLint len=strlen(tab_name);
			char *tmp = new char[len+2];
			strcpy(tmp,tab_name);
			tmp[len+1]='\0';

			if (tab_control[i].active)
			{
				tmp[len]='E';				
				tab_button[TCP_QUERY+i]->set_texture(game_textures->get_texture_data_by_name(tmp).texture);
			}
			else
			{
				tmp[len]='D';
				tab_button[TCP_QUERY+i]->set_texture(game_textures->get_texture_data_by_name(tmp).texture);
			}

			/* we show or hide the items under tab */
			for (GLuint j=0; j<tab_control[i].items.size(); j++)
			{
				//tab_control[i].items.at(j)->set_visible(tab_control[i].active);
				tab_control[i].items.at(j)->set_visible(false);
			}

			if (tab_control[i].active)
			{
				active=i;
			}

			delete [] tmp;
		}

		/* now show the active tab items */
		for (GLuint j=0; j<tab_control[active].items.size(); j++)
		{
			//tab_control[i].items.at(j)->set_visible(tab_control[i].active);
			tab_control[active].items.at(j)->set_visible(true);
		}
	}

	CPlayGUI::~CPlayGUI()
	{
		delete play_gui;
	}

	GLvoid CPlayGUI::activate_control(GLint control)
	{
		tab_control[control].active=true;

		for (GLint i=0; i<5; i++)
		{
			if (i!=control)
			{
				tab_control[i].active=false;
			}
		}
		init_tab(false);
	}

	GLvoid CPlayGUI::draw_and_do_actions()
	{
		play_gui->draw();
		play_gui->do_actions();

		// draw the jumping labels 
		if (jumping_labels.size()>0)
		{
			CJumpLabel *jumpl=NULL;;
			for (jl_iter=jumping_labels.begin(); jl_iter!=jumping_labels.end(); )
			{
				jumpl=*jl_iter;

				jumpl->draw(CV_GAME_MANAGER->getDeltaTime());

				if (jumpl->is_dead())
				{
					delete jumpl;
					jumpl=NULL;
					jl_iter = jumping_labels.erase(jl_iter);
				}
				else
				{
					jl_iter++;
				}
			}	
		}

		/* check the tab buttons */
		if (action_event.event)
		{
			GLint message_group = action_event.message_group;
			GLint message = action_event.message;

			if (message_group==AEMG_TAB_CONTROL)
			{
				activate_control((GLint)(action_event.message-AEM_QUERY));				

				/* reset the action only if we used it */
				reset_action();
			}
			else if (message_group==AEMG_BUILD_ROOMS && message==AEM_SELL)
			{
				/* rooms */
				room_info_texture->set_ready(false);
				room_data_progress_bar->set_ready(false);
				room_info_panel->set_ready(false);
				build_room_count_label->set_ready(false);
				build_room_cost_label->set_ready(false);

				/* traps and doors */
				trap_door_info_texture->set_ready(false);
				build_trap_door_count_label->set_ready(false);
				build_trap_door_cost_label->set_ready(false);

				/* spells */
				spell_info_texture->set_ready(false);
				spell_cost_label->set_ready(false);
			}
			else if (message_group==AEMG_BUILD_ROOMS)
			{
				room_info_texture->set_ready(true);
				room_info_texture->set_selected(tab_item_class_name[message-AEM_BUILD_ROOM_TREASURE],'I');
				room_data_progress_bar->set_ready(true);
				room_info_panel->set_ready(true);

				build_room_count_label->set_caption("x%d",CV_GAME_MANAGER->getRoomManager()->getRoomCount(CV_CURRENT_PLAYER_ID, CV_GAME_MANAGER->getEconomyManager()->roomTypes[message]));
				build_room_count_label->set_ready(true);

				char *room_class_name = room_class_names[message-AEM_BUILD_ROOM_TREASURE];
				build_room_cost_label->set_caption("%d",GLOBAL_CREATURE_TXT_READER->get_room_propery(room_class_names[message-AEM_BUILD_ROOM_TREASURE],PROPERTY_ROOM_COST));
				build_room_cost_label->set_ready(true);

				action=GA_ROOMS;
				last_built=message;
			}
			else if (message_group==AEMG_EXECUTE_SPELL)
			{
				spell_info_texture->set_ready(true);
				spell_info_texture->set_selected(tab_item_class_name[message-AEM_EXECUTE_SPELL_POSSESS_MONSTER],'S');

				spell_cost_label->set_ready(true);

				action=GA_ROOMS;
				last_built=message;
			}
			else if (message_group==AEMG_BUILD_TRAPS || message_group==AEMG_BUILD_DOORS)
			{
				trap_door_info_texture->set_ready(true);

				GLint item_index = 0;

				if (message_group==AEMG_BUILD_TRAPS)
				{
					item_index = message-AEM_BUILD_TRAP_BOULDER;
					trap_door_info_texture->set_selected(tab_item_class_name[message-AEM_BUILD_TRAP_BOULDER],'T');
					//build_trap_door_count_label->set_caption("x%d",CThingControler::get_item_count((CThingControler::ITEM_TYPE)item_index));
					action=GA_TRAPS;
				}
				else
				{
					item_index = message-AEM_BUILD_DOOR_WOODEN+AEM_BUILD_TRAP_WORD_OF_POWER;
					trap_door_info_texture->set_selected(tab_item_class_name[message-AEM_BUILD_DOOR_WOODEN],'D');
					//build_trap_door_count_label->set_caption("x%d",CThingControler::get_item_count((CThingControler::ITEM_TYPE)item_index));
					action=GA_DOORS;
				}

				build_trap_door_count_label->set_ready(true);
				build_trap_door_cost_label->set_ready(true);
				
				last_built=message;
			}
			else if (message_group==AEMG_LOCATE_ROOM)
			{
				/* do the room locating */
				CV_GAME_MANAGER->getRoomManager()->locateRoom(CV_GAME_MANAGER->getEconomyManager()->roomTypes[message], CV_CURRENT_PLAYER_ID);
			}
			// TODO: trap and door locating, selling boulder.
		}
	}

	ACTION_EVENT *CPlayGUI::get_last_action()
	{
		return &action_event;
	}

	GLvoid CPlayGUI::reset_action()
	{
		action_event.reset();
	}

	CGUI *CPlayGUI::get_active_gui()
	{
		return play_gui;
	}	

	GLvoid CPlayGUI::set_action(bool clicked, bool event, GLint message, GLint message_group, GLvoid *sender)
	{
		action_event.set(clicked,event,message,message_group,sender);
	}

	bool CPlayGUI::is_mouse_over_gui()
	{
		return play_gui->is_mouse_over_gui();
	}

	CDKGUILabel *CPlayGUI::get_room_count_label(GLint player)
	{
		if (player>=0 && player<=3)
		{
			return room_count_label[player];
		}
		else
		{
			return NULL;
		}
	}

	CDKGUILabel *CPlayGUI::get_creature_count_label(GLint player)
	{
		if (player>=0 && player<=3)
		{
			return creature_count_label[player];
		}
		else
		{
			return NULL;
		}
	}

	void CPlayGUI::onKeyDown(int key)
	{
	}

	void CPlayGUI::onKeyUp(int key)
	{
	}
		
	void CPlayGUI::onMouseClicked(int button)
	{
		// Get the last action
		ACTION_EVENT *ae = CV_GAME_MANAGER->getGUIManager()->getLastActionEvent();

		if(button==0)
		{
			// if your not selling (room count label wont be displayed)
			if(ae->message != AEM_SELL)
			{
				if (action==GA_ROOMS)
				{
					build_room_count_label->set_caption("x%d",CV_GAME_MANAGER->getRoomManager()->getRoomCount(CV_CURRENT_PLAYER_ID, CV_GAME_MANAGER->getEconomyManager()->roomTypes[last_built]));
				}
				/*else if (action==GA_TRAPS)
				{
					build_trap_door_count_label->set_caption("x%d",CThingControler::get_item_count((CThingControler::ITEM_TYPE)last_built));
				}
				else if (action==GA_DOORS)
				{
					build_trap_door_count_label->set_caption("x%d",CThingControler::get_item_count((CThingControler::ITEM_TYPE)(last_built+CThingControler::IT_WORD_OF_POWER)));
				}*/
			}
		}
	}

	GLvoid CPlayGUI::set_parent(GLvoid *parent)
	{
		this->parent=parent;
	}

	GLvoid CPlayGUI::updateRoomInfo()
	{
		for (GLint i=0; i<4; i++)
		{
			room_count_label[i]->int_set(CV_GAME_MANAGER->getRoomManager()->getRoomCount(i));
		}
	}

	GLvoid CPlayGUI::updateCreatureInfo()
	{
		for (GLint i=0; i<4; i++)
		{
			creature_count_label[i]->int_set(CV_GAME_MANAGER->getCreatureManager()->getCreatureCount(i));
		}
	}

	GLvoid CPlayGUI::spawnJumpingLabel(GLfloat x, GLfloat y, GLint value)
	{
		jumping_labels.push_back(new CJumpLabel(x,y,value));
	}
}