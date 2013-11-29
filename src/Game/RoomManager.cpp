#include "commons.h"
#include "RoomManager.h"
#include "Block.h"
#include "boundingBox.h"
#include "Treasury.h"

using namespace std;
using namespace cml;
using namespace geometry;
using namespace game_objects;
using namespace game_objects::rooms;

#define RLROOM	"RLROOM"
#define RC		"RC"
#define SRA		"SRA"

namespace game_utils
{
	namespace managers
	{
		CRoomManager::CRoomManager():
		showRoomArea(false)
		{
		}

		CRoomManager::~CRoomManager()
		{
		}

		bool CRoomManager::init()
		{
			// register to console: TODO register some usefull stuff
			CV_GAME_MANAGER->getConsole()->registerClass(this,"ROOM MANAGER");
			CV_GAME_MANAGER->getConsole()->addParam(RLROOM,"() Reloads the rooms.");
			CV_GAME_MANAGER->getConsole()->addParam(RC,"() Prints room count.");
			CV_GAME_MANAGER->getConsole()->addParam(RC,"() Prints room count.");
			CV_GAME_MANAGER->getConsole()->addParam(SRA,"() Shows room area.");

			// construct new rooms
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();
			for (GLuint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLuint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					CBlock *block = lManager->getBlock(x,y);
					if (block->isRoom() && !block->isInRoom())
					{
						if(block->getType() == CV_BLOCK_TYPE_HEART_ID && block->getOwner() == CV_PLAYER_0)
						{
							cml::vector3f position = block->getRealPosition();
							CV_GAME_MANAGER->getControlManager()->getCamera()->setPosition(cml::vector3f(position[0]+0.3, CV_CAMERA_INITIAL_HEIGHT, position[2]+1));
						}
						// we found a room tile that isn't in a room yet. we create a new room.
						CRoom *newRoom = new CRoom();
						newRoom->init(block);
						allRooms[newRoom->getIndex()] = newRoom;
						roomColors[newRoom->getIndex()] = vector3f((GLfloat)(rand()%101)/100.0f,(GLfloat)(rand()%101)/100.0f,(GLfloat)(rand()%101)/100.0f);
					}
				}
			}

			return true;
		}

		bool CRoomManager::checkRoomForSplits(CRoom *room, std::vector< std::vector<CBlock*> > *splits, bool reuseTiles)
		{
			std::vector<CBlock*> *roomTiles = room->getRoomTilesVector();

			GLuint currentRoomTileCount = roomTiles->size();

			room->releaseTiles(false);

			// test tiles
			CRoom testRoom;
			testRoom.init((*roomTiles)[0]);

			if (testRoom.getRoomTilesVector()->size()!=currentRoomTileCount)
			{
				// split needed
				CV_GAME_MANAGER->getConsole()->writeLine("Split needed!");

				// 1. new room is the "testRoom"
				splits->push_back(std::vector<CBlock*>(testRoom.getRoomTilesVector()->begin(),testRoom.getRoomTilesVector()->end()));

				// 2.	the remaining ones create a second room.
				//		but since we can get more that 1 room from selling a tile we must recurse

				for (GLuint i=0; i<testRoom.getRoomTilesVector()->size(); i++)
				{
					room->removeBlock((*testRoom.getRoomTilesVector())[i],false);
				}

				testRoom.releaseTiles(true);
				room->reownTiles();

				return checkRoomForSplits(room,splits,true);
			}
			else
			{
				// no change
				CV_GAME_MANAGER->getConsole()->writeLine("No room change!");

				if (reuseTiles)
				{
					splits->push_back(std::vector<CBlock*>(testRoom.getRoomTilesVector()->begin(),testRoom.getRoomTilesVector()->end()));
				}

				testRoom.releaseTiles(true);
				room->reownTiles();

				return reuseTiles;
			}

			return false;
		}

