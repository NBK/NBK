#include "commons.h"
#include "LightingObject.h"

using namespace std;
using namespace cml;
using namespace loaders;
using namespace game_utils;

namespace game_objects
{
	namespace block_objects
	{
		CLightingObject::CLightingObject(string name, vector3f position, LIGHTING_OBJECT_TYPE lot)
			: CBlockObject(name,position,NULL,0.0f)
		{
			this->lot = lot;

			setModel(CV_GAME_MANAGER->getResourceManager()->getModel(name));

			// load effect according to the type
			if (lot == LOT_CANDLE)
			{
				effectName = "EFFECTS_CANDLE";
				setEffect(CV_GAME_MANAGER->getResourceManager()->getEffect(effectName));
			}
			else if (lot == LOT_TORCH_LIT)
			{
				effectName = "EFFECTS_TORCH";
				setEffect(CV_GAME_MANAGER->getResourceManager()->getEffect(effectName));
			}
		}

		CLightingObject::~CLightingObject()
		{
		}
	};
};