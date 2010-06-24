#include "settings.h"
#include "commons.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Conversions.h"
#include "Frustum.h"
#include "Logger.h"

using namespace game_objects;
using namespace game_objects::block_objects;
using namespace game_utils;
using namespace utils;
using namespace cml;
using namespace std;
using namespace control;
using namespace rendering;
using namespace geometry;

namespace game_utils
{
	namespace managers
	{
		CRenderManager::CRenderManager(): CManager()
		{
			vbo = NULL;			
		}

		CRenderManager::~CRenderManager()
		{			
		}

		bool CRenderManager::init()
		{		
			vbo = new CVBO(CVBO::BT_STREAM_DRAW,false);

			tmpVboVertexBuffer = new GLfloat[CV_MAX_VERTEX_BUFFER*3];			
			tmpVboNormalBuffer = new GLfloat[CV_MAX_VERTEX_BUFFER*3];			
			tmpVboTexCoordBuffer = new GLfloat[CV_MAX_TEX_COORD_BUFFER*2];

			ZeroMemory(tmpVboVertexBuffer,sizeof(GLfloat)*CV_MAX_VERTEX_BUFFER*3);
			ZeroMemory(tmpVboNormalBuffer,sizeof(GLfloat)*CV_MAX_VERTEX_BUFFER*3);
			ZeroMemory(tmpVboTexCoordBuffer,sizeof(GLfloat)*CV_MAX_TEX_COORD_BUFFER*2);

			tmpVboVertexBufferSize = 0;
			tmpVboTexCoordBufferSize = 0;

			vbo->setVertexData(CV_MAX_VERTEX_BUFFER,3,sizeof(GLfloat),tmpVboVertexBuffer,CVBO::DT_FLOAT);
			vbo->setNormalData(CV_MAX_VERTEX_BUFFER,3,sizeof(GLfloat),tmpVboNormalBuffer,CVBO::DT_FLOAT);

			string textureQuality = CV_GAME_MANAGER->getSettingsManager()->getSetting_String(CV_SETTINGS_TEXTURE_QUALITY);
			
			GLint textureIndex = CV_GAME_MANAGER->getSettingsManager()->getSetting_Int(string(CV_SETTINGS_TEXTURE_ATLAS_INDEX)+"_"+textureQuality);

			CV_GAME_MANAGER->getResourceManager()->loadSelectedTexture(textureIndex);

			textureAtlasColor = CV_TEXTURE_LIST->getTexture(textureIndex);
			textureAtlasNormal = CV_TEXTURE_LIST->getTexture(textureIndex+CV_NORMAL_MAP_START_INDEX);

			vbo->setTextureData(CVBO::TU_UNIT0,textureAtlasColor,CV_MAX_TEX_COORD_BUFFER,2,sizeof(GLfloat),tmpVboTexCoordBuffer,CVBO::DT_FLOAT);
			vbo->setTextureData(CVBO::TU_UNIT1,textureAtlasNormal,CV_MAX_TEX_COORD_BUFFER,2,sizeof(GLfloat),tmpVboTexCoordBuffer,CVBO::DT_FLOAT);

			vbo->setEnumMode(CVBO::EM_QUADS);

			CSettingsManager *sManager = CV_GAME_MANAGER->getSettingsManager();
			cameraFPSExtent = sManager->getSetting_Float(CV_SETTINGS_CAMERA_FPS_EXTENT);					

			// setup FOG - TODO read settings from file
			GLfloat fog_color[]={0.0f,0.0f,0.0f,1.0f};
			glFogi(GL_FOG_MODE, GL_EXP);
			glFogfv(GL_FOG_COLOR, fog_color);
			glFogf(GL_FOG_DENSITY, 1.0f);
			glHint(GL_FOG_HINT, GL_DONT_CARE);
			glFogf(GL_FOG_START, 0.1f);
			glFogf(GL_FOG_END, 5.0f);

			return true;
		}

