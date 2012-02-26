#ifndef CHICKEN_H
#define CHICKEN_H

#include "Creature.h"

/*
	Chickens differs from other creatures which can be more or less generic. Chicken
	has it's own inteligence for having to do very basic/different tasks.
*/

namespace game_objects
{
	class CChicken: public CCreature
	{
	public:
		CChicken();
		~CChicken();
	private:
		GLvoid Idle(GLfloat deltaTime);

		virtual GLvoid update(GLfloat deltaTime);
	};
};

#endif // CHICKEN_H