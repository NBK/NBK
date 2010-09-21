#include "settings.h"
#include "commons.h"
#include "RenderManager1.h"
#include "Camera.h"
#include "Frustum.h"
#include "Logger.h"
#include "utils.h"

// TODO: fix render speed!

using namespace game_objects;
using namespace game_objects::lighting;
using namespace game_objects::block_objects;
using namespace game_utils;
using namespace utils;
using namespace cml;
using namespace std;
using namespace control;
using namespace rendering;
using namespace geometry;
using namespace shaders;

#define SHADERS__SIMPLE_TEXTURE	0
#define SHADERS__PPL_TEXTURE	1
#define SHADERS__PPL_NMAP		2

#define PPL		"PPL"
#define LTQA	"LTQA"
#define LTLA	"LTLA"
#define LTIA	"LTIA"
#define VB		"VB"
#define TC		"TC"
#define PLT		"PLT"
#define BLS		"BLS"
#define RS		"RS"

namespace game_utils
{
	namespace managers
	{
		GLfloat CRenderManager1::normals[8][3] = 
		{
			{0.0f,0.0f,1.0f},	// front
			{0.0f,0.0f,-1.0f},	// back
			{-1.0f,0.0f,0.0f},	// left
			{1.0f,0.0f,0.0f},	// right
			{0.0f,1.0f,0.0f},	// top
			{0.0f,1.0f,0.0f},	// bottom
			{0.0f,1.0f,0.0f},	// lava
			{0.0f,-1.0f,0.0f},	// ceiling
		};

		CRenderManager1::CRenderManager1(): CRenderManager(), CConsoleListener()
		{
			shaderManager = NULL;			
			drawBlockLightSources = false;

			usePPL = CV_GAME_MANAGER->getSettingsManager()->getSetting_Int(CV_SETTINGS_USE_PPL)==1;

			shaderInUse = SHADERS__PPL_TEXTURE;
		}

		CRenderManager1::~CRenderManager1()
		{
		}

		GLvoid CRenderManager1::loadShaders()
		{
			if (!shaderManager->addShader(SHADERS__SIMPLE_TEXTURE,CV_RESOURCES_DIRECTORY+"shaders\\0\\texture0.vert",CV_RESOURCES_DIRECTORY+"shaders\\0\\texture0.frag"))
			{
				CLogger::addEntry("ERROR: %s\n",shaderManager->getLastError().c_str());
				CV_GAME_MANAGER->getConsole()->writeLine("Shader problem! (check logs)");
			}	

			if (!shaderManager->addShader(SHADERS__PPL_TEXTURE,CV_RESOURCES_DIRECTORY+"shaders\\1\\ppl.vert",CV_RESOURCES_DIRECTORY+"shaders\\1\\ppl.frag"))
			{
				CLogger::addEntry("ERROR:: %s\n",shaderManager->getLastError().c_str());
				CV_GAME_MANAGER->getConsole()->writeLine("Shader problem! (check logs)");
			}	

			if (!shaderManager->addShader(SHADERS__PPL_NMAP,CV_RESOURCES_DIRECTORY+"shaders\\3\\nmap.vert",CV_RESOURCES_DIRECTORY+"shaders\\3\\nmap.frag"))
			{
				CLogger::addEntry("ERROR: %s\n",shaderManager->getLastError().c_str());
				CV_GAME_MANAGER->getConsole()->writeLine("Shader problem! (check logs)");
			}
		}

		bool CRenderManager1::init()
		{
			shaderManager = new CShaderManager();
			loadShaders();

			glEnable(GL_COLOR_MATERIAL);
			static float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);	

			//Set global ambient colour
			GLfloat globalAmbientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
			memcpy(&globalAmbientColor[0],&CV_GAME_MANAGER->getSettingsManager()->getSetting_vector3f(CV_SETTINGS_GLOBAL_AMBIENCE),sizeof(GLfloat)*3);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientColor);