		bool CRenderManager::update()
		{
			glColor3f(1.0f,1.0f,1.0f);

			// get camera
			CCamera *camera = CV_GAME_MANAGER->getControlManager()->getCamera();

			// transform view
			camera->transformView();			

			// Draw the map and items that fall into view frustum. 								

			// 1. extract approximate logical location of camera in the level map.
			vector2i center = CConversions::realToLogical(camera->getPosition());

			GLint centerX = center[0];
			GLint centerY = center[1];

			bool isFPS = CV_GAME_MANAGER->getControlManager()->isFPS();

			if (isFPS)
			{
				// fog only in FPS mode
				glEnable(GL_FOG);
			}

			/*
				In FPS mode we can't use height to determine visible offset. 
				We have to use some extent read from config (CV_CAMERA_FPS_EXTENT).
			*/
			GLint diff = (GLint)(isFPS?cameraFPSExtent:camera->getPosition()[1]*10.0f);

			// 2. create a bounding square making its center logical position calculate above.
			GLint minX = (centerX-diff>=0?centerX-diff:0);
			GLint minY = (centerY-diff>=0?centerY-diff:0);
			GLint maxX = (centerX+diff<CV_LEVEL_MAP_SIZE?centerX+diff:CV_LEVEL_MAP_SIZE-1);
			GLint maxY = (centerY+diff<CV_LEVEL_MAP_SIZE?centerY+diff:CV_LEVEL_MAP_SIZE-1);			

			// 3. go through all block that fall into this bounding square and check if they fall
			//    int out view frustum. If not then just exclude them.

			CBlock *block;

			GLint	blockVisible = 0,
					allVerticesCount = 0,
					creaturesVisible = 0,
					maxVertInput = 0,
					maxTexInput  = 0;

			tmpVboVertexBufferSize = 0;
			tmpVboTexCoordBufferSize = 0;

			vector3f	vertA,
						vertB,
						vertC;

			GLfloat **verts,
					*texCoords;

			CLevelManager			*lManager = CV_GAME_MANAGER->getLevelManager();
			CAnimatedTerrainManager	*atManager = CV_GAME_MANAGER->getAnimatedTerrainManager();
			CFrustum				*frustum = CV_GAME_MANAGER->getControlManager()->getViewFrustum();

			bool lavaWater = false;

			GLfloat delta = CV_GAME_MANAGER->getDeltaTime();	

			renderedBlocks.clear();

			for (GLint y=minY; y<=maxY; y++)
			{
				for (GLint x=minX; x<=maxX; x++)
				{
					block = lManager->getBlock(x,y);

					if (block)
					{
						//block->getBoundingBox()->draw(); // just for testing
						if (frustum->containsBBOX(block->getBoundingBox()))
						{
							blockVisible++;

							block->updateTexture(delta);

							lavaWater = (block->isLava() || block->isWater());

							if (lavaWater)
							{
								atManager->updateBlock(block);
							}

							renderedBlocks.push_back(block);

							// draw block objects
							if (block->getBlockObjects()->size()>0)
							{
								for (std::vector<CBlockObject*>::iterator rmIter = block->getBlockObjects()->begin(); rmIter != block->getBlockObjects()->end(); rmIter++)
								{
									CBlockObject *bObj = *rmIter;

									bObj->moveTo();
									glRotatef(bObj->getRotateY(),0.0f,1.0f,0.0f);
									bObj->drawModel(delta);
									glRotatef(-bObj->getRotateY(),0.0f,1.0f,0.0f);
									bObj->moveBack();
								}
							}

							bool isRoom = block->isRoom();

							if (isRoom)
							{
								std::vector<GLuint> *dls = block->getDisplayLists();
								if (dls->size()!=0)
								{			
									glEnable(GL_TEXTURE_2D);
									glBindTexture(GL_TEXTURE_2D,textureAtlasColor);
									glBegin(GL_QUADS);
									{
										for (std::vector<GLuint>::iterator dlIter = dls->begin(); dlIter != dls->end(); dlIter++)
										{
											glCallList(*dlIter);									
										}
									}
									glEnd();
									glDisable(GL_TEXTURE_2D);									
								}
							}							

							for (GLint f=CBlock::BFS_FRONT; f<=CBlock::BFS_CEILING; f++)
							{
								if ((!isFPS && f==CBlock::BFS_CEILING) || (isFPS && f==CBlock::BFS_TOP) || (isRoom && f!=CBlock::BFS_CEILING))
								{
									continue;
								}

								if (block->isFaceVisible((CBlock::BLOCK_FACE_SELECTOR)f))
								{		
									verts = block->getVertices();
									texCoords = block->getTextureCoordinates((CBlock::BLOCK_FACE_SELECTOR)f);

									if (lavaWater && f<=CBlock::BFS_RIGHT)
									{
										/* 
											Lava and water have only lowers row of wall sections drawn. 
											If they are drawn at all.
										*/
										maxVertInput = CV_FBLR_W_L_FACE_VERT_FLOATS;
										maxTexInput = CV_FBLR_W_L_FACE_TEX_FLOATS;
									}
									else
									{
										maxVertInput = f>=CBlock::BFS_TOP?CV_TBWLC_FACE_VERT_FLOATS:CV_FBLR_FACE_VERT_FLOATS;
										maxTexInput = f>=CBlock::BFS_TOP?CV_TBWLC_FACE_TEX_FLOATS:CV_FBLR_FACE_TEX_FLOATS;
									}

									if (tmpVboVertexBufferSize+maxVertInput>CV_MAX_VERTEX_BUFFER*3)
									{		
										vbo->setElementsCount(CVBO::IDT_vertex,tmpVboVertexBufferSize/3);
										vbo->setElementsCount(CVBO::IDT_texture0,tmpVboTexCoordBufferSize/2);
										vbo->setElementsCount(CVBO::IDT_texture1,tmpVboTexCoordBufferSize/2);
										vbo->draw();
										allVerticesCount+=tmpVboVertexBufferSize;
										tmpVboVertexBufferSize=0;
										tmpVboTexCoordBufferSize=0;						
									}

									memcpy(tmpVboVertexBuffer+tmpVboVertexBufferSize, verts[f], sizeof(GLfloat)*maxVertInput);
									tmpVboVertexBufferSize+=maxVertInput;

									memcpy(tmpVboTexCoordBuffer+tmpVboTexCoordBufferSize, texCoords, sizeof(GLfloat)*maxTexInput);						
									tmpVboTexCoordBufferSize+=maxTexInput;
																	
								}
							}
						}
					}
				}
			}

			if (tmpVboVertexBufferSize>0)
			{		
				vbo->setElementsCount(CVBO::IDT_vertex,tmpVboVertexBufferSize/3);
				vbo->setElementsCount(CVBO::IDT_texture0,tmpVboTexCoordBufferSize/2);
				vbo->setElementsCount(CVBO::IDT_texture1,tmpVboTexCoordBufferSize/2);
				vbo->draw();
				allVerticesCount+=tmpVboVertexBufferSize;
			}	

			// draw creatures
			CCreatureManager *cManager = CV_GAME_MANAGER->getCreatureManager();
			GLint cCount = cManager->getCreatureVector()->size();
			if (cCount>0)
			{		
				CCreature *creature = NULL;
				for (std::vector<CCreature*>::iterator cIter = cManager->getCreatureVector()->begin(); cIter != cManager->getCreatureVector()->end(); cIter++)
				{
					creature = (*cIter);

					if (creature)
					{
						sBoundingBox *cBBOX = creature->getModel()->getBoundingBox();
						cBBOX->translate(creature->getPosition());
						if (frustum->containsBBOX(cBBOX))
						{
							creature->draw(delta);
							creaturesVisible++;
						}
						cBBOX->translate(-creature->getPosition());
					}
				}
			}

			// draw transparent block objects
			for (std::vector<CBlock*>::iterator vbIter = renderedBlocks.begin(); vbIter != renderedBlocks.end(); vbIter++)
			{
				block = *vbIter;
				if (block->getBlockObjects()->size()>0)
				{
					for (std::vector<CBlockObject*>::iterator rmIter = block->getBlockObjects()->begin(); rmIter != block->getBlockObjects()->end(); rmIter++)
					{
						CBlockObject *bObj = *rmIter;

						bObj->moveTo();
						glRotatef(bObj->getRotateY(),0.0f,1.0f,0.0f);
						bObj->drawEffect();
						glRotatef(-bObj->getRotateY(),0.0f,1.0f,0.0f);
						bObj->moveBack();
					}
				}
			}

			glDisable(GL_FOG);
			
			if (!isFPS)
			{
				handlePickedObjects();
			}

			CV_GAME_MANAGER->getTextPrinter()->print((GLfloat)0,(GLfloat)(CV_SETTINGS_WINDOW_HEIGHT-15*3),"Visible blocks: %d",blockVisible);
			CV_GAME_MANAGER->getTextPrinter()->print((GLfloat)0,(GLfloat)(CV_SETTINGS_WINDOW_HEIGHT-15*2),"Visible creatures: %d",creaturesVisible);
			CV_GAME_MANAGER->getTextPrinter()->print((GLfloat)0,(GLfloat)(CV_SETTINGS_WINDOW_HEIGHT-15),"Triangles drawn: %d",(allVerticesCount/4)*2);

			// render the lights representations. usefull for debugging
			CV_GAME_MANAGER->getLightingManager()->drawLightSources(frustum);
            
			return true;
		}

