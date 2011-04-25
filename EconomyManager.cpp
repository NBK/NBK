#include "commons.h"
#include "EconomyManager.h"
#include "Block.h"
#include "BlockObject.h"
#include "Trap.h"

using namespace control;
using namespace cml;
using namespace game_objects;
using namespace game_objects::block_objects;

namespace game_utils
{
	namespace managers
	{
		GLint CEconomyManager::roomTypes[]=
		{
			CV_BLOCK_TYPE_TREASURE_ROOM_ID,
			CV_BLOCK_TYPE_LAIR_ID,
			CV_BLOCK_TYPE_HATCHERY_ID,
			CV_BLOCK_TYPE_TRAINING_ROOM_ID,
			CV_BLOCK_TYPE_LIBRARY_ID,
			CV_BLOCK_TYPE_BRIDGE_ID,
			CV_BLOCK_TYPE_GUARD_POST_ID,
			CV_BLOCK_TYPE_WORKSHOP_ID,
			CV_BLOCK_TYPE_PRISON_ID,
			CV_BLOCK_TYPE_TORTURE_CHAMBER_ID,
			CV_BLOCK_TYPE_BARRACKS_ID,
			CV_BLOCK_TYPE_TEMPLE_ID,
			CV_BLOCK_TYPE_GRAVEYARD_ID
		};

		GLint CEconomyManager::trapTypes[]= 
		{
			CTrap::TT_BOULDER,
			CTrap::TT_ALARM,
			CTrap::TT_POISON,
			CTrap::TT_LIGHTNING,
			CTrap::TT_WORD_OF_POWER,
			CTrap::TT_LAVA
		};

		CEconomyManager::CEconomyManager(): CManager(), CInputListener()
		{			
			// register to handle input
			CV_GAME_MANAGER->getControlManager()->getInput()->registerListener(this);
		}

		CEconomyManager::~CEconomyManager()
		{
		}

		bool CEconomyManager::init()
		{
			return true;
		}

		bool CEconomyManager::update()
		{
			return true;
		}

		bool CEconomyManager::shutdown()
		{
			return true;
		}

		void CEconomyManager::onKeyDown(int key)
		{
		}

		void CEconomyManager::onKeyUp(int key)
		{
		}
		
