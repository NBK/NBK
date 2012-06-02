#include "commons.h"
#include "Creature.h"
#include "Block.h"
#include "BlockObject.h"
#include "Room.h"

using namespace game_utils;
using namespace std;
using namespace loaders;
using namespace cml;

namespace game_objects
{
	CCreature::CCreature(): CEntity()
	{
		owner = CV_PLAYER_UNDEFINED;
		currentAction = NULL;
		creatureState = IS_IDLE;
		model = NULL;
		name = "";
		moveSpeed = 0.00015f;
		moveVector = vector3f(0.0f,0.0f,0.0f);
		level = 1;
		gold = 0;
		count = 0.0f;
		change = 100.0f;
		hunger = 1.0f;
		lair = vector2f(0,0);
		health = 100.0f;
		sleep = 3000.0f;
	}

	CCreature::~CCreature()
	{
		if (model)
		{
			CV_GAME_MANAGER->getResourceManager()->returnModel(name,model);
		}
	}

	GLvoid CCreature::setName(string name)
	{
		this->name = name;
	}

	GLvoid CCreature::setOwner(GLubyte owner)
	{
		this->owner=owner;
	}

	GLvoid CCreature::setLevel(GLint level)
	{
		if (this->getLevel() < 10) // cant level if already level 10
		{
			this->level = level;
			CV_GAME_MANAGER->getConsole()->writeLine(this->getName()+" has leveled up to "+CConversions::intToStr(this->getLevel()));
		}
	}

	GLvoid CCreature::setGold(GLint gold)
	{
		this->gold=gold;
	}

	GLvoid CCreature::addCurrentXP(GLint CurrentXP)
	{
		// add the experiance
		this->CurrentXP = (this->getCurrentXP() + CurrentXP);

		// if the creature has enough experiance to level
		if (this->getCurrentXP() >= this->getLevel()*100) //some forumla
		{
			this->CurrentXP = this->getCurrentXP() - (this->getLevel()*100); // reset xp and add remaining xp from level
			this->setLevel(this->getLevel() + 1);// level up
		}
	}

	GLvoid CCreature::setModel(loaders::CBR5Model *model)
	{
		this->model = model;
		this->model->setInterpolate(true);
		this->model->setConnected(true);
	}

	GLvoid CCreature::setAction(GLint action, GLint startFrame, GLint endFrame)
	{
		actions[action] = model->registerAction(startFrame,endFrame);
	}

	GLvoid CCreature::useAction(GLint action)
	{
		if(this->currentAction != action)
		{
			this->currentAction = action;

			if (model)
			{
				model->doAction(actions[action]);
			}
		}
	}

	GLvoid CCreature::faceBlock(CBlock *block)
	{
		rotation[1] = 90.0f-(float)(atan2(block->getLogicalPosition()[1]-floor(position[2]/CV_BLOCK_WIDTH),block->getLogicalPosition()[0]-floor(position[0]/CV_BLOCK_WIDTH))*180.0f/M_PI);
	}

	GLvoid CCreature::walkPath(GLfloat deltaTime)
	{
		if(path.size() == 0)
		{
			if(creatureState == IS_GOING_TO_EAT)
				creatureState = IS_AT_EATING;
			if(creatureState == IS_GOING_TO_MAKE_LAIR)
				creatureState = IS_AT_MAKING_LAIR;
			if(creatureState == IS_GOING_TO_TRAIN)
				creatureState = IS_AT_TRAINING;
			if(creatureState == IS_GOING_TO_SLEEPING)
				creatureState = IS_AT_SLEEPING;
			return;
		}
		cml::vector2i point = path.back();
		GLfloat tX = (GLfloat)point[0]*CV_BLOCK_WIDTH+CV_BLOCK_WIDTH/2.0f;
		GLfloat tZ = (GLfloat)point[1]*CV_BLOCK_DEPTH+CV_BLOCK_DEPTH/2.0f;

		if (fabs(position[0]-tX)<=0.01f && fabs(position[2]-tZ)<=0.01f)
		{
			path.pop_back();
			if(path.size()==0)
			{
				moveVector[0] = 0.0f;
				moveVector[2] = 0.0f;
				return;
			}
			point = path.back();
			tX = (GLfloat)point[0]*CV_BLOCK_WIDTH+CV_BLOCK_WIDTH/2.0f;
			tZ = (GLfloat)point[1]*CV_BLOCK_DEPTH+CV_BLOCK_DEPTH/2.0f;
		}
		
		//calculate new movement direction
		moveVector[0] = tX-position[0];
		moveVector[2] = tZ-position[2];
		moveVector.normalize();
		
		rotation[1] = 90.0f-(float)(atan2(moveVector[2],moveVector[0])*180.0f/M_PI);

		cml::vector3f oldPos = position;
		position += moveVector*moveSpeed*deltaTime;

		if ((position[0]-tX > 0.0f && oldPos[0]-tX<0.0f)
			|| (position[0]-tX < 0.0f && oldPos[0]-tX>0.0f))
			position[0] = tX;
		if ((position[2]-tZ > 0.0f && oldPos[2]-tZ<0.0f)
			|| (position[2]-tZ < 0.0f && oldPos[2]-tZ>0.0f))
			position[2] = tZ;
			
	}

