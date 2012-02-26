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

		GLvoid CSpellManager::castSpell(GLint spell, cml::vector2f position, GLubyte owner)
		{
			CBlock *block = CV_GAME_MANAGER->getLevelManager()->getBlock(position[0],position[1]);
			cml::vector3f realposition = block->getRealPosition();

			if(spell == SPELL_BUTTON_SUMMON_IMP)
			{
				// Check that the spell is being casted on land you own.
				if(block->getOwner() == owner)
					CV_GAME_MANAGER->getCreatureManager()->addCreature("IMP",cml::vector3f(realposition[0],CV_BLOCK_HEIGHT/4.0f,realposition[2]),owner);
			}
			else if(spell == SPELL_BUTTON_DESTROY_WALL)
			{
				// dont do this on the edge of the map. TODO: fix this for rock which is not on the edge of the map.
				if(block->getType() == CV_BLOCK_TYPE_ROCK_ID)
					return;

				// Make all fortified walls in a 1 block radius of where you clicked, unfortified.
				for (GLint y=position[1]-1; y<=position[1]+1; y++)
				{
					for (GLint x=position[0]-1; x<=position[0]+1; x++)
					{
						if(CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->getType() == CV_BLOCK_TYPE_WALL_ID)
						{
							CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->setOwner(CV_PLAYER_UNDEFINED);
							CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->setType(CV_BLOCK_TYPE_EARTH_ID);
							CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->init();
							CV_GAME_MANAGER->getLevelManager()->getBlock(x,y)->finalize();
						}
					}
				}
			}
			else if(spell = SPELL_BUTTON_CHICKEN)
			{
				// Check that the spell is being casted on land you own in a hatchery
				if(block->getOwner() == owner && block->getType()==CV_BLOCK_TYPE_HATCHERY_ID)
					CV_GAME_MANAGER->getCreatureManager()->addCreature("CHICKEN",cml::vector3f(realposition[0],CV_BLOCK_HEIGHT/4.0f,realposition[2]),owner);
			}
			else // Spell not implanted
			{
				CV_GAME_MANAGER->getConsole()->writeLine("NOSPELL");
			}

		}
	};
};