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
		virtual ~CCreature();

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

		void setName(std::string name);
		void setOwner(GLubyte owner);
		void setLevel(GLint level);
		void setGold(GLint gold);
		void addCurrentXP(GLint CurrentXP);
		void setModel(loaders::CBR5Model *model);
		void setAction(GLint action, GLint startFrame, GLint endFrame);
		void useAction(GLint action);
		void Idle(GLfloat deltaTime);
		void draw(GLfloat deltaTime);
		void walkPath(GLfloat deltaTime);
		void faceBlock(CBlock *block);
		virtual void update(GLfloat deltaTime);
		
		CREATURE_STATE mCreatureState;
		CBlock *mCurrentBlock;

		loaders::CBR5Model*	getModel();
		std::string			getName();
		unsigned char		getOwner();
		int					getLevel();
		int					getCurrentXP();
		int					getGold();

	protected:

		std::string			mName;

		loaders::CBR5Model	*mModel;

		int				mCurrentAction;

		// mapping for action -> model action 
		std::map<int, int> actions;

		// creature params, TODO add more
		float			mMoveSpeed;
		float			mCount; // lol count what?
		float			mChange;
		cml::vector3f	mMoveVector;
		cml::vector2f	mLair;

		// Creature stats
		GLubyte			mOwner;
		int			mLevel;
		int			mCurrentXP;
		int			mGold;
		float		mHunger;
		float		mHealth;
		float			mSleep;

		/* holds the current path */
		std::vector<cml::vector2i> path;
	};
};

#endif // CREATURE_H