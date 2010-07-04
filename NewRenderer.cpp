#include "commons.h"
#include "NewRenderer.h"

namespace game_utils
{
	namespace managers
	{
		CNewRenderer::CNewRenderer()
		{
		}

		CNewRenderer::~CNewRenderer()
		{
		}

		bool CNewRenderer::init()
		{
			return true;
		}

		//this is where the rendering is done
		bool CNewRenderer::update()
		{

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();
			game_objects::CBlock *block;
			geometry::CFrustum	*frustum = CV_GAME_MANAGER->getControlManager()->getViewFrustum();

			//clear renderedBlocks, we will re-fill this with things from the frustum
			renderedBlocks.clear();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			CV_GAME_MANAGER->getControlManager()->getCamera()->transformView();	

			//iterate through all blocks, checking if they are in the frustum
			for (GLint y=0; y<=CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<=CV_LEVEL_MAP_SIZE; x++)
				{
					block = lManager->getBlock(x,y);

					if(!block) continue;
					
					geometry::sBoundingBox *boundingBox = block->getBoundingBox();

					//actually render, it's visible
					if(frustum->containsBBOX(boundingBox))
					{
						renderedBlocks.push_back(block);

					}
				}
			}
			return true;
		}

		bool CNewRenderer::shutdown()
		{
			return true;
		}

		std::vector<game_objects::CBlock*> *CNewRenderer::getRenderedBlocks()
		{
			return &renderedBlocks;
		}
	}
}