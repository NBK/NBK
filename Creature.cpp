#include "commons.h"
#include "Creature.h"

using namespace game_utils;
using namespace std;
using namespace loaders;
using namespace cml;

namespace game_objects
{
	CCreature::CCreature(): CEntity()
	{
		owner = CV_PLAYER_UNDEFINED;
		currentAction = AA_WALK;
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
		this->currentAction = action;

		if (model)
		{
			model->doAction(actions[action]);
		}
	}

	GLvoid CCreature::walkPath(GLfloat deltaTime)
	{
		if(path.size() == 0)
		{
			if(creatureState == IS_GOING_TO_EAT)
				creatureState = IS_EATING;
			if(creatureState == IS_GOING_TO_MAKE_LAIR)
				creatureState = IS_MAKING_LAIR;
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
			Idle(deltaTime);
			
			//HAVE I GOT A LAIR??!?!?!
			if(lair[0] == 0 && lair[1] == 0)
			{
				path.clear();
				CBlock *destBlock;
				destBlock = CV_GAME_MANAGER->getRoomManager()->getRoom(CV_BLOCK_TYPE_LAIR_ID, this->getOwner());
				if (destBlock)
				{
					cml::vector2i currPos = cml::vector2i((int)floor(position[0]/CV_BLOCK_WIDTH),(int)floor(position[2]/CV_BLOCK_DEPTH));
					if(CV_GAME_MANAGER->getPathManager()->findPath(currPos,destBlock->getLogicalPosition(),&path))
					{
						creatureState = IS_GOING_TO_MAKE_LAIR;
						return;
					}
				}				
			}

			//TODO SEPERATE FUNCTION CHECK IF HUNGRY
			if(hunger < 21)
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
		}
		else if (creatureState == IS_GOING_TO_EAT)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (creatureState == IS_EATING)
		{
			//todo pick up chicken and eat ect..
			creatureState = IS_IDLE;
			hunger = 100;
			count=change;
		}
		else if (creatureState == IS_GOING_TO_MAKE_LAIR)
		{
			useAction(AA_WALK);
			walkPath(deltaTime);
		}
		else if (creatureState == IS_MAKING_LAIR)
		{
			//add the lair on the floor, meh a temp model
			CV_GAME_MANAGER->getLevelManager()->getBlock((int)floor(position[0]/CV_BLOCK_WIDTH), (int)floor(position[2]/CV_BLOCK_DEPTH))->addModel("MODEL_GRAVE",position);
			lair[0] = position[0]/CV_BLOCK_WIDTH;
			lair[1] = position[2]/CV_BLOCK_DEPTH;
			creatureState = IS_IDLE;
			count=change;
			CV_GAME_MANAGER->getConsole()->writeLine("Lair has been made");
		}	
	}
};