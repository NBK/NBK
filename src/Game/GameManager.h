#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "RenderManager.h"
#include "RenderManager1.h"
#include "NewRenderer.h"
#include "ResourceManager.h"
#include "BlockManager.h"
#include "Manager.h"
#include "LevelManager.h"
#include "AnimatedTerrainManager.h"
#include "ControlManager.h"
#include "CollisionManager.h"
#include "SettingsManager.h"
#include "LightingManager.h"
#include "DeltaTime.h"
#include "Text.h"
#include "Console.h"
#include "RoomManager.h"
#include "PickingManager.h"
#include "EconomyManager.h"
#include "CreatureManager.h"
#include "GUIManager.h"
#include "PathManager.h"
#include "SpellManager.h"

/*
	Game manager controls every module of the game. From
	drawing, sound, control, AI, resources...
*/

namespace game_utils
{
	namespace managers
	{
		class CGameManager: public CManager
		{
		public:
			CGameManager();
			virtual ~CGameManager();

			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			CResourceManager		*getResourceManager();
			CBlockManager			*getBlockManager();
			CLevelManager			*getLevelManager();
			CRenderManager			*getRenderManager();
			CNewRenderer			*getNewRenderer();
			CControlManager			*getControlManager();
			CCollisionManager		*getCollisionManager();
			CSettingsManager		*getSettingsManager();
			CAnimatedTerrainManager	*getAnimatedTerrainManager();
			CLightingManager		*getLightingManager();
			CRoomManager			*getRoomManager();
			CPickingManager			*getPickingManager();
			CEconomyManager			*getEconomyManager();
			CCreatureManager		*getCreatureManager();
			CGUIManager				*getGUIManager();
			CPathManager			*getPathManager();
			CSpellManager			*getSpellManager();

			control::CDeltaTime		*getDeltaTimeObject();
			GLfloat					getDeltaTime();
			utils::CText			*getTextPrinter();
			control::CConsole		*getConsole();			

		private:

			CSettingsManager		*settingsManager;
			CResourceManager		*resourceManager;
			CBlockManager			*blockManager;
			CLevelManager			*levelManager;
			CRenderManager			*renderManager;
			CControlManager			*controlManager;
			CCollisionManager		*collisionManager;
			CAnimatedTerrainManager	*animatedTerrainManager;
			CLightingManager		*lightingManager;
			CRoomManager			*roomManager;
			CPickingManager			*pickingManager;
			CEconomyManager			*economyManager;
			CCreatureManager		*creatureManager;
			CGUIManager				*guiManager;
			CPathManager			*pathManager;
			CSpellManager			*spellManager;
			CNewRenderer			*newRenderer;

			control::CDeltaTime		*deltaTime;
			utils::CText			*textPrinter;
			control::CConsole		*console;			

		};
	};
};

#endif // GAME_MANAGER_H