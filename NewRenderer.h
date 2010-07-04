#ifndef NEW_RENDERER_H
#define NEW_RENDERER_H

#include "manager.h"
#include "Block.h"

namespace game_utils
{
	namespace managers
	{
		class CNewRenderer: public CManager
		{
		public:
			CNewRenderer();
			~CNewRenderer();

			bool init();
			bool update();
			bool shutdown();

			std::vector<game_objects::CBlock*> *getRenderedBlocks();
		private:
			std::vector<game_objects::CBlock*> renderedBlocks;
		};
	};
};

#endif