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
		virtual ~CImp();

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
			IS_GOING_TO_DEPOSITING_GOLD_DESTINATION,
			IS_AT_DEPOSITING_GOLD,
			IS_DEPOSITING_GOLD,			

			// utility enums
			IS_ROTATING
		};
	private:
		void checkGoldLevels();
		void checkNearestForDigging();
		void checkNearestForClaiming();
		void checkNearestForWalling();
		void checkForDigging();
		void checkForClaiming();
		void checkForWalling();

		void depositGold();
		void walkPath(float deltaTime);
		void faceBlock(CBlock *block);

	private:
		IMP_STATE m_impState;
		CBlock *mCurrentBlock;
	protected:
		//max gold creature can carry
		GLint m_maxgold;
	};
};

#endif // IMP_H