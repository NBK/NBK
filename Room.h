#ifndef ROOM_H
#define	ROOM_H

#include "Block.h"
#include <vector>
#include <cml/cml.h>

namespace game_objects
{
	namespace rooms
	{
		class CRoom
		{
		public:
			CRoom();
			~CRoom();

			/*
				Recursively runs through the DK map and constructs a room.
			*/
			GLvoid init(CBlock *baseBlock);

			// returns a reference to room tiles
			std::vector<CBlock*> *getRoomTilesVector();

			// removes a specified block as a result of eg. selling the room block
			GLvoid removeBlock(CBlock *block, bool reevaluate);

			// called by the room manager to properly position the room
			GLint getIndex();

			// releases all tiles that constructed it
			GLvoid releaseTiles(bool clearReferences);

			// if we released tiles but not cleared references we can reown them
			GLvoid reownTiles();

		private:

			// hold references to all room tiles
			std::vector<CBlock*>			roomTiles;
			std::vector<CBlock*>::iterator	rtIter;

			// room bounding box. if camera is in the bbox we have to draw the room effect.
			geometry::sBoundingBox	boundingBox;

			GLvoid rec(cml::vector2i pos);

			// the index of this room, gets create in the room manager
			GLint roomIndex;

			static GLint allRoomIndices;

			GLvoid recalcBBOX();
		};
	};
};

#endif // ROOM_H