			// register to console
			CV_GAME_MANAGER->getConsole()->registerClass(this,"RENDER MANAGER");
			CV_GAME_MANAGER->getConsole()->addParam(PPL,"() Enables/Disables the perpixel lighting.");
			CV_GAME_MANAGER->getConsole()->addParam(LTQA,"(TYPE x.x) Sets|Returns the lights (of the specified type) quadratic attenuation.");
			CV_GAME_MANAGER->getConsole()->addParam(LTLA,"(TYPE x.x) Sets|Returns the lights (of the specified type) linear attenuation.");
			CV_GAME_MANAGER->getConsole()->addParam(LTIA,"(TYPE x.x) Sets|Returns the lights (of the specified type) initial attenuation.");
			CV_GAME_MANAGER->getConsole()->addParam(PLT,"() Prints the light types.");
			CV_GAME_MANAGER->getConsole()->addParam(VB,"() Returns visible blocks.");
			CV_GAME_MANAGER->getConsole()->addParam(BLS,"() Toggle drawing of helper lines showing the light-block relation.");
			CV_GAME_MANAGER->getConsole()->addParam(RS,"() Reloads all shaders from source.");

			return CRenderManager::init();
		}

		GLvoid CRenderManager1::setupLights(CBlock *block)
		{
			CLightingManager *liManager = CV_GAME_MANAGER->getLightingManager();

			CBlockLightData *blockLightData = liManager->getBlockLightData(block->getLogicalPosition());

			GLint lightsCount = blockLightData->getLightsCount();

			CCamera *camera = CV_GAME_MANAGER->getControlManager()->getCamera();								

			//if (lightsCount>0 && usePPL)
			if (usePPL)
			{
				shaderManager->useShaderProgram(shaderInUse);
				shaderManager->getShaderObject(shaderInUse)->setUniform1i("numberOfLights",lightsCount);

				// tell shader where (in which texture unit) to find color and normal map
				shaderManager->getShaderObject(shaderInUse)->setUniform1i("tex",0);
				shaderManager->getShaderObject(shaderInUse)->setUniform1i("nrm",1);
			}
			else
			{
				shaderManager->useFFPipeline();
			}			

			if (drawBlockLightSources)
			{			
				for (GLint i=0; i<lightsCount; i++)
				{	
					glColor3f(1.0f,1.0f,1.0f);
					vector3f start = block->getRealPosition()+vector3f(CV_BLOCK_WIDTH/2.0f,block->getVertices()[CBlock::BFS_BOTTOM][1],CV_BLOCK_DEPTH/2.0f);
					glBegin(GL_LINES);
					{
						glVertex3fv(&start[0]);
						glVertex3fv(&blockLightData->getLightSource(i)->getPosition()[0]);
					}
					glEnd();
				}
			}

			glDisable(GL_LIGHTING);
			for (GLint i=0; i<8; i++)
			{				
				glDisable(GL_LIGHT0+i);
			}

			if (usePPL)
			{					
				GLfloat pos[4] = {0.0f,0.0f,0.0f,1.0f};
				for (GLint i=lightsCount-1; i>=0; i--)
				{	
					CLightSource *lgt = blockLightData->getLightSource(i);					

					memcpy(pos,&lgt->getPosition()[0],sizeof(GLfloat)*3);

					glLightfv(GL_LIGHT0+i, GL_POSITION, pos);
					glLightfv(GL_LIGHT0+i, GL_AMBIENT, lgt->getAmbientColor());
					glLightfv(GL_LIGHT0+i, GL_DIFFUSE, lgt->getDiffuseColor());
					glLightfv(GL_LIGHT0+i, GL_SPECULAR, lgt->getSpecularColor());
					glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, lgt->getQuadraticAttenuation());
					glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, lgt->getLinearAttenuation());
					glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, lgt->getInitialAttenuation());
					glEnable(GL_LIGHT0+i);

				}		
				glEnable(GL_LIGHTING);
			}
		}

		bool CRenderManager1::update()
		{			
			// Draw the map and items that fall into view frustum. 	

			glColor3f(1.0f,1.0f,1.0f);

			// get camera
			CCamera *camera = CV_GAME_MANAGER->getControlManager()->getCamera();		
			

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

			// 3. go through all block that fall into this bounding square and check if they fall
			//    int out view frustum. If not then just exclude them.

			CBlock *block;

			GLint	maxVertInput = 0,
					creaturesVisible = 0,
					maxTexInput  = 0;

			tmpVboVertexBufferSize = 0;
			tmpVboTexCoordBufferSize = 0;

			vector3f	vertA,
						vertB,
						vertC;

			GLfloat **verts,
					**texCoords;

			CLevelManager			*lManager = CV_GAME_MANAGER->getLevelManager();
			CAnimatedTerrainManager	*atManager = CV_GAME_MANAGER->getAnimatedTerrainManager();
			CFrustum				*frustum = CV_GAME_MANAGER->getControlManager()->getViewFrustum();

			bool lavaWater = false;

			blockVisible=allVerticesCount=0;

			GLfloat delta = CV_GAME_MANAGER->getDeltaTime();	

			// transform view
			camera->transformView();

			renderedBlocks.clear();

			for (GLint y=0; y<=CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLint x=0; x<=CV_LEVEL_MAP_SIZE; x++)
				{
					//if block is far from the camera, cull
					if((x-centerX)*(x-centerX)+(y-centerY)*(y-centerY)>300)
						continue;

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

							setupLights(block);

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

								verts = block->getVertices();
								texCoords = block->getTextureCoordinates();

								if (block->isFaceVisible((CBlock::BLOCK_FACE_SELECTOR)f))
								{		
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

									memcpy(tmpVboVertexBuffer+tmpVboVertexBufferSize, verts[f], sizeof(GLfloat)*maxVertInput);

									for (GLint n=0; n<maxVertInput; n+=3)
									{
										memcpy(tmpVboNormalBuffer+tmpVboVertexBufferSize+n, normals[f], sizeof(GLfloat)*3);
									}

									memcpy(tmpVboTexCoordBuffer+tmpVboTexCoordBufferSize, texCoords[f], sizeof(GLfloat)*maxTexInput);	


									tmpVboVertexBufferSize+=maxVertInput;
					
									tmpVboTexCoordBufferSize+=maxTexInput;
																	
								}
							}

							if (tmpVboVertexBufferSize>0)
							{
								vbo->setElementsCount(CVBO::IDT_vertex,tmpVboVertexBufferSize/3);
								vbo->setElementsCount(CVBO::IDT_normal,tmpVboVertexBufferSize/3);
								vbo->setElementsCount(CVBO::IDT_texture0,tmpVboTexCoordBufferSize/2);
								vbo->setElementsCount(CVBO::IDT_texture1,tmpVboTexCoordBufferSize/2);
								vbo->draw();
								allVerticesCount+=tmpVboVertexBufferSize;
								tmpVboVertexBufferSize=0;
								tmpVboTexCoordBufferSize=0;						
							}
						}
					}
				}
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
						int x = CConversions::realToLogical(creature->getPosition())[0];
						int y = CConversions::realToLogical(creature->getPosition())[1];
						if((x-centerX)*(x-centerX)+(y-centerY)*(y-centerY)>300)
							continue;

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

			shaderManager->useFFPipeline();
			glDisable(GL_LIGHTING);

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

			handleMineMarker();

			CV_GAME_MANAGER->getTextPrinter()->print((GLfloat)0,(GLfloat)(CV_SETTINGS_WINDOW_HEIGHT-15*3),"Visible blocks: %d",blockVisible);
			CV_GAME_MANAGER->getTextPrinter()->print((GLfloat)0,(GLfloat)(CV_SETTINGS_WINDOW_HEIGHT-15*2),"Visible creatures: %d",creaturesVisible);
			CV_GAME_MANAGER->getTextPrinter()->print((GLfloat)0,(GLfloat)(CV_SETTINGS_WINDOW_HEIGHT-15),"Triangles drawn: %d",(allVerticesCount/4)*2);

			// render the lights representations. usefull for debugging
			CV_GAME_MANAGER->getLightingManager()->drawLightSources(frustum);
            
			return true;
		}

		bool CRenderManager1::shutdown()
		{
			delete shaderManager;

			return CRenderManager::shutdown();
		}

		string CRenderManager1::onAction(string keyword, string params)
		{	
			string checkResult = "";
			std::vector<string> tParams;

			if (keyword==PPL)
			{
				usePPL = !usePPL; //(tParams[0]=="0"?false:true);
				return "Perpixel lighting "+string(!usePPL?"disabled.":"enabled.");
			}
			else if (keyword==BLS)
			{
				drawBlockLightSources = !drawBlockLightSources;
				return "Block light sources "+string(!drawBlockLightSources?"disabled.":"enabled.");
			}
			else if (keyword==LTQA)
			{
				CLightingManager *liManager = CV_GAME_MANAGER->getLightingManager();
				GLint lightType;
				string lightName;

				if (!CConsoleListener::checkParams(params,2,checkResult,tParams))
				{					
					if (CConsoleListener::checkParams(params,1,checkResult,tParams))
					{						
						lightType = CConversions::strToInt(tParams[0]);
						lightName = liManager->lightSourceTypeIndexToName(lightType);
						return "Light "+lightName+" quadratic attenuation: "+CConversions::floatToStr(liManager->getLightParam(lightType,ATTENUATION_TYPE_QUADRATIC));
					}					
					else
					{
						return checkResult;
					}
				}									
				lightType = CConversions::strToInt(tParams[0]);
				lightName = liManager->lightSourceTypeIndexToName(lightType);
				liManager->setLightParam(lightType,ATTENUATION_TYPE_QUADRATIC,CConversions::strToFloat(tParams[1]));
				return "Quadratic attenuation for "+lightName+" light types set.";
			}
			else if (keyword==LTLA)
			{
				CLightingManager *liManager = CV_GAME_MANAGER->getLightingManager();
				GLint lightType;
				string lightName;

				if (!CConsoleListener::checkParams(params,2,checkResult,tParams))
				{					
					if (CConsoleListener::checkParams(params,1,checkResult,tParams))
					{						
						lightType = CConversions::strToInt(tParams[0]);
						lightName = liManager->lightSourceTypeIndexToName(lightType);
						return "Light "+lightName+" linear attenuation: "+CConversions::floatToStr(liManager->getLightParam(lightType,ATTENUATION_TYPE_LINEAR));
					}					
					else
					{
						return checkResult;
					}
				}									
				lightType = CConversions::strToInt(tParams[0]);
				lightName = liManager->lightSourceTypeIndexToName(lightType);
				liManager->setLightParam(lightType,ATTENUATION_TYPE_LINEAR,CConversions::strToFloat(tParams[1]));
				return "Quadratic linear for "+lightName+" light types set.";
			}
			else if (keyword==LTIA)
			{
				CLightingManager *liManager = CV_GAME_MANAGER->getLightingManager();
				GLint lightType;
				string lightName;

				if (!CConsoleListener::checkParams(params,2,checkResult,tParams))
				{					
					if (CConsoleListener::checkParams(params,1,checkResult,tParams))
					{						
						lightType = CConversions::strToInt(tParams[0]);
						lightName = liManager->lightSourceTypeIndexToName(lightType);
						return "Light "+lightName+" initial attenuation: "+CConversions::floatToStr(liManager->getLightParam(lightType,ATTENUATION_TYPE_INITIAL));
					}					
					else
					{
						return checkResult;
					}
				}									
				lightType = CConversions::strToInt(tParams[0]);
				lightName = liManager->lightSourceTypeIndexToName(lightType);
				liManager->setLightParam(lightType,ATTENUATION_TYPE_INITIAL,CConversions::strToFloat(tParams[1]));
				return "Quadratic initial for "+lightName+" light types set.";
			}
			else if (keyword==VB)
			{				
				return "Visible blocks: "+CConversions::intToStr(blockVisible);
			}
			else if (keyword==TC)
			{				
				return "Drawn triangles: "+CConversions::intToStr((allVerticesCount/4)*2);
			}			
			else if (keyword==PLT)
			{				
				return CV_GAME_MANAGER->getLightingManager()->getLightTypes();
			}	
			else if (keyword==RS)
			{				
				loadShaders();
				return "Shaders reloaded!";
			}
			return "<>";
		}
	};
};