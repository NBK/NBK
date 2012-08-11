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
			virtual ~CSpellManager();

			// from CManager
			virtual bool init();
			virtual bool update();	
			virtual bool shutdown();

			GLvoid castSpell(GLint spell, cml::vector2f position, GLubyte owner);

		private:

		};
	};
};

#endif // CREATURE_MANAGER_H