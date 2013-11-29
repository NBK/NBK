#include "commons.h"
#include "Hatchery.h"

using namespace cml;
using namespace game_utils;
using namespace managers;

namespace game_objects
{
	namespace rooms
	{
		CHatchery::CHatchery()
			: CRoom()
		{
		}

		CHatchery::~CHatchery()
		{
			chickens.clear();
		}

		GLvoid CHatchery::update()
		{
			GLuint currentChickenCount = chickens.size();
			if(currentChickenCount < 4)
			{
				for (std::vector<CBlock*>::iterator rmIter = this->getRoomTilesVector()->begin(); rmIter != this->getRoomTilesVector()->end(); rmIter++)
				{
					CBlock *thisBlock = *rmIter; 
					CV_GAME_MANAGER->getCreatureManager()->addCreature("CHICKEN",thisBlock->getRealPosition(),thisBlock->getOwner());
					CCreatureManager *cManager = CV_GAME_MANAGER->getCreatureManager();
					chickens.push_back(cManager->getCreatureVector()->back());
				}
			}
		}

	}
}