#include "commons.h"
#include "CreatureManager.h"
#include "Imp.h"

using namespace game_objects;
using namespace std;
using namespace cml;

namespace game_utils
{
	namespace managers
	{
		CCreatureManager::CCreatureManager(): CManager()
		{
		}

		CCreatureManager::~CCreatureManager()
		{
			allCreatures.clear();
		}
		
		bool CCreatureManager::init()
		{
			return true;
		}

		bool CCreatureManager::update()
		{
			GLfloat deltaTime = CV_GAME_MANAGER->getDeltaTime();
			for (cIter = allCreatures.begin(); cIter != allCreatures.end(); cIter++)
			{
				(*cIter)->update(deltaTime);
			}
			return true;
		}

		bool CCreatureManager::shutdown()
		{
			return true;
		}

		GLvoid CCreatureManager::addCreature(string creatureName, vector3f position)
		{
			/*CCreature *newCreature = new CCreature();			
			newCreature->setPosition(position);
			newCreature->setName("MODEL_BEAST");
			newCreature->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_BEAST"));
			newCreature->setAction(CCreature::AA_WALK,0,5);
			newCreature->useAction(CCreature::AA_WALK);
			allCreatures.push_back(newCreature);*/

			if (creatureName == "IMP")
			{
				CImp *imp = new CImp();

				imp->setPosition(position);
				imp->setName(creatureName);
				imp->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_IMP"));
				imp->setAction(CCreature::AA_IDLE,10,14);
				imp->setAction(CCreature::AA_WALK,5,9);
				imp->setAction(CCreature::AA_DIG,15,19);
				imp->setAction(CCreature::AA_CLAIM,0,4);
				imp->setAction(CCreature::AA_DRAG,25,29);
				imp->useAction(CCreature::AA_WALK);
				allCreatures.push_back(imp);
			}
		}

		std::vector<CCreature*> *CCreatureManager::getCreatureVector()
		{
			return &allCreatures;
		}
	};
};