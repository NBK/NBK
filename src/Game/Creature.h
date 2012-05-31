#ifndef CREATURE_H
#define CREATURE_H

#include "BR5Model.h"
#include "Entity.h"

namespace game_objects
{
	class CCreature: public CEntity
	{
	public:
		CCreature();
		~CCreature();		

		enum ACTION_ANIMATIONS
		{
			AA_IDLE = 0,
			AA_WALK,
			AA_DIG,
			AA_CLAIM,
			AA_DRAG,
			AA_ATTACK1,
			AA_SLEEP,
			AA_EAT,

			// TODO, add the rest
		};

		enum CREATURE_STATE
		{
			IS_NONE,
			IS_IDLE,
			IS_GOING_TO_EAT,
			IS_AT_EATING,
			IS_EATING,
			IS_GOING_TO_MAKE_LAIR,
			IS_AT_MAKING_LAIR,
			IS_MAKING_LAIR,
			IS_GOING_TO_SLEEPING,
			IS_AT_SLEEPING,
			IS_SLEEPING,
			IS_GOING_TO_TRAIN,
			IS_AT_TRAINING,
			IS_TRAINING,

			// utility enums
			IS_ROTATING
		};

		GLvoid setName(std::string name);
		GLvoid setOwner(GLubyte owner);
		GLvoid setLevel(GLint level);
		GLvoid setGold(GLint gold);
		GLvoid addCurrentXP(GLint CurrentXP);
		GLvoid setModel(loaders::CBR5Model *model);
		GLvoid setAction(GLint action, GLint startFrame, GLint endFrame);
		GLvoid useAction(GLint action);
		GLvoid Idle(GLfloat deltaTime);
		GLvoid draw(GLfloat deltaTime);
		GLvoid walkPath(GLfloat deltaTime);
		GLvoid faceBlock(CBlock *block);
		virtual GLvoid update(GLfloat deltaTime);
		CREATURE_STATE creatureState;
		CBlock *currBlock;

		loaders::CBR5Model	*getModel();
		std::string			getName();
		GLubyte				getOwner();
		GLint				getLevel();
		GLint				getCurrentXP();
		GLint				getGold();

	protected:

		std::string			name;

		loaders::CBR5Model	*model;

		GLint				currentAction;

		// mapping for action -> model action 
		std::map<GLint, GLint> actions;

		// creature params, TODO add more
		GLfloat			moveSpeed;
		GLfloat			count, change;
		cml::vector3f	moveVector;
		cml::vector2f	lair;

		// Creature stats
		GLubyte			owner;
		GLint			level;
		GLint			CurrentXP;
		GLint			gold;
		GLint			hunger;
		GLint			health;
		GLint			sleep;

		/* holds the current path */
		std::vector<cml::vector2i> path;
		
	};
};

#endif // CREATURE_H