		bool CRoomManager::update()
		{
			if (showRoomArea)
			{
				drawRoomArea();
			}

			// the reevaluate part
			for (std::vector<GLint>::iterator rIter = roomsToReevaluate.begin(); rIter != roomsToReevaluate.end(); rIter++)
			{
				CRoom *room = allRooms[*rIter];

				if(!room){
					allRooms.erase(allRooms.find(*rIter));
				}else
				// the room could be empty. just delete it
				if (room->getRoomTilesVector()->size()==0)
				{
					allRooms.erase(allRooms.find(*rIter));
				}
				else
				{
					// maybe we have to split the room into n new rooms
					std::vector< std::vector<CBlock*> > splits;

					if (checkRoomForSplits(room,&splits,false))
					{
						// delete this room and create splits.size() new rooms
						allRooms.erase(allRooms.find(room->getIndex()));

						// the new rooms
						for (GLuint r=0; r<splits.size(); r++)
						{
							CRoom *newRoom = new CRoom();

							for (GLuint t=0; t<splits[r].size(); t++)
							{
								newRoom->getRoomTilesVector()->push_back(splits[r][t]);
							}

							newRoom->reownTiles();
							allRooms[newRoom->getIndex()] = newRoom;
							roomColors[newRoom->getIndex()] = vector3f((GLfloat)(rand()%101)/100.0f,(GLfloat)(rand()%101)/100.0f,(GLfloat)(rand()%101)/100.0f);
						}
					}
				}
			}

			roomsToReevaluate.clear();

			CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->updateRoomInfo();


			for (roomIter=allRooms.begin(); roomIter!=allRooms.end(); roomIter++)
			{
				CRoom *room = roomIter->second;
				room->update();
			}
			return true;
		}

		bool CRoomManager::shutdown()
		{
			for (roomIter=allRooms.begin(); roomIter!=allRooms.end(); roomIter++)
			{
				delete roomIter->second;
			}
			allRooms.clear();
			roomColors.clear();
			return true;
		}

		GLvoid CRoomManager::drawRoomArea()
		{
			glBegin(GL_QUADS);
			{
				for (roomIter=allRooms.begin(); roomIter!=allRooms.end(); roomIter++)
				{
					vector3f color = roomColors[roomIter->second->getIndex()];
					glColor3fv(&color[0]);
					for (std::vector<CBlock*>::iterator bIter = roomIter->second->getRoomTilesVector()->begin(); bIter != roomIter->second->getRoomTilesVector()->end(); bIter++)
					{
						sBoundingBox *bbox = (*bIter)->getBoundingBox();

						glVertex3f(bbox->minX,CV_BLOCK_HEIGHT/2.0f,bbox->minZ);
						glVertex3f(bbox->maxX,CV_BLOCK_HEIGHT/2.0f,bbox->minZ);
						glVertex3f(bbox->maxX,CV_BLOCK_HEIGHT/2.0f,bbox->maxZ);
						glVertex3f(bbox->minX,CV_BLOCK_HEIGHT/2.0f,bbox->maxZ);
					}
				}
			}
			glEnd();

			glColor3f(1.0f,1.0f,1.0f);
		}

		CRoom *CRoomManager::getRoom(GLint roomIndex)
		{
			return allRooms[roomIndex];
		}

		GLvoid CRoomManager::reevaluate(GLint roomIndex)
		{
			roomsToReevaluate.push_back(roomIndex);
		}

		GLvoid CRoomManager::addRoomTile(CBlock *block)
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			vector2i pos = block->getLogicalPosition();

			vector2i posses[] = {vector2i(-1,0),vector2i(1,0),vector2i(0,-1),vector2i(0,1)};

			bool nbrs[4];

			nbrs[0] = lManager->isSameTypeAndOwner(pos[0]-1,pos[1],block);
			nbrs[1] = lManager->isSameTypeAndOwner(pos[0]+1,pos[1],block);
			nbrs[2] = lManager->isSameTypeAndOwner(pos[0],pos[1]-1,block);
			nbrs[3] = lManager->isSameTypeAndOwner(pos[0],pos[1]+1,block);

			GLint cnt = 0;

			cnt+=nbrs[0]?1:0;
			cnt+=nbrs[1]?1:0;
			cnt+=nbrs[2]?1:0;
			cnt+=nbrs[3]?1:0;

