#include "settings.h"
#include "commons.h"
#include "AnimatedTerrainManager.h"

namespace game_utils
{
	namespace managers
	{
		CAnimatedTerrainManager::CAnimatedTerrainManager(): CManager()
		{					
		}

		CAnimatedTerrainManager::~CAnimatedTerrainManager()
		{
		}

		bool CAnimatedTerrainManager::init()
		{
			delta=0.0f;

			// read configs from settings manager
			CSettingsManager *sManager = CV_GAME_MANAGER->getSettingsManager();

			animationSpeed = sManager->getSetting_Float(CV_SETTINGS_FLUID_ANIMATION_SPEED);
			maxAmplitude = sManager->getSetting_Float(CV_SETTINGS_FLUID_ANIMATION_MAX_AMPLITUDE);

			return true;
		}

		bool CAnimatedTerrainManager::update()
		{
			delta += animationSpeed*CV_GAME_MANAGER->getDeltaTime();
			return true;
		}

		bool CAnimatedTerrainManager::shutdown()
		{
			return true;
		}

		GLvoid CAnimatedTerrainManager::updateBlock(game_objects::CBlock *block)
		{
			GLint mx,my;
			GLfloat ny;

			GLint mapX = block->getLogicalPosition()[0];
			GLint mapY = block->getLogicalPosition()[1];

			CBlockManager *bManager = CV_GAME_MANAGER->getBlockManager();

			for (GLint y=0; y<4; y++)
			{
				for (GLint x=0; x<4; x++)
				{			
					if (block->getSkipFlag(x,y))
					{
						continue;
					}
					mx=mapX*3+x;
					my=mapY*3+y;

					ny = sin((GLfloat)(mx*my)+delta)*maxAmplitude;
					bManager->setGlobalDeformedMapPoint(mapX,mapY,x,0,y,ny);
				}
			}

			bManager->reloadSubLevelVertices(block);
		}
	};
};