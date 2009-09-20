#include "commons.h"
#include "PickingManager.h"
#include "utils.h"
#include <cml/cml.h>
#include "OGLUtils.h"

using namespace std;
using namespace utils;
using namespace game_objects;
using namespace cml;

namespace game_utils
{
	namespace managers
	{
		CPickingManager::CPickingManager(): CManager()
		{
			lastPickedBlock = NULL;
		}

		CPickingManager::~CPickingManager()
		{
		}

		bool CPickingManager::init()
		{
			return true;
		}

		bool CPickingManager::update()
		{
			// 1. block picking

			// 1.1 draw pickable blocks
			std::vector<CBlock*> *rBlocks = CV_GAME_MANAGER->getRenderManager()->getRenderedBlocks();

			if (rBlocks->size()==0)
			{
				return true;
			}

			map<vector3ub, CBlock*> colorBlockRef;

			sColor sCol(50,50,50);

			// transform view
			CV_GAME_MANAGER->getControlManager()->getCamera()->transformView();	
			
			for (std::vector<CBlock*>::iterator bIter = rBlocks->begin(); bIter != rBlocks->end(); bIter++)
			{
				CBlock *block = *bIter;

				sBoundingBox *bbox = block->getBoundingBox();

				vector3f a(bbox->A);
				vector3f b(bbox->B);
				vector3f c(bbox->C);
				vector3f d(bbox->D);

				a[1]=b[1]=c[1]=d[1]=CV_BLOCK_HEIGHT+CV_BLOCK_HEIGHT/4.0f;

				vector3ub col = sCol.getNextColorUB();

				colorBlockRef[col] = block;					

				glColor3ubv(&col[0]);

				glBegin(GL_QUADS);
				{
					glVertex3fv(&a[0]);
					glVertex3fv(&b[0]);
					glVertex3fv(&c[0]);
					glVertex3fv(&d[0]);
				}
				glEnd();
			}
		
			// 1.2 pick from backbuffer
			vector2i mousePos = CV_GAME_MANAGER->getControlManager()->getInput()->getMousePos();
			vector3ub pickedColor = COGLUtils::getColor(mousePos);
			lastPickedBlock = colorBlockRef[pickedColor];			

			// 1.3 clear the backbuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			return true;
		}

		bool CPickingManager::shutdown()
		{
			return true;
		}

		CBlock *CPickingManager::getLastPickedBlock()
		{
			return lastPickedBlock;
		}
	};
};