	GLvoid CCreature::Idle(GLfloat deltaTime)
	{
		if (count>=change)
		{
			GLfloat tX = (GLfloat)(rand()%100-50);
			GLfloat tZ = (GLfloat)(rand()%100-50);
			moveVector[0] = tX-position[0];
			moveVector[2] = tZ-position[2];
			moveVector.normalize();
			rotation[1] = 90.0f-(float)(atan2(moveVector[2],moveVector[0])*180.0f/M_PI);
			change=(GLfloat)((rand()%100))+300.0f;
			count = 0.0f;
		}

		GLint X = (GLint)(position[0]/CV_BLOCK_WIDTH);
		GLint Y = (GLint)(position[2]/CV_BLOCK_DEPTH);
		if(CV_GAME_MANAGER->getLevelManager()->getBlock(X,Y)->isWalkable(false))
		{
			position += moveVector*moveSpeed*deltaTime;
			count+=0.5f;
		}
		else
		{
			position -= moveVector*moveSpeed*deltaTime;
			count=change;
		}

	}

	GLvoid CCreature::draw(GLfloat deltaTime)
	{
		if (model)
		{
			CEntity::moveTo();
			CEntity::rotateTo();
			model->draw(deltaTime);
			CEntity::rotateBack();
			CEntity::moveBack();
		}
	}

	CBR5Model *CCreature::getModel()
	{
		return model;
	}

	string CCreature::getName()
	{
		return name;
	}

	GLubyte CCreature::getOwner()
	{
		return owner;
	}

	GLint CCreature::getLevel()
	{
		return level;
	}

	GLint CCreature::getCurrentXP()
	{
		return CurrentXP;
	}

	GLint CCreature::getGold()
	{
		return gold;
	}

