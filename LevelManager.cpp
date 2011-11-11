#include "settings.h"
#include "commons.h"
#include "LevelManager.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include "Trap.h"
#include "HeroGate.h"
#include "LightingObject.h"

#include "BasicLight.h"

using namespace std;
using namespace game_utils;
using namespace game_objects;
using namespace game_objects::block_objects;
using namespace cml;
using namespace utils;
using namespace control;

#define LDLVL	"LDLVL"
#define TDEF	"TDEF"
#define PLOG	"PLOG"
#define SAVE	"SAVE"

// things
#define HERO_GATE		52
#define LEVEL_TEXTURE	4
#define TEXTURE_ATLAS			0

#define TNG_ITEM_DECORATION		1

#define TNG_BONUS_RESSURECT_CREATURE	87
#define TNG_BONUS_INCREASE_LEVEL		91
#define TNG_BONUS_REVEAL_MAP			86
#define TNG_BONUS_TRANSFER_CREATURE		88
#define TNG_BONUS_STEAL_HERO			89
#define TNG_BONUS_MULTIPLY_CREATURES		90
#define TNG_BONUS_MAKE_SAFE				99

#define TNG_SPELL_HAND_OF_EVIL		11
#define TNG_SPELL_CREATE_IMP		12
#define	TNG_SPELL_SIGHT_OF_EVIL		15
#define	TNG_SPELL_CAVE_IN			17
#define	TNG_SPELL_HEAL_CREATURE		18
#define	TNG_SPELL_HOLD_AUDIENCE		19
#define TNG_SPELL_LIGHTNING			20
#define TNG_SPELL_SPEED_CREATURE	21
#define TNG_SPELL_PROTECT_CREATURE	22
#define TNG_SPELL_CONCEAL_CREATURE	23
#define TNG_SPELL_DISEASE			45
#define TNG_SPELL_DESTROY_WALLS		47
#define TNG_SPELL_ARMAGEDDON		134

#define TNG_BOX_BOULDER				94
#define TNG_BOX_ALARM				95
#define TNG_BOX_POISON				96
#define TNG_BOX_LIGHTNING			97
#define TNG_BOX_WORD_OF_POWER		98
#define TNG_BOX_LAVA				99

#define TNG_STATUE1					8
#define TNG_STATUE2					30
#define TNG_STATUE_UNLIT			129

#define TNG_KEY						44
#define TNG_GOLD_250				6
#define TNG_GOLD_500				3
#define TNG_HERO_GATE				49
#define TNG_BARREL					1
#define TNG_CANDLE_ON_STICK			28
#define TNG_PINK_VASE				34
#define TNG_BLUE_VASE				35
#define TNG_GREEN_VASE				36

#define TNG_LIT_TORCH				2
#define TNG_UNLIT_TORCH				7

// traps
#define TNG_TRAP					8

#define TNG_TRAP_BOULDER			1
#define TNG_TRAP_ALARM				2
#define TNG_TRAP_POISON				3
#define TNG_TRAP_LIGHTNING			4
#define TNG_TRAP_WORD_OF_POWER		5
#define TNG_TRAP_LAVA				6

// creatures
#define TNG_CREATURE				5

#define TNG_CREATURE_BARBARIAN		2
#define TNG_CREATURE_KNIGHT			6
#define TNG_CREATURE_HORNED_REAPER	14
#define TNG_CREATURE_DRAGON			17
#define TNG_CREATURE_DEMON_SPAWN	18
#define TNG_CREATURE_FLY			19
#define TNG_CREATURE_BILE_DEMON		22
#define TNG_CREATURE_IMP			23
#define TNG_CREATURE_BEETLE			24
#define TNG_CREATURE_VAMPIRE		25
#define TNG_CREATURE_SPIDER			26
#define TNG_CREATURE_HELL_HOUND		27

// room effects
#define TNG_ROOM_EFFECT				7
#define TNG_ROOM_EFFECT_WATER		2
#define TNG_ROOM_EFFECT_DRY_ICE		4

namespace game_utils
{
	namespace managers
	{
		CLevelManager::CLevelManager(): CManager(), CConsoleListener()
		{
			currentLevel=0;
		}

		CLevelManager::~CLevelManager()
		{
		}

		bool CLevelManager::levelExists(string levelFileName)
		{
			FILE *levelFile;

			bool status = true;

			status&=(levelFile=fopen((levelFileName+".own").c_str(),"rb"))!=NULL;
			status&=(levelFile=fopen((levelFileName+".slb").c_str(),"rb"))!=NULL;
			status&=(levelFile=fopen((levelFileName+".tng").c_str(),"rb"))!=NULL;
			
			return status;
		}

