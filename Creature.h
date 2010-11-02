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
			AA_DRAG
			// TODO, add the rest
		};

		GLvoid setName(std::string name);
		GLvoid setOwner(GLubyte owner);
		GLvoid setLevel(GLint level);
		GLvoid setGold(GLint gold);
		GLvoid addCurrentXP(GLint CurrentXP);
		GLvoid setModel(loaders::CBR5Model *model);
		GLvoid setAction(GLint action, GLint startFrame, GLint endFrame);
		GLvoid useAction(GLint action);
		GLvoid draw(GLfloat deltaTime);
		virtual GLvoid update(GLfloat deltaTime);

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
		cml::vector3f	moveVector;

		// Creature stats
		GLubyte			owner;
		GLint			level;
		GLint			CurrentXP;
		GLint			gold;

		/* holds the current path */
		std::vector<cml::vector2i> path;
		
	};
};

#endif // CREATURE_H