		bool CRenderManager::shutdown()
		{
			delete []	tmpVboVertexBuffer;
			delete []	tmpVboNormalBuffer;
			delete []	tmpVboTexCoordBuffer;
			delete		vbo;			

			return true;
		}

		std::vector<CBlock*> *CRenderManager::getRenderedBlocks()
		{
			return &renderedBlocks;
		}

		GLvoid CRenderManager::handlePickedObjects()
		{
			// if we are ower the menu we do not have to proccess things under the cursor
			if (CV_GAME_MANAGER->getGUIManager()->getPlayGUI()->is_mouse_over_gui())
			{
				return;
			}

			// if we are not selling or buying we don't have to process blocks (just objects TODO)
			ACTION_EVENT *ae = CV_GAME_MANAGER->getGUIManager()->getLastActionEvent();
			
			// get the block we have our cursor on
			CBlock *pickedBlock = CV_GAME_MANAGER->getPickingManager()->getLastPickedBlock();

			if (pickedBlock)
			{
				//go through all the blocks to determine which are highlighted
				for (GLint y=0; y<=CV_LEVEL_MAP_SIZE; y++)
				{
					for (GLint x=0; x<=CV_LEVEL_MAP_SIZE; x++)
					{
						pickedBlock = CV_GAME_MANAGER->getLevelManager()->getBlock(x,y);
						if(!pickedBlock)
							continue;

						//render only if block is highlighted
						if(!pickedBlock->isHighlighted())
							continue;

						GLint type = pickedBlock->getType();

						/*if (!pickedBlock->isSellable())
						{
							return;
						}*/

						sBoundingBox *bbox = pickedBlock->getBoundingBox();

						vector3f a(bbox->A);
						vector3f b(bbox->B);
						vector3f c(bbox->C);
						vector3f d(bbox->D);
						vector3f e(bbox->E);
						vector3f f(bbox->F);
						vector3f g(bbox->G);
						vector3f h(bbox->H);

						a[1]=b[1]=c[1]=d[1]=CV_BLOCK_HEIGHT+CV_BLOCK_HEIGHT/4.0f+CV_BLOCK_HEIGHT/32.0f;
						e[1]=f[1]=g[1]=h[1]=CV_BLOCK_HEIGHT/4.0f+CV_BLOCK_HEIGHT/32.0f;

						glLineWidth(4.0f);

						if (pickedBlock->isLow())
						{
							if (!(ae->message_group==AEMG_BUILD_ROOMS || ae->message_group==AEMG_BUILD_DOORS || ae->message_group==AEMG_BUILD_TRAPS))
							{
								return;
							}

							// draw the selection box					

							if (pickedBlock->isSellable(CV_CURRENT_PLAYER_ID) && ae->message == AEM_SELL)
							{
								glColor3f(0.0f,1.0f,0.0f);
							}
							else if (pickedBlock->isBuildable(CV_CURRENT_PLAYER_ID) && ae->message != AEM_SELL)
							{
								glColor3f(0.0f,1.0f,0.0f);
							}
							else
							{
								glColor3f(1.0f,0.0f,0.0f);
							}
					
							glBegin(GL_LINES);
							{
								/*glVertex3fv(&a[0]); glVertex3fv(&b[0]);
								glVertex3fv(&b[0]); glVertex3fv(&c[0]);
								glVertex3fv(&c[0]); glVertex3fv(&d[0]);
								glVertex3fv(&d[0]); glVertex3fv(&a[0]);*/

								glVertex3fv(&e[0]); glVertex3fv(&f[0]);
								glVertex3fv(&f[0]); glVertex3fv(&g[0]);
								glVertex3fv(&g[0]); glVertex3fv(&h[0]);
								glVertex3fv(&h[0]); glVertex3fv(&e[0]);

								/*glVertex3fv(&a[0]); glVertex3fv(&e[0]);
								glVertex3fv(&b[0]); glVertex3fv(&f[0]);
								glVertex3fv(&c[0]); glVertex3fv(&g[0]);
								glVertex3fv(&d[0]); glVertex3fv(&h[0]);*/
							}
							glEnd();
						}
						else
						{		
							if (!(ae->message_group==AEMG_BUILD_ROOMS || ae->message_group==AEMG_BUILD_DOORS || ae->message_group==AEMG_BUILD_TRAPS))
								glColor3f(type==CV_BLOCK_TYPE_ROCK_ID?1.0f:0.0f,type==CV_BLOCK_TYPE_ROCK_ID?0.0f:1.0f,0.0f);
							else
								glColor3f(1.0f,0.0f,0.0f);
			
							glBegin(GL_LINES);
							{
								glVertex3fv(&a[0]); glVertex3fv(&b[0]);
								glVertex3fv(&b[0]); glVertex3fv(&c[0]);
								glVertex3fv(&c[0]); glVertex3fv(&d[0]);
								glVertex3fv(&d[0]); glVertex3fv(&a[0]);

								glVertex3fv(&e[0]); glVertex3fv(&f[0]);
								glVertex3fv(&f[0]); glVertex3fv(&g[0]);
								glVertex3fv(&g[0]); glVertex3fv(&h[0]);
								glVertex3fv(&h[0]); glVertex3fv(&e[0]);

								glVertex3fv(&a[0]); glVertex3fv(&e[0]);
								glVertex3fv(&b[0]); glVertex3fv(&f[0]);
								glVertex3fv(&c[0]); glVertex3fv(&g[0]);
								glVertex3fv(&d[0]); glVertex3fv(&h[0]);
							}
							glEnd();
						}

						glLineWidth(1.0f);
					}
				}
			}
		}
	};
};