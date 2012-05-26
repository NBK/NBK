#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "Manager.h"
#include "Camera.h"

/*
	Check for collisions:
		- camera - terrain (when in FPS mode)
		- object - terrain (creatures walking, being hit by something and thrown back)
		- projectiles - terrain (shooting a meteor, and then explosion when it hits the terrain)
		- projectiles - objects (projectile hits the creature)

	Entities which want to be dealt by collision manager must register as 
	potential collision entity. And then on UPDATE all entities will get checked for 
	collisions. Collision response will also be included.
*/
namespace game_utils
{
	namespace managers
	{
		class CCollisionManager: public CManager
		{
		public:
			CCollisionManager();
			~CCollisionManager();

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			/*
				Use this function to register entities that might/will collide with terrain.
			*/
			GLvoid registerEntity_T(game_objects::CEntity *entity);

			/*
				Use this function to register entities with other entities.
			*/
			GLvoid registerEntity_O(game_objects::CEntity *entity);

		private:

			std::vector<game_objects::CEntity*>				T_entities;
			std::vector<game_objects::CEntity*>::iterator	entityIter;

			GLvoid check(game_objects::CEntity *entity, cml::vector2i blockPos);
		};
	};
};
#endif // COLLISION_MANAGER_H