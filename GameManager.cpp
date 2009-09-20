#include "commons.h"
#include "GameManager.h"
#include "Logger.h"
#include "GUI/DKTextureList.h"

using namespace std;
using namespace control;
using namespace utils;

lists::CTextureList	*game_utils::CV_TEXTURE_LIST;
cml::vector3f		game_utils::CV_CAMERA_OLD_POSITION;
GLint				game_utils::CV_CURRENT_PLAYER_ID;
CDKTextureList		*GLOBAL_TEXTURE_LIST;

namespace game_utils
{
	namespace managers
	{
		CGameManager::CGameManager(): CManager()
		{
			CV_TEXTURE_LIST	= NULL;
			settingsManager	= NULL;
			resourceManager	= NULL;
			blockManager	= NULL;
			levelManager	= NULL;
			renderManager	= NULL;
			collisionManager= NULL;
			controlManager	= NULL;
			animatedTerrainManager	= NULL;
			lightingManager	= NULL;
			roomManager		= NULL;
			pickingManager	= NULL;
			economyManager	= NULL;
			creatureManager	= NULL;
			guiManager		= NULL;
			deltaTime		= NULL;
			textPrinter		= NULL;
			console			= NULL;

			// should be changed when entering multiplayer
			CV_CURRENT_PLAYER_ID = CV_PLAYER_0;
		}

		CGameManager::~CGameManager()
		{
		}

		bool CGameManager::init()
		{
			CLogger::setEntryStart();		

			console = new CConsole();

			deltaTime = new CDeltaTime();
			textPrinter = new CText();
			textPrinter->setBlend(true);
			textPrinter->setCharacterSize(12);
			textPrinter->setColor(1.0f,1.0f,1.0f);

			CV_TEXTURE_LIST = new lists::CTextureList();			

			bool result = true;

			CLogger::setEntryStart();
			settingsManager = new CSettingsManager();
			result&=settingsManager->init();			
			CLogger::setEntryEnd("Settings manager creation and init.");

			CLogger::setEntryStart();
			resourceManager = new CResourceManager();
			result&=resourceManager->init();			
			CLogger::setEntryEnd("Resource manager creation and init.");

			CLogger::setEntryStart();
			creatureManager = new CCreatureManager();
			result&=creatureManager->init();			
			CLogger::setEntryEnd("Creature manager creation and init.");

			CLogger::setEntryStart();
			blockManager = new CBlockManager();
			result&=blockManager->init();
			CLogger::setEntryEnd("Block manager creation and init.");

			CLogger::setEntryStart();
			lightingManager = new CLightingManager();
			result&=lightingManager->init();
			CLogger::setEntryEnd("Lighting manager creation and init.");

			CLogger::setEntryStart();
			levelManager = new CLevelManager();
			result&=levelManager->init();
			CLogger::setEntryEnd("Level manager creation and init.");			

			CLogger::setEntryStart();
			collisionManager = new CCollisionManager();
			result&=collisionManager->init();
			CLogger::setEntryEnd("Collision manager creation and init.");

			CLogger::setEntryStart();
			controlManager = new CControlManager();
			result&=controlManager->init();
			CLogger::setEntryEnd("Control manager creation and init.");

			CLogger::setEntryStart();
			renderManager = new CRenderManager();
			//renderManager = new CRenderManager1();
			result&=renderManager->init();
			CLogger::setEntryEnd("Render manager creation and init.");

			CLogger::setEntryStart();
			animatedTerrainManager = new CAnimatedTerrainManager();
			result&=animatedTerrainManager->init();
			CLogger::setEntryEnd("Animated terrain manager creation and init.");			

			CLogger::setEntryStart();
			roomManager = new CRoomManager();
			result&=roomManager->init();
			CLogger::setEntryEnd("Room manager creation and init.");			

			CLogger::setEntryStart();
			pickingManager = new CPickingManager();
			result&=pickingManager->init();
			CLogger::setEntryEnd("Picking manager creation and init.");			

			CLogger::setEntryStart();
			economyManager = new CEconomyManager();
			result&=economyManager->init();
			CLogger::setEntryEnd("Economy manager creation and init.");			

			CLogger::setEntryStart();
			guiManager = new CGUIManager();
			result&=guiManager->init();
			CLogger::setEntryEnd("GUI manager (wraper) creation and init.");			

			console->init();

			CLogger::setEntryEnd("Game managers (all above) init.");						

			return result;
		}

