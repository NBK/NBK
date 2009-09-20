#include "commons.h"
#include "GUIManager.h"

using namespace DK_GUI;

namespace game_utils
{
	namespace managers
	{
		CGUIManager::CGUIManager(): CManager()
		{
			playGUI = NULL;
		}

		CGUIManager::~CGUIManager()
		{
		}

		bool CGUIManager::init()
		{
			playGUI = new CPlayGUI(CV_SETTINGS_WINDOW_WIDTH,CV_SETTINGS_WINDOW_HEIGHT,GLOBAL_TEXTURE_LIST);
			return true;
		}

		bool CGUIManager::update()
		{
			if (!CV_GAME_MANAGER->getControlManager()->isFPS())
			{
				playGUI->draw_and_do_actions();		

				// get the last action that happened in the gui 
				ACTION_EVENT *action=playGUI->get_last_action();

				// if the event flag is true we have the action 
				if (action->event)
				{
					// if there was a click we check what do we have to do 
					if (action->click)
					{	
						// save the last action for further use 
						last_action_event.set(action);					
					}

					// now we reset the event, this is a MUST 
					playGUI->reset_action();
				}
			}
			return true;
		}

		bool CGUIManager::shutdown()
		{
			delete playGUI;

			return true;
		}

		CPlayGUI *CGUIManager::getPlayGUI()
		{
			return playGUI;
		}

		ACTION_EVENT *CGUIManager::getLastActionEvent()
		{
			return &last_action_event;
		}
	};
};