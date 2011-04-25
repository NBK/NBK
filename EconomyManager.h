#ifndef ECONOMY_MANAGER_H
#define ECONOMY_MANAGER_H

#include "Manager.h"
#include "InputListener.h"

/*
	This class is responsible for the games economy
	managment. Under economy come things like selling
	and buying rooms, money managment, paydays...
*/

namespace game_utils
{
	namespace managers
	{
		class CEconomyManager: public CManager, control::CInputListener
		{
		public:
			CEconomyManager();
			~CEconomyManager();

			// from CManager
			virtual bool init();
			virtual bool update();	
			virtual bool shutdown();

			// from CInputListener
			virtual void onKeyDown(int key);
			virtual void onKeyUp(int key);
			virtual void onMouseClicked(int button);

			static GLint	roomTypes[],
							trapTypes[];
		private:

		};
	};
};

#endif // ECONOMY_MANAGER_H