			if (cnt==0)
			{
				CRoom *newRoom;
				// create a new room
				if(block->getType() == CV_BLOCK_TYPE_TREASURE_ROOM_ID)
					newRoom = new CTreasury();
				else
					newRoom = new CRoom();

				newRoom->getRoomTilesVector()->push_back(block);
				newRoom->reownTiles();
				allRooms[newRoom->getIndex()] = newRoom;
				roomColors[newRoom->getIndex()] = vector3f((GLfloat)(rand()%101)/100.0f,(GLfloat)(rand()%101)/100.0f,(GLfloat)(rand()%101)/100.0f);

				CV_GAME_MANAGER->getConsole()->writeLine("A new room!");
			}
			else
			{
				GLint owner = -1;
				GLint roomIndex = -1;
				GLint type = -1;
				bool set = false;

				bool ok[] = {true, true, true};

				CBlock *testBlock = NULL;

				for (int i=0; i<4; i++)
				{
					if (nbrs[i])
					{
						testBlock = lManager->getBlock(pos+posses[i]);
						if (!set)
						{
							set = true;
							owner = testBlock->getOwner();
							roomIndex = testBlock->getRoomIndex();
							type = testBlock->getType();
						}
						else
						{
							ok[0] &= (owner == testBlock->getOwner());
							ok[1] &= (roomIndex == testBlock->getRoomIndex());
							ok[2] &= (type == testBlock->getType());
						}
					}
				}

				if (ok[0]&&ok[1]&&ok[2])
				{
					// all of same type, owner and room. just add this tile to this existing room
					allRooms[testBlock->getRoomIndex()]->getRoomTilesVector()->push_back(block);
					allRooms[testBlock->getRoomIndex()]->reownTiles();

					CV_GAME_MANAGER->getConsole()->writeLine("Tile added to the existing room.");
				}
				else
				{
					std::map<GLint, CBlock*> blockPerRoom;

					// we must make some merging
					for (int i=0; i<4; i++)
					{
						if (nbrs[i])
						{
							testBlock = lManager->getBlock(pos+posses[i]);
							blockPerRoom[testBlock->getRoomIndex()] = testBlock;
						}
					}

					// on this stage there are at least 2 elements in blockPerRoom.
					// take the first room and add it it all other room tiles.
					// then delete other rooms

					std::map<GLint, CBlock*>::iterator irIter = blockPerRoom.begin();

					CRoom *targetRoom = allRooms[irIter->first];

					irIter++;

					for (irIter; irIter!=blockPerRoom.end(); irIter++)
					{
						CRoom *room = allRooms[irIter->first];

						for (GLuint t=0; t<room->getRoomTilesVector()->size(); t++)
						{
							targetRoom->getRoomTilesVector()->push_back((*room->getRoomTilesVector())[t]);
						}

						// delete the unwanted room
						allRooms.erase(allRooms.find(irIter->first));
					}

					targetRoom->getRoomTilesVector()->push_back(block);

					targetRoom->reownTiles();

					CV_GAME_MANAGER->getConsole()->writeLine("Rooms merged.");
				}
			}
		}

		GLint CRoomManager::getRoomCount(GLint owner)
		{
			GLint count = 0;

			for (roomIter=allRooms.begin(); roomIter!=allRooms.end(); roomIter++)
			{
				CRoom *room = roomIter->second;

				// extract owner info the first tile. this is safe since no room can have 0 tiles.
				CBlock *roomTile = (*room->getRoomTilesVector())[0];
				count+=(roomTile->getOwner() == owner && !(roomTile->getType() == CV_BLOCK_TYPE_HEART_ID || roomTile->getType() == CV_BLOCK_TYPE_PORTAL_ID))?1:0;
			}

			return count;
		}

		GLint CRoomManager::getRoomCount(GLint owner, GLint type)
		{
			GLint count = 0;

			for (roomIter=allRooms.begin(); roomIter!=allRooms.end(); roomIter++)
			{
				CRoom *room = roomIter->second;

				// extract owner info the first tile. this is safe since no room can have 0 tiles.
				CBlock *roomTile = (*room->getRoomTilesVector())[0];
				count+=(roomTile->getOwner() == owner && roomTile->getType() == type && !(roomTile->getType() == CV_BLOCK_TYPE_HEART_ID || roomTile->getType() == CV_BLOCK_TYPE_PORTAL_ID))?1:0;
			}

			return count;
		}

		CBlock *CRoomManager::getRoom(GLint roomType, GLubyte owner)
		{
			for (roomIter=allRooms.begin(); roomIter!=allRooms.end(); roomIter++)
			{
				CRoom *room = roomIter->second;
				CBlock *roomTile = (*room->getRoomTilesVector())[0];
				if (roomTile->getType() == roomType && roomTile->getOwner() == owner)
				{
					return roomTile;
				}
			}
			return NULL;
		}

		GLvoid CRoomManager::locateRoom(GLint roomType, GLubyte owner)
		{
				CBlock *roomTile = getRoom(roomType, owner);
				if(roomTile)
				{
					cml::vector3f currPos = roomTile->getRealPosition();
					CV_GAME_MANAGER->getControlManager()->getCamera()->setPosition(cml::vector3f(currPos[0], CV_CAMERA_INITIAL_HEIGHT, currPos[2]+1));
				}
		}

		string CRoomManager::onAction(string keyword, string params)
		{
			if (keyword==RLROOM)
			{
				this->shutdown();
				this->init();
				return "Rooms reloaded!";
			}
			else if (keyword==RC)
			{
				return "Rooms count: "+CConversions::intToStr(allRooms.size());
			}
			else if (keyword==SRA)
			{
				showRoomArea=!showRoomArea;
				return "Room area "+string(showRoomArea?"enabled.":"disabled");
			}
			return "<>";
		}
	};
};