#ifndef PICKING_MANAGER_H
#define PICKING_MANAGER_H

#include "Manager.h"
#include "Block.h"

/*
	Handles the picking of every possible thing. That includes
	creatures, heroes, pickable objects (gold, powerups...),
	terrain.
*/

namespace game_utils
{
	namespace managers
	{
		class CPickingManager: public CManager
		{
		public:
			CPickingManager();
			virtual ~CPickingManager();

			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			game_objects::CBlock *getLastPickedBlock();

		private:
			game_objects::CBlock *lastPickedBlock;
		};
	};
};

#endif // PICKING_MANAGER_H