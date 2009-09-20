#include "commons.h"
#include "Room.h"
#include "LevelManager.h"
#include <algorithm>

using namespace std;
using namespace cml;
using namespace game_utils;
using namespace game_utils::managers;

namespace game_objects
{
	namespace rooms
	{
		GLint CRoom::allRoomIndices = 0;

		CRoom::CRoom()
		{
			roomIndex = allRoomIndices++;
		}

		CRoom::~CRoom()
		{
			// reset tiles
			releaseTiles(true);
		}

		GLvoid CRoom::recalcBBOX()
		{
			boundingBox.reset();
			for (rtIter=roomTiles.begin(); rtIter!=roomTiles.end(); rtIter++)
			{
				boundingBox.update((*rtIter)->getRealPosition());
				boundingBox.update((*rtIter)->getRealPosition()+vector3f(CV_BLOCK_WIDTH,0.0f,CV_BLOCK_DEPTH));
			}
			boundingBox.calculateExtents();
			boundingBox.calculateVertices();
		}

		GLvoid CRoom::removeBlock(CBlock *block, bool reevaluate)
		{
			block->setRoomIndex(-1);

			rtIter = find(roomTiles.begin(),roomTiles.end(),block);

			if (rtIter!=roomTiles.end())
			{
				// this should allways work!
				roomTiles.erase(rtIter);
			}

			if (reevaluate)
			{
				CV_GAME_MANAGER->getRoomManager()->reevaluate(roomIndex);
				recalcBBOX();
			}
		}

		GLvoid CRoom::rec(vector2i pos)
		{
			// check pos validity
			if (pos[0]<0 || pos[1]<0 || pos[0]>=CV_LEVEL_MAP_SIZE || pos[1]>=CV_LEVEL_MAP_SIZE)
			{
				return;
			}

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();
			CBlock *baseBlock = roomTiles.size()>0?*roomTiles.begin():NULL;
			CBlock *targetBlock = lManager->getBlock(pos);

			// check if we are of same type and owner as last block			
			if (baseBlock && !lManager->isSameTypeAndOwner(pos[0],pos[1],baseBlock))
			{
				return;
			}

			// check if we have already been here
			if (targetBlock->isInRoom())
			{
				return;
			}

			// we have a valid block to add to out room
			targetBlock->setRoomIndex(roomIndex);
			roomTiles.push_back(targetBlock);

			// search in all directions
			rec(pos+vector2i(-1,0));	// L
			rec(pos+vector2i(+1,0));	// R
			rec(pos+vector2i(0,-1));	// U
			rec(pos+vector2i(0,+1));	// D
		}

		GLvoid CRoom::init(CBlock *baseBlock)
		{
			releaseTiles(true);
			rec(baseBlock->getLogicalPosition());
			recalcBBOX();
		}

		std::vector<CBlock*> *CRoom::getRoomTilesVector()
		{
			return &roomTiles;
		}

		GLint CRoom::getIndex()
		{
			return roomIndex;
		}

		GLvoid CRoom::releaseTiles(bool clearReferences)
		{
			for (rtIter=roomTiles.begin(); rtIter!=roomTiles.end(); rtIter++)
			{
				(*rtIter)->setRoomIndex(-1);
			}

			if (clearReferences)
			{
				roomTiles.clear();
			}
		}

		GLvoid CRoom::reownTiles()
		{
			for (rtIter=roomTiles.begin(); rtIter!=roomTiles.end(); rtIter++)
			{
				(*rtIter)->setRoomIndex(roomIndex);
			}
			recalcBBOX();
		}
	};
};