#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "Manager.h"
#include "ConsoleListener.h"
#include "Room.h"
#include "Block.h"

namespace game_utils
{
	namespace managers
	{
		class CRoomManager: public CManager, public control::CConsoleListener
		{
		public:
			CRoomManager();
			~CRoomManager();

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

			game_objects::rooms::CRoom *getRoom(GLint roomIndex);

			/* 
				If something has happened to the room, this room calls this function.
				This usually happens when a tile sends a remove demand to the room.
				Maybe we have to delete this room. Maybe we have to split it into 
				several new rooms. Merging takes place elsewhere. The reevaluation
				takes place in the update part.
			*/
			GLvoid reevaluate(GLint roomIndex);

			/* 
				Called when a new block of room type is constructed. Sevelar
				things may accure at this moment:
					- this tile is on its own and creates a new room
					- this tile is added to an existing room
					- this tile connects two rooms and starts the merging operation
			*/
			GLvoid addRoomTile(game_objects::CBlock *block);

			/*
				Returns the room number for selected player;
			*/
			GLint getRoomCount(GLint owner);

			game_objects::CBlock *getRoom(GLint roomType, GLubyte owner);

			GLvoid CRoomManager::locateRoom(GLint roomType, GLubyte owner);
		private:

			// contains all the rooms in the game
			std::map<GLint, game_objects::rooms::CRoom*>			allRooms;
			std::map<GLint, game_objects::rooms::CRoom*>::iterator	roomIter;
			std::map<GLint, cml::vector3f>							roomColors;

			std::vector<GLint>	roomsToReevaluate;

			// draws simple quads where the room area is
			GLvoid drawRoomArea();

			/* 
				Checks the room for possible splits as a result of the tile selling.
				Returns true if we have a split and the properly split tiles int the vector.
			*/
			bool checkRoomForSplits(game_objects::rooms::CRoom *room, std::vector< std::vector<game_objects::CBlock*> > *splits, bool reuseTiles);

			bool showRoomArea;
		};
	};
};

#endif // ROOM_MANAGER_H