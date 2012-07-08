#include "commons.h"
#include "CreatureManager.h"
#include "Imp.h"
#include "Chicken.h"

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

			CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->updateCreatureInfo();

			return true;
		}

		bool CCreatureManager::shutdown()
		{
			return true;
		}

		GLvoid CCreatureManager::addCreature(string creatureName, vector3f position, GLubyte owner)
		{
			if (creatureName == "CHICKEN")
 			{
				CChicken *chicken = new CChicken();
				chicken->setPosition(position);
				chicken->setName(creatureName);
				chicken->setOwner(owner);
				chicken->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_CHICKEN"));
				allCreatures.push_back(chicken);
			}
			else if (creatureName == "BARBARIAN")
 			{
				CCreature *barbarian = new CCreature();
				barbarian->setPosition(position);
				barbarian->setName(creatureName);
				barbarian->setOwner(owner);
				barbarian->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_BARBARIAN"));
				allCreatures.push_back(barbarian);
			}
			else if (creatureName == "KNIGHT")
 			{
				CCreature *knight = new CCreature();
				knight->setPosition(position);
				knight->setName(creatureName);
				knight->setOwner(owner);
				knight->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_KNIGHT"));
				allCreatures.push_back(knight);
			}
			else if (creatureName == "HORNED_REAPER")
			{
				CCreature *horned_reaper = new CCreature();
				horned_reaper->setPosition(position);
				horned_reaper->setName(creatureName);
				horned_reaper->setOwner(owner);
				horned_reaper->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_HORNED_REAPER"));
				allCreatures.push_back(horned_reaper);
			}
			else if (creatureName == "DRAGON")
			{
				CCreature *dragon = new CCreature();
				dragon->setPosition(position);
				dragon->setName(creatureName);
				dragon->setOwner(owner);
				dragon->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_DRAGON"));
				allCreatures.push_back(dragon);
			}
			else if (creatureName == "DEMON_SPAWN")
			{
				CCreature *demon_spawn = new CCreature();
				demon_spawn->setPosition(position);
				demon_spawn->setName(creatureName);
				demon_spawn->setOwner(owner);
				demon_spawn->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_DEMON_SPAWN"));
				allCreatures.push_back(demon_spawn);
			}
			else if (creatureName == "FLY")
			{
				CCreature *fly = new CCreature();
				fly->setPosition(position);
				fly->setName(creatureName);
				fly->setOwner(owner);
				fly->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_FLY"));
				allCreatures.push_back(fly);
			}
			else if (creatureName == "BILE_DEMON")
			{
				CCreature *bile_demon = new CCreature();
				bile_demon->setPosition(position);
				bile_demon->setName(creatureName);
				bile_demon->setOwner(owner);
				bile_demon->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_BILE_DEMON"));
				bile_demon->setAction(CCreature::AA_WALK,0,29);
				bile_demon->setAction(CCreature::AA_IDLE,30,59);
				bile_demon->setAction(CCreature::AA_ATTACK1,60,119);
				//bile_demon->setAction(CCreature::AA_GO_TO_SLEEP,120,149);
				//bile_demon->setAction(CCreature::AA_CHEER,150,179);
				bile_demon->setAction(CCreature::AA_EAT,180,209);
				//bile_demon->setAction(CCreature::AA_FLY,210,239);
				bile_demon->setAction(CCreature::AA_SLEEP,240,269);
				//bile_demon->setAction(CCreature::AA_YAWN,270,299);
				allCreatures.push_back(bile_demon);
			}
			else if (creatureName == "IMP")
			{
				CImp *imp = new CImp();

				imp->setPosition(position);
				imp->setName(creatureName);
				imp->setOwner(owner);
				imp->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_IMP"));
				imp->setAction(CCreature::AA_IDLE,10,14);
				imp->setAction(CCreature::AA_WALK,5,9);
				imp->setAction(CCreature::AA_DIG,15,19);
				imp->setAction(CCreature::AA_CLAIM,0,4);
				imp->setAction(CCreature::AA_DRAG,25,29);
				imp->useAction(CCreature::AA_WALK);
				allCreatures.push_back(imp);
			}
			else if (creatureName == "BEETLE")
			{
				CCreature *beetle = new CCreature();
				beetle->setPosition(position);
				beetle->setName(creatureName);
				beetle->setOwner(owner);
				beetle->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_BEETLE"));
				allCreatures.push_back(beetle);
			}
			else if (creatureName == "VAMPIRE")
			{
				CCreature *vampire = new CCreature();
				vampire->setPosition(position);
				vampire->setName(creatureName);
				vampire->setOwner(owner);
				vampire->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_VAMPIRE"));
				allCreatures.push_back(vampire);
			}
			else if (creatureName == "SPIDER")
			{
				CCreature *spider = new CCreature();
				spider->setPosition(position);
				spider->setName(creatureName);
				spider->setOwner(owner);
				spider->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_SPIDER"));
				allCreatures.push_back(spider);
			}
			else if (creatureName == "HELL_HOUND")
			{
				CCreature *hell_hound = new CCreature();
				hell_hound->setPosition(position);
				hell_hound->setName(creatureName);
				hell_hound->setOwner(owner);
				hell_hound->setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_HELL_HOUND"));
				allCreatures.push_back(hell_hound);
			}
		}

		GLint CCreatureManager::getCreatureCount(GLint owner)
		{
			GLint count = 0;

			for (cIter=allCreatures.begin(); cIter!=allCreatures.end(); cIter++)
			{
				CCreature *creature = (*cIter);

				// ignore imps
				count+=(creature->getOwner() == owner && !(creature->getName() == "IMP"))?1:0;
			}

			return count;
		}

		std::vector<CCreature*> *CCreatureManager::getCreatureVector()
		{
			return &allCreatures;
		}
	};
};