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

		enum IMP_STATE
		{
			IS_NONE,
			IS_IDLE,
			IS_GOING_TO_CLAIMING_DESTINATION,
			IS_AT_CLAIMING_BLOCK, 
			IS_CLAIMING,
			IS_GOING_TO_DIGGING_DESTINATION,
			IS_AT_DIGGING_BLOCK,
			IS_DIGGING,
			IS_GOING_TO_WALLING_DESTINATION,
			IS_AT_WALLING_BLOCK,
			IS_WALLING,

			// utility enums
			IS_ROTATING
		};
	private:
		GLvoid checkNearestForDigging();
		GLvoid checkNearestForClaiming();
		GLvoid checkNearestForWalling();
		GLvoid checkForDigging();
		GLvoid checkForClaiming();
		GLvoid checkForWalling();
		GLvoid walkPath(GLfloat deltaTime);
		IMP_STATE impState;
	};
};

#endif // IMP_H