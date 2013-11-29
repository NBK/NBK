#ifndef HATCHERY_H
#define HATCHERY_H

/*
	Representation of all possible goldstacks.
*/

#include "Room.h"
#include "Chicken.h"

namespace game_objects
{
	namespace rooms
	{
		class CHatchery: public CRoom
		{
		public:
			virtual GLvoid update();

			CHatchery();
			~CHatchery();

		private:
			std::vector<game_objects::CCreature*>			chickens;
		};
	};
};

#endif // HATCHERY_H