#ifndef HERO_GATE_H
#define HERO_GATE_H

#include "BlockObject.h"

namespace game_objects
{
	namespace block_objects
	{
		class CHeroGate: public CBlockObject
		{
		public:
			CHeroGate(std::string name, cml::vector3f position);
			~CHeroGate();

			loaders::CBR5Model	*getModel();

		private:

			// controls the movement of the perlin noise in the time
			float time;
		};
	};
};

#endif // HERO_GATE_H