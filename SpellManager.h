#ifndef SPELL_MANAGER_H
#define SPELL_MANAGER_H

#include "Manager.h"

namespace game_utils
{
	namespace managers
	{
		class CSpellManager: public CManager
		{
		public:
			CSpellManager();
			~CSpellManager();

			// from CManager
			virtual bool init();
			virtual bool update();	
			virtual bool shutdown();

			GLvoid CSpellManager::castSpell(GLint spell, cml::vector3f position, GLubyte owner);

		private:

		};
	};
};

#endif // CREATURE_MANAGER_H