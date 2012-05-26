#ifndef ANIMATED_TERRAIN_MANAGER_H
#define ANIMATED_TERRAIN_MANAGER_H

#include "Block.h"
#include "Manager.h"

/*
	Used for animating terrain of lava and water. That's all.
*/
namespace game_utils
{
	namespace managers
	{
		class CAnimatedTerrainManager: public CManager
		{
		public:
			CAnimatedTerrainManager();
			~CAnimatedTerrainManager();

			virtual bool init();
			virtual bool update();			
			virtual bool shutdown();

			/*
				If the block is animated eg. lava or water then this 
				function will update its geometry.
			*/
			GLvoid updateBlock(game_objects::CBlock *block);

		private:
			GLfloat delta,
					animationSpeed,
					maxAmplitude;
		};
	};
};

#endif // ANIMATED_TERRAIN_MANAGER_H