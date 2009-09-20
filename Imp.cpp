#include "Imp.h"

namespace game_objects
{
	CImp::CImp(): CCreature()
	{

	}

	CImp::~CImp()
	{
	}

	GLvoid CImp::update(GLfloat deltaTime)
	{
		// tmp update pos
		moveVector[2] = 1.0f;
		moveSpeed = 0.0001f;
		position += moveVector*moveSpeed*deltaTime;
	}
};