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
		mOwner = CV_PLAYER_UNDEFINED;
		mCurrentAction = NULL;
		mCreatureState = IS_IDLE;
		mModel = NULL;
		mName = "";
		m_MoveSpeed = 0.00015f;
		
		mMoveVector = vector3f(0.0f,0.0f,0.0f);
		mLair = vector2f(0,0);

		mLevel = 1;
		mGold = 0;
		mCount = 0.0f;
		mChange = 100.0f;
		mHunger = 1.0f;

		mHealth = 100.0f;
		mSleep = 3000.0f;
	}

	CCreature::~CCreature()
	{
		if (mModel)
		{
			CV_GAME_MANAGER->getResourceManager()->returnModel(mName,mModel);
		}
	}

	void CCreature::setName(string name)
	{
		this->mName = name;
	}

	void CCreature::setOwner(GLubyte owner)
	{
		this->mOwner=owner;
	}

	void CCreature::setLevel(GLint level)
	{
		if (this->getLevel() < 10) // cant level if already level 10
		{
			this->mLevel = level;
			CV_GAME_MANAGER->getConsole()->writeLine(this->getName()+" has leveled up to "+CConversions::intToStr(this->getLevel()));
		}
	}

	void CCreature::setGold(GLint gold)
	{
		this->mGold=gold;
	}

	void CCreature::addCurrentXP(GLint CurrentXP)
	{
		// add the experiance
		this->mCurrentXP = (this->getCurrentXP() + CurrentXP);

		// if the creature has enough experiance to level
		if (this->getCurrentXP() >= this->getLevel()*100) //some forumla
		{
			this->mCurrentXP = this->getCurrentXP() - (this->getLevel()*100); // reset xp and add remaining xp from level
			this->setLevel(this->getLevel() + 1);// level up
		}
	}

	void CCreature::setModel(loaders::CBR5Model *model)
	{
		this->mModel = model;
		this->mModel->setInterpolate(true);
		this->mModel->setConnected(true);
	}

	void CCreature::setAction(GLint action, GLint startFrame, GLint endFrame)
	{
		actions[action] = mModel->registerAction(startFrame,endFrame);
	}

	void CCreature::useAction(GLint action)
	{
		if(this->mCurrentAction != action)
		{
			this->mCurrentAction = action;

			if (mModel)
			{
				mModel->doAction(actions[action]);
			}
		}
	}

	void CCreature::faceBlock(CBlock *block)
	{
		mRotation[1] = 90.0f-(float)(atan2(block->getLogicalPosition()[1]-floor(mPosition[2]/CV_BLOCK_WIDTH),block->getLogicalPosition()[0]-floor(mPosition[0]/CV_BLOCK_WIDTH))*180.0f/M_PI);
	}

	void CCreature::walkPath(GLfloat deltaTime)
	{
		if(path.size() == 0)
		{
			if(mCreatureState == IS_GOING_TO_EAT)
				mCreatureState = IS_AT_EATING;
			if(mCreatureState == IS_GOING_TO_MAKE_LAIR)
				mCreatureState = IS_AT_MAKING_LAIR;
			if(mCreatureState == IS_GOING_TO_TRAIN)
				mCreatureState = IS_AT_TRAINING;
			if(mCreatureState == IS_GOING_TO_SLEEPING)
				mCreatureState = IS_AT_SLEEPING;
			return;
		}
		cml::vector2i point = path.back();
		GLfloat tX = (GLfloat)point[0]*CV_BLOCK_WIDTH+CV_BLOCK_WIDTH/2.0f;
		GLfloat tZ = (GLfloat)point[1]*CV_BLOCK_DEPTH+CV_BLOCK_DEPTH/2.0f;

		if (fabs(mPosition[0]-tX)<=0.01f && fabs(mPosition[2]-tZ)<=0.01f)
		{
			path.pop_back();
			if(path.size()==0)
			{
				mMoveVector[0] = 0.0f;
				mMoveVector[2] = 0.0f;
				return;
			}
			point = path.back();
			tX = (GLfloat)point[0]*CV_BLOCK_WIDTH+CV_BLOCK_WIDTH/2.0f;
			tZ = (GLfloat)point[1]*CV_BLOCK_DEPTH+CV_BLOCK_DEPTH/2.0f;
		}
		
		//calculate new movement direction
		mMoveVector[0] = tX-mPosition[0];
		mMoveVector[2] = tZ-mPosition[2];
		mMoveVector.normalize();
		
		mRotation[1] = 90.0f-(float)(atan2(mMoveVector[2],mMoveVector[0])*180.0f/M_PI);

		cml::vector3f oldPos = mPosition;
		mPosition += mMoveVector*m_MoveSpeed*deltaTime;

		if ((mPosition[0]-tX > 0.0f && oldPos[0]-tX<0.0f)
			|| (mPosition[0]-tX < 0.0f && oldPos[0]-tX>0.0f))
			mPosition[0] = tX;
		if ((mPosition[2]-tZ > 0.0f && oldPos[2]-tZ<0.0f)
			|| (mPosition[2]-tZ < 0.0f && oldPos[2]-tZ>0.0f))
			mPosition[2] = tZ;
			
	}

	void CCreature::Idle(GLfloat deltaTime)
	{
		if (mCount>=mChange)
		{
			GLfloat tX = (GLfloat)(rand()%100-50);
			GLfloat tZ = (GLfloat)(rand()%100-50);
			mMoveVector[0] = tX-mPosition[0];
			mMoveVector[2] = tZ-mPosition[2];
			mMoveVector.normalize();
			mRotation[1] = 90.0f-(float)(atan2(mMoveVector[2],mMoveVector[0])*180.0f/M_PI);
			mChange=(GLfloat)((rand()%100))+300.0f;
			mCount = 0.0f;
		}

		GLint X = (GLint)(mPosition[0]/CV_BLOCK_WIDTH);
		GLint Y = (GLint)(mPosition[2]/CV_BLOCK_DEPTH);
		if(CV_GAME_MANAGER->getLevelManager()->getBlock(X,Y)->isWalkable(false))
		{
			mPosition += mMoveVector*m_MoveSpeed*deltaTime;
			mCount+=0.5f;
		}
		else
		{
			mPosition -= mMoveVector*m_MoveSpeed*deltaTime;
			mCount=mChange;
		}

	}

	void CCreature::draw(GLfloat deltaTime)
	{
		if (mModel)
		{
			CEntity::moveTo();
			CEntity::rotateTo();
			mModel->draw(deltaTime);
			CEntity::rotateBack();
			CEntity::moveBack();
		}
	}

	CBR5Model *CCreature::getModel()
	{
		return mModel;
	}

	string CCreature::getName()
	{
		return mName;
	}

	unsigned char CCreature::getOwner()
	{
		return mOwner;
	}

	int CCreature::getLevel()
	{
		return mLevel;
	}

	int CCreature::getCurrentXP()
	{
		return mCurrentXP;
	}

	int CCreature::getGold()
	{
		return mGold;
	}

	void CCreature::update(GLfloat deltaTime)
	{

		if(mCreatureState == IS_IDLE)
		{
			//TODO: if standing still and idling use the idle animation for now use WALK (because he is moving)
			useAction(AA_WALK);
			Idle(deltaTime);

			//HAVE I GOT A LAIR??!?!?!
			if(mLair[0] == 0 && mLair[1] == 0)
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
							mCurrentBlock = thisBlock;
							break;
						}
					}

					if(mCurrentBlock)
					{
						cml::vector2i currPos = cml::vector2i((int)floor(mPosition[0]/CV_BLOCK_WIDTH),(int)floor(mPosition[2]/CV_BLOCK_DEPTH));
						if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,mCurrentBlock->getLogicalPosition(),&path))
						{
							mCreatureState = IS_GOING_TO_MAKE_LAIR;
							return;
						}
					}
				}
			}
			//////////////////////////////////
			if(mSleep < 3000.0f)
			{
				if(mLair[0] != 0 && mLair[1] != 0)
				{
					path.clear();
					CBlock *destBlock;
					destBlock = CV_GAME_MANAGER->getLevelManager()->getBlock(mLair[0], mLair[1]);
					if (destBlock)
					{
						cml::vector2i currPos = cml::vector2i((int)floor(mPosition[0]/CV_BLOCK_WIDTH),(int)floor(mPosition[2]/CV_BLOCK_DEPTH));
						if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,destBlock->getLogicalPosition(),&path))
						{
							mCreatureState = IS_GOING_TO_SLEEPING;
							return;
						}
					}
				}
			}

			//TODO SEPERATE FUNCTION CHECK IF HUNGRY
			if(mHunger < 2100.0f)
			{
				path.clear();
				CBlock *destBlock;
				destBlock = CV_GAME_MANAGER->getRoomManager()->getRoom(CV_BLOCK_TYPE_HATCHERY_ID, this->getOwner());
				if (destBlock)
				{
					cml::vector2i currPos = cml::vector2i((int)floor(mPosition[0]/CV_BLOCK_WIDTH),(int)floor(mPosition[2]/CV_BLOCK_DEPTH));
					if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,destBlock->getLogicalPosition(),&path))
					{
						mCreatureState = IS_GOING_TO_EAT;
						return;
					}
				}
			}
			//END CHECK IF HUNGRY

			// training check
			if(mHunger > 5000.0f && mHealth > 50 /*some other checke */)
			{
				path.clear();

				mCurrentBlock = CV_GAME_MANAGER->getRoomManager()->getRoom(CV_BLOCK_TYPE_TRAINING_ROOM_ID, this->getOwner());
				if(mCurrentBlock != NULL)
				{
					rooms::CRoom *currRoom = CV_GAME_MANAGER->getRoomManager()->getRoom(mCurrentBlock->getRoomIndex());
					mCurrentBlock = NULL;
					for (std::vector<CBlock*>::iterator rmIter = currRoom->getRoomTilesVector()->begin(); rmIter != currRoom->getRoomTilesVector()->end(); rmIter++)
					{
						CBlock *thisBlock = *rmIter;
						
						for (std::vector<block_objects::CBlockObject*>::iterator rmIter = thisBlock->getBlockObjects()->begin(); rmIter != thisBlock->getBlockObjects()->end(); rmIter++)
						{
							block_objects::CBlockObject *bObject = *rmIter;

							if (bObject->getName() == "MODEL_ROD")
							{
								mCurrentBlock = thisBlock;
								break;
							}
						}
					}

					if(mCurrentBlock)
					{
						cml::vector2i currPos = cml::vector2i((int)floor(mPosition[0]/CV_BLOCK_WIDTH),(int)floor(mPosition[2]/CV_BLOCK_DEPTH));
						if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,mCurrentBlock->getLogicalPosition(),&path))
						{
							mCreatureState = IS_GOING_TO_TRAIN;
							return;
						}
					}
				}
			}
		}
		else if (mCreatureState == IS_GOING_TO_EAT)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (mCreatureState == IS_GOING_TO_SLEEPING)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}

		else if (mCreatureState == IS_GOING_TO_MAKE_LAIR)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (mCreatureState == IS_GOING_TO_TRAIN)
		{
			//little hacky fix so creature isnt INSIDE THE ROD
			//if(CV_GAME_MANAGER->getLevelManager()->getBlock((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH))->getType() == CV_BLOCK_TYPE_TRAINING_ROOM_ID)
			//	creatureState = IS_AT_TRAINING;
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (mCreatureState == IS_AT_EATING)
		{
			mCreatureState = IS_EATING;
		}
		else if (mCreatureState == IS_AT_SLEEPING)
		{
			useAction(AA_SLEEP);
			mCreatureState = IS_SLEEPING;	
		}
		else if (mCreatureState == IS_AT_MAKING_LAIR)
		{
			mCreatureState = IS_MAKING_LAIR;
		}
		else if (mCreatureState == IS_AT_TRAINING)
		{
			faceBlock(mCurrentBlock);
			useAction(AA_ATTACK1);
			mCreatureState = IS_TRAINING;
		}
		else if (mCreatureState == IS_EATING)
		{
			//todo pick up chicken and eat ect..
			useAction(AA_EAT);
			//CV_GAME_MANAGER->getConsole()->writeLine(CConversions::floatToStr(mHunger) + " " + CConversions::floatToStr(deltaTime));
			this->mHunger+=(deltaTime);
			if(mHunger >= 10000.0f)
				mCreatureState = IS_IDLE;
			mCount=mChange;
		}
		else if (mCreatureState == IS_SLEEPING)
		{
			//CV_GAME_MANAGER->getConsole()->writeLine(CConversions::floatToStr(mSleep) + " sleep " + CConversions::floatToStr(deltaTime));
			mCreatureState = IS_SLEEPING;
			// moan if no lair ect
			this->mSleep+=(deltaTime);
			if(mSleep >= 5000.0f)
			{
				mCreatureState = IS_IDLE;
			}
		}
		else if (mCreatureState == IS_MAKING_LAIR)
		{
			block_objects::CBlockObject *obj = new block_objects::CBlockObject("MODEL_GRAVE",mPosition, CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_GRAVE"));
			obj->setClassName("BED");
			CV_GAME_MANAGER->getLevelManager()->getBlock((int)floor(mPosition[0]/CV_BLOCK_WIDTH), (int)floor(mPosition[2]/CV_BLOCK_DEPTH))->addModel(obj);

			mLair[0] = mPosition[0]/CV_BLOCK_WIDTH;
			mLair[1] = mPosition[2]/CV_BLOCK_DEPTH;
			mCount=mChange;
			mCreatureState = IS_IDLE;
		}
		else if (mCreatureState == IS_TRAINING)
		{
			mCount=mChange;

			//CV_GAME_MANAGER->getConsole()->writeLine(CConversions::floatToStr(mHunger) + " " + CConversions::floatToStr(mSleep) + " " + CConversions::floatToStr(deltaTime));
			this->mHunger-=(deltaTime*0.0001f);
			this->mSleep-=(deltaTime*0.001f);
			 //(todo change this to be spercific for every creature)
			if((mHunger < 2000.0f) || (mSleep < 1000.0f))
			{
				mCreatureState = IS_IDLE;
				useAction(AA_WALK);
			}
		}
	}
};