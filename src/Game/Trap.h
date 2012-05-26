#ifndef TRAP_H
#define TRAP_H

/*
	Representation of all possible traps.
	Boulder traps must be added the the entity manager.
*/

#include "BlockObject.h"

namespace game_objects
{
	namespace block_objects
	{
		class CTrap: public CBlockObject
		{
		public:

			enum TRAP_TYPE
			{
				TT_BOULDER = 0,
				TT_ALARM,
				TT_POISON,
				TT_LIGHTNING,
				TT_WORD_OF_POWER,
				TT_LAVA
			};

			CTrap(std::string name, cml::vector3f position, TRAP_TYPE trapType);
			~CTrap();

		private:

			TRAP_TYPE trapType;		
		};
	};
};

#endif // TRAP_H