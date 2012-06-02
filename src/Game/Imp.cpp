#include "commons.h"
#include "Imp.h"
#include <math.h>

using namespace game_utils;

namespace game_objects
{
	CImp::CImp(): CCreature()
	{
		impState = IS_IDLE;
		moveSpeed = 0.0005f;
		maxgold = 250;
	}

	CImp::~CImp()
	{
	}

	GLvoid CImp::checkGoldLevels()
	{
		if(this->getGold() >= this->maxgold)
		{
			path.clear();
			CBlock *destBlock;
			destBlock = CV_GAME_MANAGER->getRoomManager()->getRoom(CV_BLOCK_TYPE_TREASURE_ROOM_ID, this->getOwner());

			if(destBlock != NULL)
			{
				rooms::CRoom *currRoom = CV_GAME_MANAGER->getRoomManager()->getRoom(destBlock->getRoomIndex());

				for (std::vector<CBlock*>::iterator rmIter = currRoom->getRoomTilesVector()->begin(); rmIter != currRoom->getRoomTilesVector()->end(); rmIter++)
				{
					CBlock *thisBlock = *rmIter;
					bool found = false;

					for (std::vector<block_objects::CBlockObject*>::iterator rmIter = thisBlock->getBlockObjects()->begin(); rmIter != thisBlock->getBlockObjects()->end(); rmIter++)
					{
						block_objects::CBlockObject *bObject = *rmIter;

						if (bObject->getName() == "MODEL_GOLD250")
							found = true;

					}
					if(!found)
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
						impState = IS_GOING_TO_DEPOSITING_GOLD_DESTINATION;
						return;
					}
				}
			}
			currBlock->addModel("MODEL_GOLD250",position);
			this->setGold(0);
		}
	}

	GLvoid CImp::checkNearestForDigging()
	{
		if(impState != IS_IDLE) return;

		std::vector<CBlock*> markedBlocks,possBlocks;
		std::vector<CBlock*>::iterator markedBlocksIter;
		CBlock *block;

		if(CV_GAME_MANAGER->getLevelManager()->isBlockTypeNear(CV_BLOCK_TYPE_EARTH_ID,cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)),true,CV_PLAYER_UNDEFINED,&markedBlocks))
		{
			int oldSearchLimit = CV_GAME_MANAGER->getPathManager()->getSearchLimit();
			CV_GAME_MANAGER->getPathManager()->setSearchLimit(2);
			bool oldDiagonalMoves = CV_GAME_MANAGER->getPathManager()->getDiagonalMoves();
			CV_GAME_MANAGER->getPathManager()->setDiagonalMoves(false);
			for(markedBlocksIter=markedBlocks.begin(); markedBlocksIter!=markedBlocks.end(); markedBlocksIter++)
			{
				block = *markedBlocksIter;
				if(!block->isMarked())
					continue;
				path.clear();
					if(CV_GAME_MANAGER->getPathManager()->findPath(cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)),block->getLogicalPosition(),&path))
						possBlocks.push_back(block);
			}
			CV_GAME_MANAGER->getPathManager()->setSearchLimit(oldSearchLimit);
			CV_GAME_MANAGER->getPathManager()->setDiagonalMoves(oldDiagonalMoves);
			if(possBlocks.size()>0)
			{
				GLint blockNum = rand()%possBlocks.size();
				currBlock = possBlocks[blockNum];
				path.clear();
				impState = IS_AT_DIGGING_BLOCK;
				return;
			}
		}
	}

	GLvoid CImp::checkNearestForClaiming()
	{
		if(impState != IS_IDLE) return;

		std::vector<CBlock*> unclaimedBlocks,claimedBlocks,possBlocks;
		std::vector<CBlock*>::iterator unclaimedBlocksIter, claimedBlocksIter;
		CBlock *block;

		if(CV_GAME_MANAGER->getLevelManager()->isBlockTypeNear(CV_BLOCK_TYPE_UNCLAIMED_LAND_ID,cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)),true,CV_PLAYER_UNDEFINED,&unclaimedBlocks))
		{
			int oldSearchLimit = CV_GAME_MANAGER->getPathManager()->getSearchLimit();
			CV_GAME_MANAGER->getPathManager()->setSearchLimit(2);
			for(unclaimedBlocksIter=unclaimedBlocks.begin(); unclaimedBlocksIter!=unclaimedBlocks.end(); unclaimedBlocksIter++)
			{
				block = *unclaimedBlocksIter;
				if(block->isTaken())
					continue;
				claimedBlocks.clear();
				if(CV_GAME_MANAGER->getLevelManager()->isBlockClaimable(block->getLogicalPosition(),this->getOwner(),&claimedBlocks))
				{
					path.clear();
					if(CV_GAME_MANAGER->getPathManager()->findPath(cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)),block->getLogicalPosition(),&path))
						possBlocks.push_back(block);
				}
			}
			CV_GAME_MANAGER->getPathManager()->setSearchLimit(oldSearchLimit);
			if(possBlocks.size()>0)
			{
				GLint blockNum = rand()%possBlocks.size();
				path.clear();
				currBlock = possBlocks[blockNum];
				path.push_back(currBlock->getLogicalPosition());
				currBlock->setTaken(true);
				impState = IS_GOING_TO_CLAIMING_DESTINATION;
				return;
			}
		}
	}

	GLvoid CImp::checkNearestForWalling()
	{
		if(impState != IS_IDLE) return;

		std::vector<CBlock*> unfortifiedBlocks,claimedBlocks,possBlocks;
		std::vector<CBlock*>::iterator unfortifiedBlocksIter;
		CBlock *block;

		if(CV_GAME_MANAGER->getLevelManager()->isBlockTypeNear(CV_BLOCK_TYPE_EARTH_ID,cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)),true,CV_PLAYER_UNDEFINED,&unfortifiedBlocks))
		{
			int oldSearchLimit = CV_GAME_MANAGER->getPathManager()->getSearchLimit();
			CV_GAME_MANAGER->getPathManager()->setSearchLimit(2);
			bool oldDiagonalMoves = CV_GAME_MANAGER->getPathManager()->getDiagonalMoves();
			CV_GAME_MANAGER->getPathManager()->setDiagonalMoves(false);
			for(unfortifiedBlocksIter=unfortifiedBlocks.begin(); unfortifiedBlocksIter!=unfortifiedBlocks.end(); unfortifiedBlocksIter++)
			{
				block = *unfortifiedBlocksIter;
				claimedBlocks.clear();
				if(CV_GAME_MANAGER->getLevelManager()->isBlockClaimable(block->getLogicalPosition(),this->getOwner(),&claimedBlocks))
				{
					path.clear();
					if(CV_GAME_MANAGER->getPathManager()->findPath(cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)),block->getLogicalPosition(),&path))
						possBlocks.push_back(block);
				}
			}
			CV_GAME_MANAGER->getPathManager()->setSearchLimit(oldSearchLimit);
			CV_GAME_MANAGER->getPathManager()->setDiagonalMoves(oldDiagonalMoves);
			if(possBlocks.size()>0)
			{
				GLint blockNum = rand()%possBlocks.size();
				currBlock = possBlocks[blockNum];
				path.clear();
				impState = IS_AT_WALLING_BLOCK;
				return;
			}
		}
	}

	GLvoid CImp::checkForDigging()
	{
		if(impState != IS_IDLE) return;
		path.clear();
		currBlock = CV_GAME_MANAGER->getLevelManager()->getMarkedBlock(this->getOwner(),cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)));
		//bool oldEndOnDiagonal = CV_GAME_MANAGER->getPathManager()->getAllowEndDiagonal();
		CV_GAME_MANAGER->getPathManager()->setAllowEndDiagonal(false);
		if (currBlock)
		{
			cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
			if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,currBlock->getLogicalPosition(),&path) && !currBlock->isLow())
				impState = IS_GOING_TO_DIGGING_DESTINATION;
		}
		CV_GAME_MANAGER->getPathManager()->setAllowEndDiagonal(true);
	}

	GLvoid CImp::checkForClaiming()
	{
		if(impState != IS_IDLE) return;
		path.clear();
		currBlock = CV_GAME_MANAGER->getLevelManager()->getUnclaimedBlock(this->getOwner(),cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)));
		if (currBlock)
		{
			cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
			if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,currBlock->getLogicalPosition(),&path))
				impState = IS_GOING_TO_CLAIMING_DESTINATION;
			else
				currBlock->setTaken(false);
		}
	}

	GLvoid CImp::checkForWalling()
	{
		if(impState != IS_IDLE) return;
		path.clear();
		currBlock = CV_GAME_MANAGER->getLevelManager()->getUnfortifiedBlock(this->getOwner(),cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH)));
		//bool oldEndOnDiagonal = CV_GAME_MANAGER->getPathManager()->getAllowEndDiagonal();
		CV_GAME_MANAGER->getPathManager()->setAllowEndDiagonal(false);
		if (currBlock)
		{
			cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
			if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,currBlock->getLogicalPosition(),&path) && !currBlock->isLow())
				impState = IS_GOING_TO_WALLING_DESTINATION;
		}
		CV_GAME_MANAGER->getPathManager()->setAllowEndDiagonal(true);
	}

	GLvoid CImp::faceBlock(CBlock *block)
	{
		rotation[1] = 90.0f-(float)(atan2(block->getLogicalPosition()[1]-floor(position[2]/CV_BLOCK_WIDTH),block->getLogicalPosition()[0]-floor(position[0]/CV_BLOCK_WIDTH))*180.0f/M_PI);
	}

	GLvoid CImp::walkPath(GLfloat deltaTime)
	{
		if(path.size() == 0)
		{
			if(impState == IS_GOING_TO_DEPOSITING_GOLD_DESTINATION)
				impState = IS_AT_DEPOSITING_GOLD;
			else if(impState == IS_GOING_TO_CLAIMING_DESTINATION)
				impState = IS_AT_CLAIMING_BLOCK;
			else if (impState == IS_GOING_TO_DIGGING_DESTINATION)
				impState = IS_AT_DIGGING_BLOCK;
			else if (impState == IS_GOING_TO_WALLING_DESTINATION)
				impState = IS_AT_WALLING_BLOCK;
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

	GLvoid CImp::update(GLfloat deltaTime)
	{
		if (impState == IS_IDLE)
		{
			Idle(deltaTime);
			//check for next space digging
			//check for next space claiming
			//check for next space walling

			//check for digging
			//check for claiming
			//check for walling

			checkNearestForDigging();
			checkNearestForClaiming();
			checkNearestForWalling();

			checkForDigging();
			checkForClaiming();
			checkForWalling();
		} else if (impState == IS_GOING_TO_DEPOSITING_GOLD_DESTINATION)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);		
		} else if (impState == IS_GOING_TO_DIGGING_DESTINATION)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		} else if (impState == IS_GOING_TO_CLAIMING_DESTINATION)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		} else if (impState == IS_GOING_TO_WALLING_DESTINATION)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		} else if (impState == IS_AT_DEPOSITING_GOLD)
		{
			impState = IS_DEPOSITING_GOLD;
		} else if (impState == IS_AT_DIGGING_BLOCK)
		{
			faceBlock(currBlock);
			impState = IS_DIGGING;
			useAction(AA_DIG);
		} else if (impState == IS_AT_CLAIMING_BLOCK)
		{
			impState = IS_CLAIMING;
			useAction(AA_CLAIM);
		} else if (impState == IS_AT_WALLING_BLOCK)
		{
			faceBlock(currBlock);
			impState = IS_WALLING;
			useAction(AA_CLAIM);
		} else if (impState == IS_DEPOSITING_GOLD)
		{
			//Todo: if a block has a 250 gold, upp it to 500...
			bool found = false;
			for (std::vector<block_objects::CBlockObject*>::iterator rmIter = currBlock->getBlockObjects()->begin(); rmIter != currBlock->getBlockObjects()->end(); rmIter++)
			{
				block_objects::CBlockObject *bObject = *rmIter;

				if (bObject->getName() == "MODEL_GOLD250")
					found = true;

			}
			if(!found)
			{
				currBlock->addModel("MODEL_GOLD250",position);
				PLAYER0_MONEY = PLAYER0_MONEY + this->getGold();//do for other teams
				this->setGold(0);	
				impState = IS_IDLE;
				useAction(AA_IDLE);
			}
			else
			{
				checkGoldLevels();
			}
		} else if (impState == IS_DIGGING)
		{
			if(currBlock->isLow() || !currBlock->isMarked())
			{
				impState = IS_IDLE;
				useAction(AA_IDLE);
				return;
			}
			if(currBlock->getType() == CV_BLOCK_TYPE_GOLD_ID)
				//TODO: use a new formula
				this->setGold(this->getGold() + 1);
			currBlock->decLife(deltaTime*moveSpeed);
			if (currBlock->getLife()<=0.0f)
			{
				currBlock->digBlock();

				impState = IS_IDLE;
				useAction(AA_IDLE);
			}
			//Check if you have gold :)
			checkGoldLevels();
		} else if (impState == IS_CLAIMING)
		{
			currBlock->decLife(deltaTime*moveSpeed);
			if (currBlock->getLife()<=0.0f)
			{
				currBlock->claimBlock(this->getOwner());
				impState = IS_IDLE;
				useAction(AA_IDLE);
			}
		} else if (impState == IS_WALLING)
		{
			if(currBlock->isLow() || currBlock->isMarked())
			{
				impState = IS_IDLE;
				useAction(AA_IDLE);
				return;
			}
			currBlock->addLife(deltaTime*moveSpeed);
			if (currBlock->getLife()>=9.0f)
			{
				currBlock->fortifyBlock(this->getOwner());

				impState = IS_IDLE;
				useAction(AA_IDLE);
			}
		}
	}
};