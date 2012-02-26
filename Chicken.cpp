#include "commons.h"
#include "Chicken.h"
#include <math.h>

using namespace game_utils;

namespace game_objects
{
	CChicken::CChicken(): CCreature()
	{
		moveSpeed = 0.00025f;
	}

	CChicken::~CChicken()
	{
	}

	GLvoid CChicken::Idle(GLfloat deltaTime)
	{
		if (count>=change)
		{
			GLfloat tX = (GLfloat)(rand()%100-50);
			GLfloat tZ = (GLfloat)(rand()%100-50);
			moveVector[0] = tX-position[0];
			moveVector[2] = tZ-position[2];
			moveVector.normalize();
			rotation[1] = 90.0f+(float)(atan2(moveVector[2],moveVector[0])*180.0f/M_PI);
			change=(GLfloat)((rand()%100))+300.0f;
			count = 0.0f;
		}

		GLint X = (GLint)(position[0]/CV_BLOCK_WIDTH);
		GLint Y = (GLint)(position[2]/CV_BLOCK_DEPTH);
		if(CV_GAME_MANAGER->getLevelManager()->getBlock(X,Y)->isWalkable(false) && CV_GAME_MANAGER->getLevelManager()->getBlock(X,Y)->getType()==CV_BLOCK_TYPE_HATCHERY_ID)
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

	GLvoid CChicken::update(GLfloat deltaTime)
	{
		Idle(deltaTime);
	}
};