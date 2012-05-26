#ifndef LIGHTING_OBJECT_H
#define LIGHTING_OBJECT_H

/*
	Into this category fall items like torches, candles, ...
*/

#include "BlockObject.h"

namespace game_objects
{
	namespace block_objects
	{
		class CLightingObject: public CBlockObject
		{
		public:

			enum LIGHTING_OBJECT_TYPE
			{
				LOT_TORCH_LIT = 0,
				LOT_TORCH_UNLIT,
				LOT_CANDLE
			};

			CLightingObject(std::string name, cml::vector3f position, LIGHTING_OBJECT_TYPE lot);
			~CLightingObject();	

		private:

			LIGHTING_OBJECT_TYPE lot;

		};
	};
};

#endif // LIGHTING_OBJECT_H