		bool CLevelManager::init()
		{
			CLogger::setEntryStart();

			// Initialization: create the array of blocks.
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					levelMap[y][x] = new CBlock();
					levelMap[y][x]->setLogicalPosition(vector2i(x,y));
				}
			}
			CLogger::setEntryEnd("\tCreating a map ob blocks 85x85 new blocks.");

			string fileName = "";

			// Load level list from file.
			if (levelFileName=="")
			{
				levelFileNames.clear();
				ifstream iFile;
				iFile.open((CV_RESOURCES_DIRECTORY+CV_CONFIG_LEVELS).c_str());

				if (!iFile)
				{
					return false;
				}

				string line="";

				while (!iFile.eof())
				{
					getline (iFile,line);

					if (line.length()!=0 && line.at(0)!='#')
					{
						levelFileNames.push_back(CV_RESOURCES_DIRECTORY+line);
					}
				}	

				iFile.close();

				// Load level from files.
				fileName = levelFileNames[currentLevel];
			}
			else
			{
				fileName = levelFileName;
			}

			bool result = true;
			result&=loadSLB(fileName+".slb");
			result&=loadOWN(fileName+".own");
			result&=loadTNG(fileName+".tng");

			CLogger::setEntryStart();
			// After everything has been properly loaded call INIT on all blocks.
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					levelMap[y][x]->init();
				}
			}
			CLogger::setEntryEnd("\tInitializing 85x85 blocks.");

			CLogger::setEntryStart();
			// MUST: calculate height of the ceiling for every block
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					if (levelMap[y][x]->isLow())
					{
						calculateBlockCeilingHeight(levelMap[y][x]);
					}
				}
			}
			CLogger::setEntryEnd("\tCalculating ceiling 85x85 blocks.");

			CLogger::setEntryStart();

			// And only now can we call finalize.
			CLightingManager *liManager = CV_GAME_MANAGER->getLightingManager();
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					levelMap[y][x]->finalize();

					// setup light info
					if (levelMap[y][x]->hasTorch())
					{
						liManager->addLightSource(levelMap[y][x]->getLogicalPosition(), CV_LIGHT_TYPE_TORCH);
					}
					
					for (std::vector<CBlockObject*>::iterator rmIter = levelMap[y][x]->getBlockObjects()->begin(); rmIter != levelMap[y][x]->getBlockObjects()->end(); rmIter++)
					{
						CBlockObject *bObject = *rmIter;

						if (bObject->getEffectName() == "EFFECTS_CANDLE")
						{
							liManager->addLightSource(levelMap[y][x]->getLogicalPosition(), CV_LIGHT_TYPE_CANDLE);
							break;
						}
					}

					if (levelMap[y][x]->getType()==CV_BLOCK_TYPE_LAVA_ID && rand()%3==0)
					{
						liManager->addLightSource(levelMap[y][x]->getLogicalPosition(), CV_LIGHT_TYPE_LAVA);
					}
					else if (levelMap[y][x]->getType()==CV_BLOCK_TYPE_WATER_ID  && rand()%3==0)
					{
						liManager->addLightSource(levelMap[y][x]->getLogicalPosition(), CV_LIGHT_TYPE_WATER);
					}
				}
			}

			//liManager->addLightSource(levelMap[3][3]->getLogicalPosition(), LIGHT_TYPE_TORCH);

			CLogger::setEntryEnd("\tFinalizing 85x85 blocks.");

			CV_GAME_MANAGER->getConsole()->registerClass(this,"LEVEL MANAGER");
			CV_GAME_MANAGER->getConsole()->addParam(LDLVL,"(xxxx) Loads specified level. Level must exist in data/resources/levels.");
			CV_GAME_MANAGER->getConsole()->addParam(TDEF,"() Toggles the terrain deformations."); // TODO
			CV_GAME_MANAGER->getConsole()->addParam(PLOG,"() Prints the contents of the log file.");
			CV_GAME_MANAGER->getConsole()->addParam(SAVE,"(xxxx) Saves the game. Level will be saved in data/resources/saves.");

			return result;
		}

		bool CLevelManager::update()
		{
			return true;
		}

		bool CLevelManager::shutdown()
		{
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					delete levelMap[y][x];
				}
			}
			return true;
		}

		bool CLevelManager::loadSLB(string fileName)
		{
			struct sSLB
			{
				GLbyte typeID;	// ID of the block
				GLbyte empty;	// empty not used reserved value
			};

			// open file and read data
			FILE *inSLB;

			if (!(inSLB=fopen(fileName.c_str(),"rb")))
			{
				return false;
			}

			sSLB slb[CV_LEVEL_MAP_SIZE][CV_LEVEL_MAP_SIZE];

			for (GLuint i=0; i<CV_LEVEL_MAP_SIZE; i++)
			{
				if (fread(slb[i],CV_LEVEL_MAP_SIZE,sizeof(sSLB),inSLB)==0)
				{
					fclose(inSLB);
					return FALSE;
				}
			}

			fclose(inSLB);			

			unclaimedBlocksList.clear();
			unfortifiedBlocksList.clear();

			// process the data
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					levelMap[y][x]->setType(slb[y][x].typeID);				
					if(slb[y][x].typeID == CV_BLOCK_TYPE_UNCLAIMED_LAND_ID)
						unclaimedBlocksList[getBlock(x,y)] = getBlock(x,y);
				}
			}

			std::vector<CBlock*> Blocks;
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{	// If earth block is next to a peice of claimed land, add it to the unfortified list (todo: check land is yours)
					if((slb[y][x].typeID == CV_BLOCK_TYPE_EARTH_ID || slb[y][x].typeID == CV_BLOCK_TYPE_EARTH_WITH_TORCH_PLATE_ID) && (slb[y+1][x].typeID == CV_BLOCK_TYPE_CLAIMED_LAND_ID || slb[y-1][x].typeID == CV_BLOCK_TYPE_CLAIMED_LAND_ID || slb[y][x+1].typeID == CV_BLOCK_TYPE_CLAIMED_LAND_ID || slb[y][x-1].typeID == CV_BLOCK_TYPE_CLAIMED_LAND_ID))
						unfortifiedBlocksList[getBlock(x,y)] = getBlock(x,y);
				}
			}
			return true;
		}

		bool CLevelManager::saveSLB(string fileName)
		{
			struct sSLB
			{
				GLbyte typeID;	// ID of the block
				GLbyte empty;	// empty not used reserved value
			};

			// open file and read data
			FILE *inSLB;

			if (!(inSLB=fopen(fileName.c_str(),"wb")))
			{
				return false;
			}

			sSLB slb[CV_LEVEL_MAP_SIZE][CV_LEVEL_MAP_SIZE];

			// process the data
			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					slb[y][x].typeID = levelMap[y][x]->getType();			
				}
			}

			for (GLuint i=0; i<CV_LEVEL_MAP_SIZE; i++)
			{
				if (fwrite(slb[i],CV_LEVEL_MAP_SIZE,sizeof(sSLB),inSLB)==0)
				{
					fclose(inSLB);
					return FALSE;
				}
			}
			fclose(inSLB);	
			return true;
		}

		bool CLevelManager::loadOWN(string fileName)
		{
			FILE *inOWN = NULL;
			if (!(inOWN=fopen(fileName.c_str(),"rb")))
			{
				return false;
			}

			GLubyte line[256][256];

			for (GLint i=0; i<256; i++)
			{
				fread(line[i],1,256,inOWN);
			}

			fclose(inOWN);

			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					levelMap[y][x]->setOwner(line[y*3][x*3]);
				}
			}
			return true;
		}

		bool CLevelManager::saveOWN(string fileName)
		{
			FILE *inOWN = NULL;
			if (!(inOWN=fopen(fileName.c_str(),"wb")))
			{
				return false;
			}

			GLubyte line[256][256];


			for (GLint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					line[y*3][x*3]=levelMap[y][x]->getOwner();
				}
			}

			for (int i=0; i<256; i++)
			{
				fwrite(line[i],1,256,inOWN);
			}
			
			fclose(inOWN);
			return true;
		}

		bool CLevelManager::loadTNG(string fileName)
		{
			FILE *f_tng = NULL;

			if (!(f_tng=fopen(fileName.c_str(),"rb")))
			{
				return false;
			}

			GLshort things_count=0;
			GLubyte thing_data[21];

			GLubyte thing_type;
			GLubyte thing_subtype;
			GLubyte owner;

			GLubyte subtile_x,subtile_y;

			GLint map_x,map_y;

			GLfloat thing_x,thing_y;

			srand((unsigned int)time(NULL));
		    
			// read the number of thing in the dk_map
			fread(&things_count,1,2,f_tng);			

			for (GLshort i=0; i<things_count; i++)
			{
				fread(thing_data,1,21,f_tng);

				thing_type=thing_data[6];
				thing_subtype=thing_data[7];
				owner=thing_data[8];

				subtile_x=thing_data[1];
				subtile_y=thing_data[3];

				map_x=(GLint)subtile_x/3;
				map_y=(GLint)subtile_y/3;

				thing_x=(GLfloat)map_x*CV_BLOCK_WIDTH+(GLfloat)(subtile_x%3)*(CV_BLOCK_WIDTH/3.0f)+(CV_BLOCK_WIDTH/6.0f);
				thing_y=(GLfloat)map_y*CV_BLOCK_DEPTH+(GLfloat)(subtile_y%3)*(CV_BLOCK_DEPTH/3.0f)+(CV_BLOCK_DEPTH/6.0f);

				CBlock *block = getBlock(map_x,map_y);

				GLfloat y = (block->isWater() || block->isLava())?0.0f:CV_BLOCK_HEIGHT/4.0f;
		        
				switch (thing_type)
				{
					case TNG_ITEM_DECORATION:
					{
						if (thing_subtype==TNG_BONUS_RESSURECT_CREATURE)
						{
							block->addModel("MODEL_BONUS",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BONUS_INCREASE_LEVEL)
						{
							block->addModel("MODEL_BONUS",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BONUS_REVEAL_MAP)
						{
							block->addModel("MODEL_BONUS",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BONUS_TRANSFER_CREATURE)
						{
							block->addModel("MODEL_BONUS",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BONUS_STEAL_HERO)
						{
							block->addModel("MODEL_BONUS",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BONUS_MULTIPLY_CREATURES)
						{
							block->addModel("MODEL_BONUS",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BONUS_MAKE_SAFE)
						{
							block->addModel("MODEL_BONUS",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_HAND_OF_EVIL)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_CREATE_IMP)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_SIGHT_OF_EVIL)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_CAVE_IN)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_HEAL_CREATURE)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_HOLD_AUDIENCE)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_LIGHTNING)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_SPEED_CREATURE)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_PROTECT_CREATURE)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_CONCEAL_CREATURE)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_DISEASE)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_DESTROY_WALLS)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_SPELL_ARMAGEDDON)
						{
							block->addModel("MODEL_BOOK",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BOX_BOULDER)
						{
							block->addModel("MODEL_BOX",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BOX_ALARM)
						{
							block->addModel("MODEL_BOX",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BOX_POISON)
						{
							block->addModel("MODEL_BOX",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BOX_LIGHTNING)
						{
							block->addModel("MODEL_BOX",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BOX_WORD_OF_POWER)
						{
							block->addModel("MODEL_BOX",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_BOX_LAVA)
						{
							block->addModel("MODEL_BOX",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_STATUE1)
						{
							block->addModel("MODEL_STATUE0",vector3f(thing_x,y,thing_y),true);
						}
						else if (thing_subtype==TNG_STATUE2)
						{
							block->addModel("MODEL_STATUE1",vector3f(thing_x,y,thing_y),true);
						}
						else if (thing_subtype==TNG_STATUE_UNLIT)
						{
							block->addModel("MODEL_STATUE2",vector3f(thing_x,y,thing_y),true);
						}
						else if (thing_subtype==TNG_KEY)
						{
							block->addModel("MODEL_KEY",vector3f(thing_x,(GLfloat)thing_data[5]*(CV_BLOCK_HEIGHT/4.2f),thing_y));
						}
						else if (thing_subtype==TNG_GOLD_250)
						{
							block->addModel("MODEL_GOLD250",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_GOLD_500)
						{
							block->addModel("MODEL_GOLD500",vector3f(thing_x,y,thing_y));
						}
						else if (thing_subtype==TNG_HERO_GATE)
						{
							block->addModel(new CHeroGate("MODEL_HGATE",vector3f(thing_x,y,thing_y)));
							//CV_GAME_MANAGER->getConsole()->writeLine("HERO GATE: "+CConversions::intToStr(map_x)+","+CConversions::intToStr(map_y));
						}
						else if (thing_subtype==TNG_BARREL)
						{
							block->addModel("MODEL_BARREL",vector3f(thing_x,y,thing_y),true);
						}
						else if (thing_subtype==TNG_CANDLE_ON_STICK)
						{
							//block->addModel("MODEL_CANDLE",vector3f(thing_x,y,thing_y));
							block->addModel(new CLightingObject("MODEL_CANDLE",vector3f(thing_x,y,thing_y),CLightingObject::LOT_CANDLE));
						}
						else if (thing_subtype==TNG_PINK_VASE)
						{
							block->addModel("MODEL_VASE0",vector3f(thing_x,y,thing_y),true);
						}
						else if (thing_subtype==TNG_BLUE_VASE)
						{
							block->addModel("MODEL_VASE1",vector3f(thing_x,y,thing_y),true);
						}
						else if (thing_subtype==TNG_GREEN_VASE)
						{
							block->addModel("MODEL_VASE2",vector3f(thing_x,y,thing_y),true);
						}
						else if (thing_subtype==TNG_LIT_TORCH || thing_subtype==TNG_UNLIT_TORCH)
						{
							if (!getBlock(map_x,map_y)->isRoom())
							{
								block->addModel(new CLightingObject("MODEL_TORCH",vector3f(thing_x,CV_BLOCK_HEIGHT-CV_BLOCK_HEIGHT/7.0f,thing_y),TNG_LIT_TORCH?CLightingObject::LOT_TORCH_LIT:CLightingObject::LOT_TORCH_UNLIT));					
							}
						}

						break;
					}
					// creature
					case TNG_CREATURE:
					{
						if (thing_subtype==TNG_CREATURE_BARBARIAN)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED BARBARIAN");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("BARBARIAN",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_KNIGHT)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED KNIGHT");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("KNIGHT",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_HORNED_REAPER)
						{
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED HORNED REAPER");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("HORNED_REAPER",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_DRAGON)
 						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED DRAGON");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("DRAGON",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_DEMON_SPAWN)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED DEMON SPAWN");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("DEMON_SPAWN",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_FLY)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED FLY");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("FLY",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_BILE_DEMON)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED BILE DEMON");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("BILE_DEMON",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_IMP)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED IMP");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("IMP",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_BEETLE)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED BEETLE");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("BEETLE",vector3f(thing_x,y,thing_y),owner);
 						}
						else if (thing_subtype==TNG_CREATURE_VAMPIRE)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED VAMPIRE");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("VAMPIRE",vector3f(thing_x,y,thing_y),owner);
						}
						else if (thing_subtype==TNG_CREATURE_SPIDER)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED SPIDER");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("SPIDER",vector3f(thing_x,y,thing_y),owner);
 						}
 						else if (thing_subtype==TNG_CREATURE_HELL_HOUND)
						{							
							CV_GAME_MANAGER->getConsole()->writeLine("CREATED HELL HOUND");
							CV_GAME_MANAGER->getCreatureManager()->addCreature("HELL_HOUND",vector3f(thing_x,y,thing_y),owner);
 						}

						break;
					}
					// room effect
					case 7:
					{
						break;
					}
					// trap
					case TNG_TRAP:
					{
						thing_x=(GLfloat)map_x*CV_BLOCK_WIDTH+CV_BLOCK_WIDTH/2.0f;
						thing_y=(GLfloat)map_y*CV_BLOCK_DEPTH+CV_BLOCK_DEPTH/2.0f;
						
						if (thing_subtype==TNG_TRAP_ALARM)
						{
							//trap_class_name=CLASS_NAME_TRAP_ALARM;
							block->addModel(new CTrap("MODEL_TRAP",vector3f(thing_x,y,thing_y),CTrap::TT_ALARM));
						}
						else if (thing_subtype==TNG_TRAP_POISON)
						{
							block->addModel(new CTrap("MODEL_TRAP",vector3f(thing_x,y,thing_y),CTrap::TT_POISON));
						}
						else if (thing_subtype==TNG_TRAP_BOULDER)
						{
							block->addModel(new CTrap("MODEL_BOULDER",vector3f(thing_x,y,thing_y),CTrap::TT_BOULDER));
						}
						else if (thing_subtype==TNG_TRAP_LIGHTNING)
						{
							block->addModel(new CTrap("MODEL_TRAP",vector3f(thing_x,y,thing_y),CTrap::TT_LIGHTNING));
						}
						else if (thing_subtype==TNG_TRAP_WORD_OF_POWER)
						{
							block->addModel(new CTrap("MODEL_TRAP",vector3f(thing_x,y,thing_y),CTrap::TT_WORD_OF_POWER));
						}
						else if (thing_subtype==TNG_TRAP_LAVA)
						{
							block->addModel(new CTrap("MODEL_TRAP",vector3f(thing_x,y,thing_y),CTrap::TT_LAVA));
						}
						break;
					}
					// door
					case 9:
					{
						break;
					}
				}
			}

			fclose(f_tng);

			return true;
		}

		bool CLevelManager::saveTNG(string fileName)
		{
			// TODO this
			return true;
		}

		CBlock *CLevelManager::getBlock(GLint x, GLint y)
		{
			return (x>=0&&y>=0&&x<CV_LEVEL_MAP_SIZE&&y<CV_LEVEL_MAP_SIZE?levelMap[y][x]:NULL);
		}

		CBlock *CLevelManager::getBlockOld(GLint x, GLint y)
		{
			return getBlock(y,x);
		}

		GLint CLevelManager::getBlockType(GLint x, GLint y)
		{
			return (x>=0&&y>=0&&x<CV_LEVEL_MAP_SIZE&&y<CV_LEVEL_MAP_SIZE?levelMap[y][x]->getType():-1);
		}

		GLint CLevelManager::getBlockTypeOld(GLint x, GLint y)
		{
			return getBlockType(y,x);
		}

		bool CLevelManager::isFullBlock(CBlock *block)
		{
			return !block->isLow();
		}

		CBlock *CLevelManager::getBlock(vector2i pos)
		{
			return getBlock(pos[0],pos[1]);
		}

		bool CLevelManager::isBlockTypeNear(GLint blockType, GLint x, GLint y, bool diagonal, GLubyte owner, std::vector<CBlock*> *blocks)
		{
			if (!(getBlock(x-1,y-1) && getBlock(x+1,y+1)))
				return false;
			
			for(GLint x1 = -1; x1<=1; x1++)
				for(GLint y1 = -1; y1<=1; y1++)
					if ((diagonal || x1==0 || y1==0) && !(x1==0 && y1==0) &&
						(getBlock(x+x1,y+y1)->getType()==blockType) && (owner==getBlock(x+x1,y+y1)->getOwner()))
					{
						if(blocks) blocks->push_back(getBlock(x+x1,y+y1));
						else return true;
					}

			/*CBlock block;
			block.setOwner(owner);
			block.setType(blockType);
			if(owner <= 8)
			{
				if(isSameTypeAndOwner(x-1,y,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x-1,y));}
				if(isSameTypeAndOwner(x+1,y,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x+1,y));}
				if(isSameTypeAndOwner(x,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x,y-1));}
				if(isSameTypeAndOwner(x,y+1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x,y+1));}
			
				if(diagonal)
				{
					if(isSameTypeAndOwner(x-1,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x-1,y-1));}
					if(isSameTypeAndOwner(x-1,y+1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x-1,y+1));}
					if(isSameTypeAndOwner(x+1,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x+1,y-1));}
					if(isSameTypeAndOwner(x+1,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x+1,y+1));}
				}
			} else {
				if(isSameType(x-1,y,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x-1,y));}
				if(isSameType(x+1,y,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x+1,y));}
				if(isSameType(x,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x,y-1));}
				if(isSameType(x,y+1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x,y+1));}
			
				if(diagonal)
				{
					if(isSameType(x-1,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x-1,y-1));}
					if(isSameType(x-1,y+1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x-1,y+1));}
					if(isSameType(x+1,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x+1,y-1));}
					if(isSameType(x+1,y-1,&block)){ if(!blocks) return true; else blocks->push_back(getBlock(x+1,y+1));}
				}
			}*/

			return (blocks?blocks->size()>0:false);
		}
		 
		bool CLevelManager::isBlockClaimable(GLint x, GLint y, GLubyte owner, std::vector<CBlock*> *blocks)
		{
			if (!(getBlock(x-1,y-1) && getBlock(x+1,y+1)))
				return false;
			
			for(GLint x1 = -1; x1<=1; x1++)
				for(GLint y1 = -1; y1<=1; y1++)
					if ((x1==0 || y1==0) && !(x1==0 && y1==0) &&
						((getBlock(x+x1,y+y1)->isRoom()) || (getBlock(x+x1,y+y1)->getType()==CV_BLOCK_TYPE_CLAIMED_LAND_ID)) && (owner==getBlock(x+x1,y+y1)->getOwner()))
					{
						if(blocks) blocks->push_back(getBlock(x+x1,y+y1));
						else return true;
					}

			return (blocks?blocks->size()>0:false);
		}

		bool CLevelManager::isBlockTypeNear(GLint blockType, cml::vector2i logicalPos, bool diagonal, GLubyte owner, std::vector<CBlock*> *blocks)
		{
			return isBlockTypeNear(blockType,logicalPos[0], logicalPos[1], diagonal,owner,blocks);
		}

		bool CLevelManager::isBlockClaimable(cml::vector2i logicalPos, GLubyte owner, std::vector<CBlock*> *blocks)
		{
			return isBlockClaimable(logicalPos[0], logicalPos[1], owner, blocks);
		}

		std::map<CBlock*,CBlock*> *CLevelManager::getUnclaimedBlocksList()
		{
			return &unclaimedBlocksList;
		}

		GLvoid CLevelManager::removeMarkedBlock(CBlock *block)
		{
			markedBlocksList.erase(block);
		}

		GLvoid CLevelManager::removeUnclaimedBlock(CBlock *block)
		{
			unclaimedBlocksList.erase(block);
		}

		GLvoid CLevelManager::removeUnfortifiedBlock(CBlock *block)
		{
			unfortifiedBlocksList.erase(block);
		}

		GLvoid CLevelManager::addMarkedBlock(CBlock *block)
		{
			markedBlocksList[block] = block;
		}

		GLvoid CLevelManager::addUnclaimedBlock(CBlock *block)
		{
			unclaimedBlocksList[block] = block;
		}

		GLvoid CLevelManager::addUnfortifiedBlock(CBlock *block)
		{
			unfortifiedBlocksList[block] = block;
		}

		CBlock *CLevelManager::getMarkedBlock(GLubyte owner, cml::vector2i position)
		{
			CBlock *tempblock = NULL;
			std::vector<cml::vector2i> path;
			std::vector<cml::vector2i> currpath;
			currpath.clear();
			for (map<CBlock*,CBlock*>::iterator iter=markedBlocksList.begin(); iter!=markedBlocksList.end(); iter++)
			{
				path.clear();
				if(CV_GAME_MANAGER->getPathManager()->findPath(position,((CBlock*)iter->second)->getLogicalPosition(),&path))
				{
					if((path.size() < currpath.size()) || (currpath.size() == 0))
					{
						tempblock = (CBlock*)iter->second;
						currpath = path;
					}
				}
			}
			if(tempblock)
				return tempblock;
			
			return NULL;
		}

		CBlock *CLevelManager::getUnclaimedBlock(GLubyte owner, cml::vector2i position)
		{
			CBlock *tempblock = NULL;
			std::vector<cml::vector2i> path;
			std::vector<cml::vector2i> currpath;
			currpath.clear();
			for (map<CBlock*,CBlock*>::iterator iter=unclaimedBlocksList.begin(); iter!=unclaimedBlocksList.end(); iter++)
			{
				path.clear();
				if(!((CBlock*)iter->second)->isTaken())
				{
					if(isBlockClaimable(((CBlock*)iter->second)->getLogicalPosition(),owner))
					{	
						if(CV_GAME_MANAGER->getPathManager()->findPath(position,((CBlock*)iter->second)->getLogicalPosition(),&path))
						{
							if((path.size() < currpath.size()) || (currpath.size() == 0))
							{
								tempblock = (CBlock*)iter->second;
								currpath = path;
							}
						}
					}
				}
			}
			if(tempblock)
			{
				tempblock->setTaken(true);
				return tempblock;
			}
			return NULL;
		}

		CBlock *CLevelManager::getUnfortifiedBlock(GLubyte owner, cml::vector2i position)
		{
			CBlock *tempblock = NULL;
			std::vector<cml::vector2i> path;
			std::vector<cml::vector2i> currpath;
			currpath.clear();
			for (map<CBlock*,CBlock*>::iterator iter=unfortifiedBlocksList.begin(); iter!=unfortifiedBlocksList.end(); iter++)
			{
				path.clear();
				if(!((CBlock*)iter->second)->isTaken())
				{
					if(isBlockClaimable(((CBlock*)iter->second)->getLogicalPosition(),owner))
					{
						if(CV_GAME_MANAGER->getPathManager()->findPath(position,((CBlock*)iter->second)->getLogicalPosition(),&path))
						{
							if((path.size() < currpath.size()) || (currpath.size() == 0))
							{
								tempblock = (CBlock*)iter->second;
								currpath = path;
							}
						}
					}
				}
			}
			if(tempblock)
			{
				tempblock->setTaken(true);
				return tempblock;
			}
			return NULL;
		}

		void CLevelManager::getUnclaimedBlock(GLubyte owner, std::vector<game_objects::CBlock*> *blocks)
		{
			for (map<CBlock*,CBlock*>::iterator iter=unclaimedBlocksList.begin(); iter!=unclaimedBlocksList.end(); iter++)
			{
				if(!((CBlock*)iter->second)->isTaken())
				{
					if(isBlockClaimable(((CBlock*)iter->second)->getLogicalPosition(),owner))
					{
						blocks->push_back(iter->second);
					}
				}
			}
			return;
		}

		bool CLevelManager::isNotSameTypeAndOwnerAndNotRockOrEarth(GLint targetX, GLint targetY, CBlock *sourceBlock)
		{
			GLint type = sourceBlock->getType();
			return (!isSameTypeAndOwner(targetX,targetY,sourceBlock) && !(type==CV_BLOCK_TYPE_EARTH_ID || type==CV_BLOCK_TYPE_ROCK_ID));
		}

		bool CLevelManager::isSameTypeAndOwner(GLint targetX, GLint targetY, CBlock *sourceBlock)
		{
			return (isSameType(targetX,targetY,sourceBlock) && isSameOwner(targetX,targetY,sourceBlock));
		}

		bool CLevelManager::isSameOwner(GLint targetX, GLint targetY, CBlock *sourceBlock)
		{
			CBlock *tmp = getBlock(targetX,targetY);
			return (tmp?tmp->getOwner()==sourceBlock->getOwner():false);
		}

		bool CLevelManager::isSameType(GLint targetX, GLint targetY, CBlock *sourceBlock)
		{
			CBlock *tmp = getBlock(targetX,targetY);
			return (tmp?tmp->getType()==sourceBlock->getType():false);
		}

		GLvoid CLevelManager::calculateBlockCeilingHeight(CBlock *block)
		{
			GLint min_height=0;

			// the checking goes on in the clock-wise fashion. first goes right

			vector2i lPos = block->getLogicalPosition();

			GLint tx=lPos[0];
			GLint ty=lPos[1];
			GLint height=0;

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			while (lManager->getBlock(tx,ty)->isLow() && height<MAX_CEILING_HEIGHT)
			{
				height++;
				tx++;
			}

			min_height=height;

			// right down
			tx=lPos[0];
			ty=lPos[1];
			height=0;

			while (lManager->getBlock(tx,ty)->isLow() && height<min_height)
			{
				height++;
				tx++;
				ty++;
			}

			if (height<min_height)
			{
				min_height=height;
			}

			// down
			tx=lPos[0];
			ty=lPos[1];
			height=0;

			while (lManager->getBlock(tx,ty)->isLow() && height<min_height)
			{
				height++;
				ty++;
			}

			if (height<min_height)
			{
				min_height=height;
			}

			// left down
			tx=lPos[0];
			ty=lPos[1];
			height=0;

			while (lManager->getBlock(tx,ty)->isLow() && height<min_height)
			{
				height++;
				tx--;
				ty++;
			}

			if (height<min_height)
			{
				min_height=height;
			}

			// left
			tx=lPos[0];
			ty=lPos[1];
			height=0;

			while (lManager->getBlock(tx,ty)->isLow() && height<min_height)
			{
				height++;
				tx--;
			}

			if (height<min_height)
			{
				min_height=height;
			}

			// left up
			tx=lPos[0];
			ty=lPos[1];
			height=0;

			while (lManager->getBlock(tx,ty)->isLow() && height<min_height)
			{
				height++;
				tx--;
				ty--;
			}

			if (height<min_height)
			{
				min_height=height;
			}

			// up
			tx=lPos[0];
			ty=lPos[1];
			height=0;

			while (lManager->getBlock(tx,ty)->isLow() && height<min_height)
			{
				height++;
				ty--;
			}

			if (height<min_height)
			{
				min_height=height;
			}

			// right up
			tx=lPos[0];
			ty=lPos[1];
			height=0;

			while (lManager->getBlock(tx,ty)->isLow() && height<min_height)
			{
				height++;
				tx++;
				ty--;
			}

			if (height<min_height)
			{
				min_height=height;
			}

			block->setCeilingHeight(min_height);
		}

		string CLevelManager::onAction(string keyword, string params)
		{
			string checkResult = "";
			std::vector<string> tParams;

			if (keyword==LDLVL)
			{
				CConsole *console = CV_GAME_MANAGER->getConsole();

				bool os = console->setForceRedraw(true);

				if (!CConsoleListener::checkParams(params,1,checkResult,tParams))
				{
					return checkResult;
				}

				levelFileName = CV_RESOURCES_DIRECTORY+"levels\\"+tParams[0];

				console->writeLine("Checking necessary level files...");
				if (!levelExists(levelFileName))
				{
					return "Necessary files for level "+tParams[0]+" missing!";
				}

				console->writeLine("Reseting lighting manager...");
				CV_GAME_MANAGER->getLightingManager()->shutdown();

				console->writeLine("Reseting room manager...");
				CV_GAME_MANAGER->getRoomManager()->shutdown();

				console->writeLine("Reseting current level...");
				this->shutdown();

				console->writeLine("Loading new level...");

				console->setForceRedraw(os);

				if (!this->init())
				{					
					return "Level "+tParams[0]+" loading failed!";
				}
				else
				{
					// some other things need to be reset too
					CV_GAME_MANAGER->getRoomManager()->init();

					CV_GAME_MANAGER->getLightingManager()->init();

					CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->updateRoomInfo();

					return "Level "+tParams[0]+" loaded.";
				}				
			}
			else if (keyword==PLOG)
			{
				std::vector<string> log;
				CLogger::getLog(log);

				for (std::vector<string>::iterator sIter = log.begin(); sIter != log.end(); sIter++)
				{
					CV_GAME_MANAGER->getConsole()->writeLine(*sIter);
				}
			}
			else if (keyword==SAVE)
			{
				CConsole *console = CV_GAME_MANAGER->getConsole();

				if (!CConsoleListener::checkParams(params,1,checkResult,tParams))
				{
					return checkResult;
				}

				levelFileName = CV_RESOURCES_DIRECTORY+"saves\\"+tParams[0];

				console->writeLine("Checking if save exists...");
				if (levelExists(levelFileName))
				{
					return "Save "+tParams[0]+" already exists!";
				}
				else
				{
					saveOWN(levelFileName + ".OWN");
					saveSLB(levelFileName + ".SLB");
					saveTNG(levelFileName + ".TNG");
					return "Saved "+tParams[0]+" sucessfully.";
				}
			}

			return "<>";
		}
	};
};