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
			mouseDown = false;
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
			if(mouseDown)
			{
				CBlock *pickedBlock = CV_GAME_MANAGER->getPickingManager()->getLastPickedBlock();
				if(!pickedBlock)
					goto broke;

				int selected2X = pickedBlock->getLogicalPosition()[0];
				int selected2Y = pickedBlock->getLogicalPosition()[1];

				int right, left, top, bottom;
				left = selected2X<selectedX?selected2X:selectedX;
				right = selected2X>selectedX?selected2X:selectedX;
				top = selected2Y<selectedY?selected2Y:selectedY;
				bottom = selected2Y>selectedY?selected2Y:selectedY;

				ACTION_EVENT *ae = CV_GAME_MANAGER->getGUIManager()->getLastActionEvent();
				if (pickedBlock)
				{				
					GLint group = ae->message_group;
					GLint msg = ae->message;

					for(int x = 0; x<=CV_LEVEL_MAP_SIZE; x++)
					{
						for(int y = 0; y<=CV_LEVEL_MAP_SIZE; y++)
						{
							CBlock *block = CV_GAME_MANAGER->getLevelManager()->getBlock(x,y);
							if(!block) continue;
							if(x>=left&&x<=right&&y>=top&&y<=bottom)
								block->setHighlighted(true);
							else
								block->setHighlighted(false);
						}
					}
				}
			}
			broke:
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

		void CEconomyManager::onMouseReleased(int button)
		{
			CBlock *pickedBlock = CV_GAME_MANAGER->getPickingManager()->getLastPickedBlock();
			int selected2X, selected2Y;
				
			if(!pickedBlock)
			{
				selected2X = -1;
				selected2Y = -1;
			}else
			{
				selected2X = pickedBlock->getLogicalPosition()[0];
				selected2Y = pickedBlock->getLogicalPosition()[1];
			}

			if (button==0)
			{
				mouseDown = false;

				//unhighlight all the blocks except the mouseovered
				for(int x = 0; x<=CV_LEVEL_MAP_SIZE; x++)
				{
					for(int y = 0; y<=CV_LEVEL_MAP_SIZE; y++)
					{
						CBlock *block = CV_GAME_MANAGER->getLevelManager()->getBlock(x,y);
						if(!block) continue;
						if(x==selected2X&&y==selected2Y)
							block->setHighlighted(true);
						else
							block->setHighlighted(false);

					}
				}

				if (CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->is_mouse_over_gui())
				{
					return;
				}

				if(!pickedBlock)
					return;

				int right, left, top, bottom;
				left = selected2X<selectedX?selected2X:selectedX;
				right = selected2X>selectedX?selected2X:selectedX;
				top = selected2Y<selectedY?selected2Y:selectedY;
				bottom = selected2Y>selectedY?selected2Y:selectedY;

				ACTION_EVENT *ae = CV_GAME_MANAGER->getGUIManager()->getLastActionEvent();
				if (pickedBlock)
				{				
					GLint group = ae->message_group;
					GLint msg = ae->message;

					for(int x = left; x<=right; x++)
					{
						for(int y = top; y<=bottom; y++)
						{
							CBlock *block = CV_GAME_MANAGER->getLevelManager()->getBlock(x,y);

							// If you have clicked on a rock, mark it for mining.
							if (CV_GAME_MANAGER->getLevelManager()->isFullBlock(block))
 							{
								if (!block->getType()==CV_BLOCK_TYPE_ROCK_ID)
								{
									block->setMarked(!block->isMarked());
								}
							}
							else if (group==AEMG_BUILD_ROOMS || group==AEMG_BUILD_DOORS || group==AEMG_BUILD_TRAPS)
							{
								// selling comes with the message group AEMG_BUILD_ROOMS
								if (msg==AEM_SELL)
								{						
									// the flag tells us that we have to sell stuff 
									if (block->isSellable(CV_CURRENT_PLAYER_ID))
									{	
										// actual selling procedure
										if (block->getType() == CV_BLOCK_TYPE_CLAIMED_LAND_ID)
										{
											// selling traps
											CV_GAME_MANAGER->getConsole()->writeLine("Trying to sell a trap");

											for (std::vector<CBlockObject*>::iterator rmIter = block->getBlockObjects()->begin(); rmIter != block->getBlockObjects()->end(); rmIter++)
											{
												CBlockObject *bObject = *rmIter;

												if (bObject->getClassName() == "TRAP")
												{
													block->removeBlockObject(bObject);
													continue;
												}
											}
										}
										else
										{
											// selling rooms
											block->setType(CV_BLOCK_TYPE_CLAIMED_LAND_ID);
											block->init();

											// fix the neighbours
											GLint sx = block->getLogicalPosition()[0];
											GLint sy = block->getLogicalPosition()[1];

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
									// the flag tells us that we have to build some stuff (traps, doors, rooms)
									if (!block->isBuildable(CV_CURRENT_PLAYER_ID))
									{
										continue;
									}

									// sanity check: we cant build bridge on dry land ans we cant build rooms on water or lava
									if (roomTypes[msg] != CV_BLOCK_TYPE_BRIDGE_ID && (block->isLava() || block->isWater()))
									{
										// tryint to build room on water or lava
										continue;
									}

									if (roomTypes[msg] == CV_BLOCK_TYPE_BRIDGE_ID && !(block->isLava() || block->isWater()))
									{
										// tryint to build bridge on dry land
										continue;
									}

									block->setType(roomTypes[msg]);
									block->init();

									// fix the neighbours
									GLint sx = block->getLogicalPosition()[0];
									GLint sy = block->getLogicalPosition()[1];

									for (GLint y=sy-1; y<=sy+1; y++)
									{
										for (GLint x=sx-1; x<=sx+1; x++)
										{
											CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->finalize();
										}
									}							
								}
								else if (group==AEMG_BUILD_TRAPS)
								{
									// the flag tells us that we have to build some stuff (traps, doors, rooms)
									if (!block->isBuildable(CV_CURRENT_PLAYER_ID))
									{
										continue;
									}
						
									GLint tt = ae->message - AEM_BUILD_TRAP_BOULDER;

									block->addModel(new CTrap(tt==0?"MODEL_BOULDER":"MODEL_TRAP",block->getRealPosition()+vector3f(CV_BLOCK_WIDTH/2.0f,0.0f,CV_BLOCK_DEPTH/2.0f),(CTrap::TRAP_TYPE)trapTypes[tt]));
								}

								// if we're building a hatchery then we must lower the models by 1/4 of height
								for (std::vector<CBlockObject*>::iterator rmIter = block->getBlockObjects()->begin(); rmIter != block->getBlockObjects()->end(); rmIter++)
								{
									CBlockObject *bObject = *rmIter;

									vector3f bPos = bObject->getPosition();

									if (bPos[1]>=CV_BLOCK_HEIGHT/2.0f)
									{
										// exclude torches
										continue;
									}

									bPos[1] = CV_BLOCK_HEIGHT/4.0f;

									GLint bType = block->getType();

									if (bType == CV_BLOCK_TYPE_HATCHERY_ID)
									{
										bPos[1] = 0.0f;
									}

									bObject->setPosition(bPos);
								}
							}else if(group==AEMG_EXECUTE_SPELL)
							{
								CV_GAME_MANAGER->getSpellManager()->castSpell(ae->message,block->getRealPosition(),CV_CURRENT_PLAYER_ID);
							}
						}
					}
				}
			}
		}
		
		void CEconomyManager::onMouseClicked(int button)
		{
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
				//save the position for the drag start of the drag box
				CBlock *pickedBlock = CV_GAME_MANAGER->getPickingManager()->getLastPickedBlock();

				if(pickedBlock)
				{
					selectedX = pickedBlock->getLogicalPosition()[0];
					selectedY = pickedBlock->getLogicalPosition()[1];
					mouseDown = true;
				}
			}
		}
	};
};