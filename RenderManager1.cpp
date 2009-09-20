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

			if (lightsCount>0 && usePPL)
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

			// get camera
			CCamera *camera = CV_GAME_MANAGER->getControlManager()->getCamera();		
			

			// 1. extract approximate logical location of camera in the level map.
			vector2i center = CConversions::realToLogical(camera->getPosition());

			GLint centerX = center[0];
			GLint centerY = center[1];

			bool isFPS = CV_GAME_MANAGER->getControlManager()->isFPS();

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

			GLint	maxVertInput = 0,
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

			blockVisible=allVerticesCount=0;

			GLfloat delta = CV_GAME_MANAGER->getDeltaTime();	

			// transform view
			camera->transformView();

			for (GLint y=minY; y<=maxY; y++)
			{
				for (GLint x=minX; x<=maxX; x++)
				{
					block = lManager->getBlock(x,y);

					if (block)
					{
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

							for (GLint f=CBlock::BFS_FRONT; f<=CBlock::BFS_CEILING; f++)
							{
								if ((!isFPS && f==CBlock::BFS_CEILING) || (isFPS && f==CBlock::BFS_TOP))
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

									memcpy(tmpVboVertexBuffer+tmpVboVertexBufferSize, verts[f], sizeof(GLfloat)*maxVertInput);

									for (GLint n=0; n<maxVertInput; n+=3)
									{
										memcpy(tmpVboNormalBuffer+tmpVboVertexBufferSize+n, normals[f], sizeof(GLfloat)*3);
									}

									tmpVboVertexBufferSize+=maxVertInput;

									memcpy(tmpVboTexCoordBuffer+tmpVboTexCoordBufferSize, texCoords, sizeof(GLfloat)*maxTexInput);						
									tmpVboTexCoordBufferSize+=maxTexInput;
																	
								}
							}

							if (tmpVboVertexBufferSize>0)
							{
								vbo->setElementsCount(CVBO::IDT_vertex,tmpVboVertexBufferSize/3);
								vbo->setElementsCount(CVBO::IDT_normal,tmpVboVertexBufferSize/3);
								vbo->setElementsCount(CVBO::IDT_texture0,tmpVboTexCoordBufferSize/2);
								vbo->draw();
								allVerticesCount+=tmpVboVertexBufferSize;
								tmpVboVertexBufferSize=0;
								tmpVboTexCoordBufferSize=0;						
							}
						}
					}
				}
			}

			shaderManager->useFFPipeline();
			glDisable(GL_LIGHTING);

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