		bool CGameManager::update()
		{
			bool result = true;

			deltaTime->beforeUpdate();

			result&=settingsManager->update();			
			result&=resourceManager->update();			
			result&=creatureManager->update();	
			result&=blockManager->update();
			result&=levelManager->update();

			// usefull for calculating terrain-camera collisions
			CV_CAMERA_OLD_POSITION = controlManager->getCamera()->getPosition();

			result&=controlManager->update();
			result&=collisionManager->update();

			// picking manager uses color picking and must be called prior to render manager
			result&=pickingManager->update();

			result&=renderManager->update();
			result&=animatedTerrainManager->update();
			result&=lightingManager->update();
			result&=roomManager->update();
			result&=guiManager->update();	
			result&=economyManager->update();						

			deltaTime->afterUpdate();

			console->update();

			return result;
		}

		bool CGameManager::shutdown()
		{
			bool result = true;

			result&=settingsManager->shutdown();
			result&=resourceManager->shutdown();
			result&=creatureManager->shutdown();
			result&=roomManager->shutdown();
			result&=blockManager->shutdown();
			result&=levelManager->shutdown();
			result&=renderManager->shutdown();
			result&=collisionManager->shutdown();
			result&=controlManager->shutdown();
			result&=animatedTerrainManager->shutdown();
			result&=lightingManager->shutdown();			
			result&=pickingManager->shutdown();			
			result&=economyManager->shutdown();			
			result&=guiManager->shutdown();			

			delete CV_TEXTURE_LIST;
			delete settingsManager;
			delete resourceManager;
			delete blockManager;
			delete levelManager;
			delete renderManager;
			delete collisionManager;
			delete controlManager;
			delete animatedTerrainManager;
			delete lightingManager;
			delete roomManager;
			delete pickingManager;
			delete economyManager;
			delete creatureManager;
			delete guiManager;
			delete deltaTime;
			delete textPrinter;
			delete console;			

			return result;
		}

		CResourceManager *CGameManager::getResourceManager()
		{
			return resourceManager;
		}

		CBlockManager *CGameManager::getBlockManager()
		{
			return blockManager;
		}

		CLevelManager *CGameManager::getLevelManager()
		{
			return levelManager;
		}

		CRenderManager *CGameManager::getRenderManager()
		{
			return renderManager;
		}

		CControlManager *CGameManager::getControlManager()
		{
			return controlManager;
		}

		CCollisionManager *CGameManager::getCollisionManager()
		{
			return collisionManager;
		}

		CSettingsManager *CGameManager::getSettingsManager()
		{
			return settingsManager;
		}

		CDeltaTime *CGameManager::getDeltaTimeObject()
		{
			return deltaTime;
		}

		GLfloat	CGameManager::getDeltaTime()
		{
			return deltaTime->getDelta();
		}

		CAnimatedTerrainManager	*CGameManager::getAnimatedTerrainManager()
		{
			return animatedTerrainManager;
		}

		CLightingManager *CGameManager::getLightingManager()
		{
			return lightingManager;
		}

		CRoomManager *CGameManager::getRoomManager()
		{
			return roomManager;
		}

		CPickingManager *CGameManager::getPickingManager()
		{
			return pickingManager;
		}

		CEconomyManager	*CGameManager::getEconomyManager()
		{
			return economyManager;
		}

		CCreatureManager *CGameManager::getCreatureManager()
		{
			return creatureManager;
		}

		CGUIManager *CGameManager::getGUIManager()
		{
			return guiManager;
		}

		CText *CGameManager::getTextPrinter()
		{
			return textPrinter;
		}

		CConsole *CGameManager::getConsole()
		{
			return console;
		}		
	};
};