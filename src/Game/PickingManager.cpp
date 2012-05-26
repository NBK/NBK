#include "commons.h"
#include "PickingManager.h"
#include "utils.h"
#include <cml/cml.h>
#include "OGLUtils.h"

#define _SECURE_SCL 0


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

			//using a map is very slow
			std::vector< pair<vector3ub, CBlock*> > colorBlockRef;

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
				vector3f e(bbox->E);
				vector3f f(bbox->F);
				vector3f g(bbox->G);
				vector3f h(bbox->H);

				a[1]=b[1]=c[1]=d[1]=CV_BLOCK_HEIGHT+CV_BLOCK_HEIGHT/4.0f;
				e[1]=f[1]=g[1]=h[1]=CV_BLOCK_HEIGHT/4.0f;

				vector3ub col = sCol.getNextColorUB();

				colorBlockRef.push_back(pair<vector3ub, CBlock*>(col, block));				

				glColor3ubv(&col[0]);

				glBegin(GL_QUADS);
				{
					if (block->isFaceVisible(CBlock::BFS_TOP))
					{
						glVertex3fv(&a[0]);
						glVertex3fv(&b[0]);
						glVertex3fv(&c[0]);
						glVertex3fv(&d[0]);

						if (block->isFaceVisible(CBlock::BFS_RIGHT))
						{
							glVertex3fv(&c[0]);
							glVertex3fv(&b[0]);
							glVertex3fv(&f[0]);
							glVertex3fv(&g[0]);
						}
						if (block->isFaceVisible(CBlock::BFS_LEFT))
						{
							glVertex3fv(&h[0]);
							glVertex3fv(&e[0]);
							glVertex3fv(&a[0]);
							glVertex3fv(&d[0]);
						}
						if (block->isFaceVisible(CBlock::BFS_FRONT))
						{
							glVertex3fv(&e[0]);
							glVertex3fv(&f[0]);
							glVertex3fv(&b[0]);
							glVertex3fv(&a[0]);
						}
						if (block->isFaceVisible(CBlock::BFS_BACK))
						{
							glVertex3fv(&g[0]);
							glVertex3fv(&h[0]);
							glVertex3fv(&d[0]);
							glVertex3fv(&c[0]);
						}

					}else{
						glVertex3fv(&e[0]);
						glVertex3fv(&f[0]);
						glVertex3fv(&g[0]);
						glVertex3fv(&h[0]);
					}
				}
				glEnd();
			}
		
			// 1.2 pick from backbuffer
			vector2i mousePos = CV_GAME_MANAGER->getControlManager()->getInput()->getMousePos();
			
			//convert mouse pos to relative to this window
#if WIN32
			RECT cl,wnd;
			GetClientRect(CV_WINDOW_HANDLE,&cl);
			GetWindowRect(CV_WINDOW_HANDLE,&wnd);

			int border=(wnd.right-wnd.left-cl.right)/2;
			int titlebar=wnd.bottom-wnd.top-cl.bottom-border;

			mousePos[0] -= border+wnd.left;
			mousePos[1] -= titlebar+wnd.top;
#else
			// TODO Tequila: Can we just take it as is with SDL Support ?
#endif

			vector3ub pickedColor = COGLUtils::getColor(mousePos);
			for(std::vector< pair<vector3ub, CBlock*> >::iterator i = colorBlockRef.begin(); i != colorBlockRef.end(); i++)
				if(i->first == pickedColor)
					lastPickedBlock = i->second;

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
