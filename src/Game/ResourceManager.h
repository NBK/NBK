#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "BR5Model.h"
#include "LUA/LUAEffect.h"
#include "ObjectPool.h"
#include <map>

/*
	When you need a model, a copy of a model, a sound... you come here.
*/

#include "TextureList.h"
#include "Manager.h"
#include "ConsoleListener.h"
#include "CreatureTxtReader.h"

namespace game_utils
{
	namespace managers
	{
		class CResourceManager: public CManager, public control::CConsoleListener
		{
		public:
			CResourceManager();
			virtual ~CResourceManager();

			GLvoid loadSelectedTexture(GLint textureIndex);

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

			loaders::CBR5Model		*getModel(std::string modelName);
			GLvoid					returnModel(std::string modelName, loaders::CBR5Model *model);

			LUA_effects::CLUAEffect	*getEffect(std::string effectName);
			GLvoid					returnEffect(std::string effectName, LUA_effects::CLUAEffect *effect);

			CCreatureTxtReader *creature_txt_reader;

		private:

			/*
				Description:
				Extract texture info from a line that has been 
				read from a texture list file.

				Return:
				Filled sTextureData and index of texture map.
			*/
			std::pair<GLint, lists::CTextureList::sTextureData> extractTextureInfo(std::string line);

			/*
				Description:
				Loads all textures from data\resources\textures.conf. Lines should be
				in this form:

				# fileName (eg: 1 floor1.jpg) FOR NOW !!

				We can supply other texture lists

				Return:
				True if all goes well. 
			*/
			bool loadTexturesFromList();

			/*
				Here we have object pools.
			*/

			// an index -> model pool
			std::map<std::string, game_utils::CObjectPool<loaders::CBR5Model>* >			modelPool;
			std::map<std::string, game_utils::CObjectPool<loaders::CBR5Model>* >::iterator	mpIter;

			std::map<std::string, game_utils::CObjectPool<LUA_effects::CLUAEffect>* >			effectPool;
			std::map<std::string, game_utils::CObjectPool<LUA_effects::CLUAEffect>* >::iterator	effIter;

			GLvoid initEffects();
		};
	};
};

#endif // RESOURCE_MANAGER_H