		void CEconomyManager::onMouseClicked(int button)
		{
			CBlock *pickedBlock = CV_GAME_MANAGER->getPickingManager()->getLastPickedBlock();

			ACTION_EVENT *ae = CV_GAME_MANAGER->getGUIManager()->getLastActionEvent();

			if (CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->is_mouse_over_gui())
			{
				return;
			}

			if (button==1)
			{
				// reset
				ae->reset();
			}

			if (button==0)
			{
				if (pickedBlock)
				{				
					GLint group = ae->message_group;
					GLint msg = ae->message;

					vector2i mousePos = CV_GAME_MANAGER->getControlManager()->getInput()->getMousePos();

					if(group==AEMG_EXECUTE_SPELL)
					{
						CV_GAME_MANAGER->getSpellManager()->castSpell(ae->message,pickedBlock->getLogicalPosition(),CV_CURRENT_PLAYER_ID);
						return;
					}
					// If you have clicked on a rock, mark it for mining.
					if (CV_GAME_MANAGER->getLevelManager()->isFullBlock(pickedBlock))
 					{
						if (!pickedBlock->getType()==CV_BLOCK_TYPE_ROCK_ID)
							pickedBlock->setMarked(!pickedBlock->isMarked());
					}
					else if (group==AEMG_BUILD_ROOMS || group==AEMG_BUILD_DOORS || group==AEMG_BUILD_TRAPS)
					{
						// selling comes with the message group AEMG_BUILD_ROOMS
						if (msg==AEM_SELL)
						{						
							// the flag tells us that we have to sell stuff 
							if (pickedBlock->isSellable(CV_CURRENT_PLAYER_ID))
							{	
								// actual selling procedure
								if (pickedBlock->getType() == CV_BLOCK_TYPE_CLAIMED_LAND_ID)
								{
									// selling traps
									CV_GAME_MANAGER->getConsole()->writeLine("Trying to sell a trap");

									for (std::vector<CBlockObject*>::iterator rmIter = pickedBlock->getBlockObjects()->begin(); rmIter != pickedBlock->getBlockObjects()->end(); rmIter++)
									{
										CBlockObject *bObject = *rmIter;

										if (bObject->getClassName() == "TRAP")
										{
											pickedBlock->removeBlockObject(bObject);
											break;
										}
									}
								}
								else
								{
									// generate the room cost (divide by 2 because were selling)
									int k, position;
									bool found = false;

									// find target's position if present
									k = 0;
									while (k < sizeof(roomTypes) && !found)
									{
										if (roomTypes[k] == pickedBlock->getType())
										{
											position = k;
										found = true;
										}
										else
											k++;
									}
									GLint room_cost = GLOBAL_CREATURE_TXT_READER->get_room_propery(room_class_names[position],PROPERTY_ROOM_COST)/2;
											
									// we built somethig so we have to lower the money
									PLAYER0_MONEY+=room_cost;

									// spawn the jumping label
									CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->spawnJumpingLabel(mousePos[0],mousePos[1],room_cost);

									// selling rooms
									pickedBlock->setType(CV_BLOCK_TYPE_CLAIMED_LAND_ID);
									pickedBlock->init();

									// fix the neighbours
									GLint sx = pickedBlock->getLogicalPosition()[0];
									GLint sy = pickedBlock->getLogicalPosition()[1];

									for (GLint y=sy-1; y<=sy+1; y++)
									{
										for (GLint x=sx-1; x<=sx+1; x++)
										{
											CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->finalize();
										}
									}
								}
							}
						}
						else if (group==AEMG_BUILD_ROOMS)
						{
							GLint room_cost = GLOBAL_CREATURE_TXT_READER->get_room_propery(room_class_names[msg],PROPERTY_ROOM_COST);
						
							if(PLAYER0_MONEY-room_cost>=0)
							{
								// the flag tells us that we have to build some stuff (traps, doors, rooms)
								if (!pickedBlock->isBuildable(CV_CURRENT_PLAYER_ID))
								{
									return;
								}

								// sanity check: we cant build bridge on dry land ans we cant build rooms on water or lava
								if (roomTypes[msg] != CV_BLOCK_TYPE_BRIDGE_ID && (pickedBlock->isLava() || pickedBlock->isWater()))
								{
									// tryint to build room on water or lava
									return;
								}

								if (roomTypes[msg] == CV_BLOCK_TYPE_BRIDGE_ID && !(pickedBlock->isLava() || pickedBlock->isWater()))
								{
									// tryint to build bridge on dry land
									return;
								}

								// we built somethig so we have to lower the money
								PLAYER0_MONEY-=room_cost;

								// spawn the jumping label
								CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->spawnJumpingLabel(mousePos[0],mousePos[1],room_cost);

								pickedBlock->setType(roomTypes[msg]);
								pickedBlock->init();

								// fix the neighbours
								GLint sx = pickedBlock->getLogicalPosition()[0];
								GLint sy = pickedBlock->getLogicalPosition()[1];

								for (GLint y=sy-1; y<=sy+1; y++)
								{
									for (GLint x=sx-1; x<=sx+1; x++)
									{
										CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->finalize();
									}
								}	
							}
						}
						else if (group==AEMG_BUILD_TRAPS)
						{
							// the flag tells us that we have to build some stuff (traps, doors, rooms)
							if (!pickedBlock->isBuildable(CV_CURRENT_PLAYER_ID))
							{
								return;
							}
						
							GLint tt = ae->message - AEM_BUILD_TRAP_BOULDER;

							pickedBlock->addModel(new CTrap(tt==0?"MODEL_BOULDER":"MODEL_TRAP",pickedBlock->getRealPosition()+vector3f(CV_BLOCK_WIDTH/2.0f,0.0f,CV_BLOCK_DEPTH/2.0f),(CTrap::TRAP_TYPE)trapTypes[tt]));
						}

						// if we're building a hatchery then we must lower the models by 1/4 of height
						for (std::vector<CBlockObject*>::iterator rmIter = pickedBlock->getBlockObjects()->begin(); rmIter != pickedBlock->getBlockObjects()->end(); rmIter++)
						{
							CBlockObject *bObject = *rmIter;

							vector3f bPos = bObject->getPosition();

							if (bPos[1]>=CV_BLOCK_HEIGHT/2.0f)
							{
								// exclude torches
								continue;
							}

							bPos[1] = CV_BLOCK_HEIGHT/4.0f;

							GLint bType = pickedBlock->getType();

							if (bType == CV_BLOCK_TYPE_HATCHERY_ID)
							{
								bPos[1] = 0.0f;
							}

							bObject->setPosition(bPos);
						}

					}
				}
			}
		}
	};
};