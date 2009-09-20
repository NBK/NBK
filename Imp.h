#ifndef IMP_H
#define IMP_H

#include "Creature.h"

/*
	Imp differs from other creatures which can be more or less generic. Imp
	has it's own inteligence for having to do very specific tasks.
*/

namespace game_objects
{
	class CImp: public CCreature
	{
	public:
		CImp();
		~CImp();

		virtual GLvoid update(GLfloat deltaTime);
	};
};

#endif // IMP_H