	GLvoid CCreature::update(GLfloat deltaTime)
	{

		if(creatureState == IS_IDLE)
		{
			//TODO: if standing still and idling use the idle animation for now use WALK (because he is moving)
			useAction(AA_WALK);
			Idle(deltaTime);

			//HAVE I GOT A LAIR??!?!?!
			if(lair[0] == 0 && lair[1] == 0)
			{
				path.clear();

				CBlock *destBlock;
				destBlock = CV_GAME_MANAGER->getRoomManager()->getRoom(CV_BLOCK_TYPE_LAIR_ID, this->getOwner());
				if(destBlock != NULL)
				{
					rooms::CRoom *currRoom = CV_GAME_MANAGER->getRoomManager()->getRoom(destBlock->getRoomIndex());

					for (std::vector<CBlock*>::iterator rmIter = currRoom->getRoomTilesVector()->begin(); rmIter != currRoom->getRoomTilesVector()->end(); rmIter++)
					{
						CBlock *thisBlock = *rmIter;
						bool foundbed = false;

						for (std::vector<block_objects::CBlockObject*>::iterator rmIter = thisBlock->getBlockObjects()->begin(); rmIter != thisBlock->getBlockObjects()->end(); rmIter++)
						{
							block_objects::CBlockObject *bObject = *rmIter;

							if (bObject->getClassName() == "BED")
								foundbed = true;
						}
						if(!foundbed)
						{
							currBlock = thisBlock;
							break;
						}
					}

					if(currBlock)
					{
						cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
						if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,currBlock->getLogicalPosition(),&path))
						{
							creatureState = IS_GOING_TO_MAKE_LAIR;
							return;
						}
					}
				}
			}
			//////////////////////////////////
			if(sleep < 3000.0f)
			{
				if(lair[0] != 0 && lair[1] != 0)
				{
					path.clear();
					CBlock *destBlock;
					destBlock = CV_GAME_MANAGER->getLevelManager()->getBlock(lair[0], lair[1]);
					if (destBlock)
					{
						cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
						if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,destBlock->getLogicalPosition(),&path))
						{
							creatureState = IS_GOING_TO_SLEEPING;
							return;
						}
					}
				}
			}

			//TODO SEPERATE FUNCTION CHECK IF HUNGRY
			if(hunger < 2100.0f)
			{
				path.clear();
				CBlock *destBlock;
				destBlock = CV_GAME_MANAGER->getRoomManager()->getRoom(CV_BLOCK_TYPE_HATCHERY_ID, this->getOwner());
				if (destBlock)
				{
					cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
					if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,destBlock->getLogicalPosition(),&path))
					{
						creatureState = IS_GOING_TO_EAT;
						return;
					}
				}
			}
			//END CHECK IF HUNGRY

			// training check
			if(hunger > 5000.0f && health > 50 /*some other checke */)
			{
				path.clear();

				currBlock = CV_GAME_MANAGER->getRoomManager()->getRoom(CV_BLOCK_TYPE_TRAINING_ROOM_ID, this->getOwner());
				if(currBlock != NULL)
				{
					rooms::CRoom *currRoom = CV_GAME_MANAGER->getRoomManager()->getRoom(currBlock->getRoomIndex());
					currBlock = NULL;
					for (std::vector<CBlock*>::iterator rmIter = currRoom->getRoomTilesVector()->begin(); rmIter != currRoom->getRoomTilesVector()->end(); rmIter++)
					{
						CBlock *thisBlock = *rmIter;
						
						for (std::vector<block_objects::CBlockObject*>::iterator rmIter = thisBlock->getBlockObjects()->begin(); rmIter != thisBlock->getBlockObjects()->end(); rmIter++)
						{
							block_objects::CBlockObject *bObject = *rmIter;

							if (bObject->getName() == "MODEL_ROD")
							{
								currBlock = thisBlock;
								break;
							}
						}
					}

					if(currBlock)
					{
						cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
						if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,currBlock->getLogicalPosition(),&path))
						{
							creatureState = IS_GOING_TO_TRAIN;
							return;
						}
					}
				}
			}
		}
		else if (creatureState == IS_GOING_TO_EAT)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (creatureState == IS_GOING_TO_SLEEPING)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}

		else if (creatureState == IS_GOING_TO_MAKE_LAIR)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (creatureState == IS_GOING_TO_TRAIN)
		{
			//little hacky fix so creature isnt INSIDE THE ROD
			//if(CV_GAME_MANAGER->getLevelManager()->getBlock((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH))->getType() == CV_BLOCK_TYPE_TRAINING_ROOM_ID)
			//	creatureState = IS_AT_TRAINING;
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (creatureState == IS_AT_EATING)
		{
			creatureState = IS_EATING;
		}
		else if (creatureState == IS_AT_SLEEPING)
		{
			useAction(AA_SLEEP);
			creatureState = IS_SLEEPING;	
		}
		else if (creatureState == IS_AT_MAKING_LAIR)
		{
			creatureState = IS_MAKING_LAIR;
		}
		else if (creatureState == IS_AT_TRAINING)
		{
			faceBlock(currBlock);
			useAction(AA_ATTACK1);
			creatureState = IS_TRAINING;
		}
		else if (creatureState == IS_EATING)
		{
			//todo pick up chicken and eat ect..
			useAction(AA_EAT);
			CV_GAME_MANAGER->getConsole()->writeLine(CConversions::floatToStr(hunger) + " " + CConversions::floatToStr(deltaTime));
			this->hunger+=(deltaTime);
			if(hunger >= 10000.0f)
				creatureState = IS_IDLE;
			count=change;
		}
		else if (creatureState == IS_SLEEPING)
		{
			CV_GAME_MANAGER->getConsole()->writeLine(CConversions::floatToStr(sleep) + " sleep " + CConversions::floatToStr(deltaTime));
			creatureState = IS_SLEEPING;
			// moan if no lair ect
			this->sleep+=(deltaTime);
			if(sleep >= 5000.0f)
			{
				creatureState = IS_IDLE;
			}
		}
		else if (creatureState == IS_MAKING_LAIR)
		{
			block_objects::CBlockObject *obj = new block_objects::CBlockObject("MODEL_GRAVE",position, CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_GRAVE"));
			obj->setClassName("BED");
			CV_GAME_MANAGER->getLevelManager()->getBlock((int)floor(position[0]/CV_BLOCK_WIDTH), (int)floor(position[2]/CV_BLOCK_DEPTH))->addModel(obj);

			lair[0] = position[0]/CV_BLOCK_WIDTH;
			lair[1] = position[2]/CV_BLOCK_DEPTH;
			count=change;
			creatureState = IS_IDLE;
		}
		else if (creatureState == IS_TRAINING)
		{
			count=change;
			CV_GAME_MANAGER->getConsole()->writeLine(CConversions::floatToStr(hunger) + " " + CConversions::floatToStr(sleep) + " " + CConversions::floatToStr(deltaTime));
			this->hunger-=(deltaTime*0.0001f);
			this->sleep-=(deltaTime*0.001f);
			 //(todo change this to be spercific for every creature)
			if((hunger < 2000.0f) || (sleep < 1000.0f))
			{
				creatureState = IS_IDLE;
				useAction(AA_WALK);
			}
		}
	}
};