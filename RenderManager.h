#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "VBO.h"
#include "Manager.h"
#include "Block.h"

/*
	Render manager is the main thing that controls the drawing
	of everything in this game. Map, models, GUI, ...
*/

namespace game_utils
{
	namespace managers
	{
		class CRenderManager: public CManager
		{
		public:
			CRenderManager();
			~CRenderManager();

			virtual bool init();
			virtual bool update();	
			virtual bool shutdown();

			std::vector<game_objects::CBlock*> *getRenderedBlocks();

		protected:

			rendering::CVBO *vbo;

			GLfloat			*tmpVboVertexBuffer;
			GLfloat			*tmpVboNormalBuffer;
			GLfloat			*tmpVboTexCoordBuffer;
			GLint			tmpVboVertexBufferSize,
							tmpVboTexCoordBufferSize;

			GLfloat			cameraFPSExtent;	

			GLuint	textureAtlasColor,
					textureAtlasNormal;

			/* 
				All blocks that get rendered in this frame get added to this list. 
				Then the picking manager uses it for the picking operations.
			*/
			std::vector<game_objects::CBlock*> renderedBlocks;

			GLvoid handlePickedObjects();
		};
	};
};

#endif // RENDER_MANAGER_H