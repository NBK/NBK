#include "commons.h"
#include "Chicken.h"
#include <math.h>

using namespace game_utils;

namespace game_objects
{
	CChicken::CChicken(): CCreature()
	{
		m_MoveSpeed = 0.00025f;
	}

	CChicken::~CChicken()
	{
	}

	GLvoid CChicken::Idle(GLfloat deltaTime)
	{
		if (mCount>=mChange)
		{
			GLfloat tX = (GLfloat)(rand()%100-50);
			GLfloat tZ = (GLfloat)(rand()%100-50);
			mMoveVector[0] = tX-mPosition[0];
			mMoveVector[2] = tZ-mPosition[2];
			mMoveVector.normalize();
			mRotation[1] = 90.0f+(float)(atan2(mMoveVector[2],mMoveVector[0])*180.0f/M_PI);
			mChange=(GLfloat)((rand()%100))+300.0f;
			mCount = 0.0f;
		}

		GLint X = (GLint)(mPosition[0]/CV_BLOCK_WIDTH);
		GLint Y = (GLint)(mPosition[2]/CV_BLOCK_DEPTH);
		if(CV_GAME_MANAGER->getLevelManager()->getBlock(X,Y)->isWalkable(false) && CV_GAME_MANAGER->getLevelManager()->getBlock(X,Y)->getType()==CV_BLOCK_TYPE_HATCHERY_ID)
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

	GLvoid CChicken::update(GLfloat deltaTime)
	{
		Idle(deltaTime);
	}
};