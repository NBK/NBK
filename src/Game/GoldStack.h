#ifndef GOLD_STACK_H
#define GOLD_STACK_H

/*
	Representation of all possible goldstacks.
*/

#include "BlockObject.h"

namespace game_objects
{
	namespace block_objects
	{
		class CGoldStack: public CBlockObject
		{
		public:

			enum GOLD_STACK_TYPE
			{
				TT_GOLD_250 = 0,
				TT_GOLD_500
			};

			CGoldStack(cml::vector3f position, GOLD_STACK_TYPE goldstackType);
			GLvoid setGold(GLint amount);
			~CGoldStack();

		private:

			GOLD_STACK_TYPE goldstackType;		
			GLint gold;
		};
	};
};

#endif // GOLD_STACK_H