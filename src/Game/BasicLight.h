#pragma once

#include "LightSource.h"

/*
	This is the basic light usefull for torches, candles, lava, water, ....
*/

namespace game_objects
{
	namespace lighting
	{
		class CBasicLight: public CLightSource
		{
		public:
			CBasicLight(std::string name);
			CBasicLight(CBasicLight &basicLight);
			~CBasicLight(void);
		};
	};
};