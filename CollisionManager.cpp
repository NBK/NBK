#include "commons.h"
#include "utils.h"
#include "CollisionManager.h"
#include "Conversions.h"
#include "LevelManager.h"
#include <cml/cml.h>

using namespace game_objects;
using namespace game_utils;
using namespace cml;
using namespace utils;

namespace game_utils
{
	namespace managers
	{
		CCollisionManager::CCollisionManager(): CManager()
		{
		}

		CCollisionManager::~CCollisionManager()
		{
		}

		bool CCollisionManager::init()
		{
			return true;
		}
		
		GLvoid CCollisionManager::check(CEntity *entity, cml::vector2i blockPos)
		{
		}

		bool CCollisionManager::update()
		{
			CEntity *entity;
					

			for (entityIter=T_entities.begin(); entityIter!=T_entities.end(); ++entityIter)
			{
				entity = *entityIter;
				/*	
					Check entities with terrain end make proper response, 
					all 4 must be checked since we ce be in the corner.
				*/
				if (entity->isActive())
				{
					// 1. get logical position
					vector2i pos = CConversions::realToLogical(entity->getPosition());

					// 2. extract proper planes of nearby block					
					check(entity,pos+vector2i(0,-1));
					check(entity,pos+vector2i(0,1));
					check(entity,pos+vector2i(-1,0));
					check(entity,pos+vector2i(1,0));

					/* ? check(entity,pos+vector2i(1,1));
					check(entity,pos+vector2i(1,-1));
					check(entity,pos+vector2i(-1,-1));
					check(entity,pos+vector2i(-1,1)); ? */

					// just clamp entity to the bottom if we are below min level
					vector3f ePos = entity->getPosition();

					if (ePos[1]<CV_CAMERA_MIN_FPS_Y_POS)
					{
						ePos[1] = CV_CAMERA_MIN_FPS_Y_POS;
						entity->setPosition(ePos);
					}
				}
			}

			return true;
		}

		bool CCollisionManager::shutdown()
		{
			return true;
		}

		GLvoid CCollisionManager::registerEntity_T(CEntity *entity)
		{
			T_entities.push_back(entity);
		}

		GLvoid CCollisionManager::registerEntity_O(CEntity *entity)
		{
		}
	}
}