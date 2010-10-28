#ifndef CREATURE_MANAGER_H
#define CREATURE_MANAGER_H

#include "Creature.h"
#include "Manager.h"

namespace game_utils
{
	namespace managers
	{
		class CCreatureManager: public CManager
		{
		public:
			CCreatureManager();
			~CCreatureManager();

			// from CManager
			virtual bool init();
			virtual bool update();	
			virtual bool shutdown();

			GLvoid addCreature(std::string creatureName, cml::vector3f position, GLubyte owner);

			GLint getCreatureCount(GLint owner);

			std::vector<game_objects::CCreature*> *getCreatureVector();

		private:

			std::vector<game_objects::CCreature*>			allCreatures;
			std::vector<game_objects::CCreature*>::iterator	cIter;
		};
	};
};

#endif // CREATURE_MANAGER_H