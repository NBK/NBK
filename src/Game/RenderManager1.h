#ifndef RENDER_MANAGER_1_H
#define RENDER_MANAGER_1_H

#include "ShaderManager.h"
#include "RenderManager.h"
#include "Block.h"
#include "ConsoleListener.h"

/*
	Extension to the original render manager. Only
	the render part has been changed. This way we
	can render a scene with > 100 lights on it.
*/

namespace game_utils
{
	namespace managers
	{
		class CRenderManager1: public CRenderManager, public control::CConsoleListener
		{
		public:
			CRenderManager1();
			virtual ~CRenderManager1();

			// from CManager
			virtual bool init();
			virtual bool update();	
			virtual bool shutdown();

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

		private:

			// shaders for PR and PP lighting
			shaders::CShaderManager	*shaderManager;

			/*
				Setups lights affecting this block so they can be used in a shader program.
			*/
			GLvoid setupLights(game_objects::CBlock *block);

			GLvoid loadShaders();

			static GLfloat normals[8][3];

			// used in console
			bool	usePPL,					// enables/disabled perpixel lighting
					drawBlockLightSources;

			GLint	blockVisible,
					allVerticesCount,
					shaderInUse;
		};
	};
};

#endif // RENDER_MANAGER_1_H
