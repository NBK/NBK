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
			lastSpawn = timeGetTime();
		}

		GLvoid CHatchery::update()
		{
			GLuint currentChickenCount = chickens.size();
			if(currentChickenCount < this->getRoomTilesVector()->size() && (timeGetTime() > (lastSpawn+20000+20000*currentChickenCount)))
			{
				for (std::vector<CBlock*>::iterator rmIter = this->getRoomTilesVector()->begin(); rmIter != this->getRoomTilesVector()->end(); rmIter++)
				{
					CBlock *thisBlock = *rmIter; 
					CV_GAME_MANAGER->getCreatureManager()->addCreature("CHICKEN",thisBlock->getCenterPosition(),thisBlock->getOwner());
					CCreatureManager *cManager = CV_GAME_MANAGER->getCreatureManager();
					chickens.push_back(cManager->getCreatureVector()->back());
					lastSpawn = (float)timeGetTime();
					break;
				}
			}
		}

	}
}