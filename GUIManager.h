#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#include "Manager.h"
#include "GUI/PlayGUI.h"

/*
	Wrapper around the original NBK CPlayGUI
*/

namespace game_utils
{
	namespace managers
	{
		class CGUIManager: public CManager
		{
		public:
			CGUIManager();
			~CGUIManager();

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			DK_GUI::CPlayGUI	*getPlayGUI();

			// should be called from different managers that need to know what happened in the GUI
			ACTION_EVENT *getLastActionEvent();

		private:
			DK_GUI::CPlayGUI	*playGUI;

			// contains the last event
			ACTION_EVENT last_action_event;
		};
	};
};

#endif // GUI_MANAGER_H