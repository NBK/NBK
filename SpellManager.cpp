#include "commons.h"
#include "SpellManager.h"

using namespace game_objects;
using namespace std;
using namespace cml;

namespace game_utils
{
	namespace managers
	{
		CSpellManager::CSpellManager(): CManager()
		{
		}

		CSpellManager::~CSpellManager()
		{

		}
		
		bool CSpellManager::init()
		{
			return true;
		}

		bool CSpellManager::update()
		{
			return true;
		}

		bool CSpellManager::shutdown()
		{
			return true;
		}

		GLvoid CSpellManager::castSpell(GLint spell, cml::vector3f position, GLubyte owner)
		{
			if(spell == SPELL_BUTTON_SUMMON_IMP)
				CV_GAME_MANAGER->getCreatureManager()->addCreature("IMP",position,owner);
		}
	};
};