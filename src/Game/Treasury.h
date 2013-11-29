#ifndef TREASURY_H
#define TREASURY_H

/*
	Representation of all possible goldstacks.
*/

#include "Room.h"

namespace game_objects
{
	namespace rooms
	{
		class CTreasury: public CRoom
		{
		public:
			virtual GLvoid update();

			CTreasury();
			~CTreasury();

		private:

		};
	};
};

#endif // TREASURY_H