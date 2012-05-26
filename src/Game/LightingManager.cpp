#include "commons.h"
#include "LightingManager.h"
#include <algorithm>
#include "BasicLight.h"
#include "Conversions.h"

using namespace cml;
using namespace game_objects;
using namespace game_objects::lighting;
using namespace game_utils;
using namespace geometry;
using namespace control;
using namespace std;

#define LOC		"LOC"
#define LBBOX	"LBBOX"

namespace game_utils
{
	namespace managers
	{
		CLightingManager::CLightingManager(): CManager(), CConsoleListener()
		{
			drawBBOX=false;
		}

		CLightingManager::~CLightingManager()
		{
		}

		bool CLightingManager::init()
		{			
			// register to console
			CV_GAME_MANAGER->getConsole()->registerClass(this,"LIGHTING MANAGER");
			CV_GAME_MANAGER->getConsole()->addParam(LOC,"() Returns the number of light source currently being processed.");
			CV_GAME_MANAGER->getConsole()->addParam(LBBOX,"(1|0) Enables|Disables light object bbox.");


			// UPDATE: when more light types are available
			lightIndexName.clear();
			lightIndexName[0]=CV_LIGHT_TYPE_TORCH;
			lightIndexName[1]=CV_LIGHT_TYPE_CANDLE;
			lightIndexName[2]=CV_LIGHT_TYPE_LAVA;
			lightIndexName[3]=CV_LIGHT_TYPE_WATER;

			return true;
		}

		bool CLightingManager::update()
		{
			return true;
		}

		bool CLightingManager::shutdown()
		{
			// a must, we get exception otherwise
			lightSources.clear();

			// handle clearing of light-block assignments
			for (GLuint y=0; y<CV_LEVEL_MAP_SIZE; y++)
			{
				for (GLuint x=0; x<CV_LEVEL_MAP_SIZE; x++)
				{
					blockLightData[y][x].reset();
				}
			}

			return true;
		}

		CBlockLightData *CLightingManager::getBlockLightData(vector2i blockPos)
		{
			return &blockLightData[blockPos[1]][blockPos[0]];
		}

		string CLightingManager::lightSourceTypeIndexToName(GLint lightSourceType)
		{
			map<GLint,string>::iterator mapIter = lightIndexName.find(lightSourceType);
			return (mapIter!=lightIndexName.end()?mapIter->second:"ERR");
		}

		GLint CLightingManager::lightSourceNameToTypeIndex(string lightSourceName)
		{
			for (map<GLint,string>::iterator mapIter=lightIndexName.begin(); mapIter!=lightIndexName.end(); mapIter++)
			{
				if (mapIter->second==lightSourceName)
				{
					return mapIter->first;
				}
			}
			return -1;
		}

		string CLightingManager::getLightTypes()
		{
			string result = "";

			for (map<GLint,string>::iterator mapIter=lightIndexName.begin(); mapIter!=lightIndexName.end(); mapIter++)
			{
				result+="("+CConversions::intToStr(mapIter->first)+")"+mapIter->second+", ";
			}

			result=result.substr(0,result.size()-2);

			return result;
		}

		GLvoid CLightingManager::setLightParam(GLint lightSourceType, GLint attenuationType, GLfloat val)
		{
			string sName = lightSourceTypeIndexToName(lightSourceType);

			for (lightSourcesIter=lightSources.begin(); lightSourcesIter!=lightSources.end(); ++lightSourcesIter)
			{
				if ((*lightSourcesIter)->getName()==sName)
				{
					(*lightSourcesIter)->setAttenuation(attenuationType,val);
				}
			}
		}

		GLfloat CLightingManager::getLightParam(GLint lightSourceType, GLint attenuationType)
		{
			string sName = lightSourceTypeIndexToName(lightSourceType);

			for (lightSourcesIter=lightSources.begin(); lightSourcesIter!=lightSources.end(); ++lightSourcesIter)
			{
				if ((*lightSourcesIter)->getName()==sName)
				{
					return (*lightSourcesIter)->getAttenuation(attenuationType);
				}
			}

			// if there is no such light type present in the level, try loading it from the pool
			//CLightSource *light = lightSourcePool[lightSourceType].popObject(); TODO
			GLfloat att = 0.0f;
			/*if (light)
			{
				att = light->getAttenuation(attenuationType);
				lightSourcePool[lightSourceType].pushObject(light);
			}*/			
			return att;
		}

		GLvoid CLightingManager::setLightParam(GLint lightSourceType, GLint colorType, cml::vector3f color)
		{
			/* 
				No need to update the lights in the light pool since this
				methods will only get used in development time to determine
				proper params for some properties.
			*/
			string sName = lightSourceTypeIndexToName(lightSourceType);

			for (lightSourcesIter=lightSources.begin(); lightSourcesIter!=lightSources.end(); ++lightSourcesIter)
			{
				if ((*lightSourcesIter)->getName()==sName)
				{
					if (colorType==COLOR_TYPE_AMBIENT)
					{
						(*lightSourcesIter)->setAmbientColor(color);
					}
					else if (colorType==COLOR_TYPE_DIFFUSE)
					{
						(*lightSourcesIter)->setDiffuseColor(color);
					}
				}
			}
		}

		GLvoid CLightingManager::addLightSource(vector2i position, string lightSourceName)
		{
			addLightSource(position,lightSourceNameToTypeIndex(lightSourceName));
		}

		GLvoid CLightingManager::addLightSource(vector2i position, GLint lightSourceType)
		{
			// depending on the block position we need to setup from 1 to 4 lights
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();
				
			GLint faces[4];
			GLint bfVis = lManager->getBlock(position)->getFreeFaces(faces);

			vector3f lPoss[] =
			{
				vector3f(CV_BLOCK_WIDTH/2,CV_BLOCK_HEIGHT,CV_BLOCK_DEPTH+CV_BLOCK_DEPTH/10.0f),		// front
				vector3f(CV_BLOCK_WIDTH/2,CV_BLOCK_HEIGHT,-CV_BLOCK_DEPTH/10.0f),					// back
				vector3f(CV_BLOCK_WIDTH+CV_BLOCK_DEPTH/10.0f,CV_BLOCK_HEIGHT,CV_BLOCK_DEPTH/2.0f),	// left
				vector3f(-CV_BLOCK_DEPTH/10.0f,CV_BLOCK_HEIGHT,CV_BLOCK_DEPTH/2.0f),				// right
				vector3f(CV_BLOCK_DEPTH/2.0f,CV_BLOCK_HEIGHT,CV_BLOCK_DEPTH/2.0f)					// middle
			};

			if (bfVis==-1)
			{
				bfVis=1;
				faces[0]=4;
			}
				
			for (GLint i=0; i<bfVis; i++)
			{
				// get a light from the light pool
				//CLightSource *light = lightSourcePool[lightSourceType].popObject();
				CLightSource *light = new CLightSource(lightIndexName[lightSourceType]);
				if (light)
				{
					// set its position
					light->setPosition(lManager->getBlock(position)->getRealPosition()+lPoss[faces[i]]);

					// append to block
					//blockLightData[position[1]][position[0]].add(light);
					//blockLightData[position[1]][position[0]].add(light,lManager->getBlock(position));

					// and add to the global list of used lights
					lightSources.push_back(light);

					GLint lightRadius = (int)ceil(light->getRadius()/CV_BLOCK_WIDTH)+2; // +2 is there for extended radius comparison

					//vector2i lightPos = CConversions::realToLogical(light->getPosition());

					// now for every block of this light, add references of this light to influenced neighbour blocks.
					for (GLint y=-lightRadius; y<=lightRadius; y++)
					{
						for (GLint x=-lightRadius; x<=lightRadius; x++)
						{			
							if (position[1]+y<0 || position[0]+x<0 || position[1]+y>=84 || position[0]+x>=84)
							{
								continue;
							}

							if (faces[i]==CBlock::BFS_FRONT && y<0)
							{
								continue;
							}

							if (faces[i]==CBlock::BFS_BACK && y>0)
							{
								continue;
							}

							if (faces[i]==CBlock::BFS_LEFT && x<0)
							{
								continue;
							}

							if (faces[i]==CBlock::BFS_RIGHT && x>0)
							{
								continue;
							}

							CBlock *block = lManager->getBlock(position[0]+x,position[1]+y);

							vector3f lPos(light->getPosition()[0],0.0f,light->getPosition()[2]);

							GLfloat d1 = (block->getRealPosition()-lPos).length();
							GLfloat d2 = ((block->getRealPosition()+vector3f(CV_BLOCK_WIDTH,0.0f,0.0f))-lPos).length();
							GLfloat d3 = ((block->getRealPosition()+vector3f(CV_BLOCK_WIDTH,0.0f,CV_BLOCK_DEPTH))-lPos).length();
							GLfloat d4 = ((block->getRealPosition()+vector3f(0.0f,0.0f,CV_BLOCK_DEPTH))-lPos).length();

							GLfloat rad =light->getRadius();

							if (d1<=rad || d2<=rad || d3<=rad || d4<=rad)
							{
								blockLightData[position[1]+y][position[0]+x].add(light,block);
							}
						}
					}
				}
			}	
		}

		GLvoid CLightingManager::drawLightSources(CFrustum *viewFrustum)
		{
			CLightSource *light = NULL;

			GLfloat lightBoxSize = (CV_BLOCK_WIDTH / 3.0f) / 2.0f;

			visibleLightObjects = 0;

			for (lightSourcesIter=lightSources.begin(); lightSourcesIter!=lightSources.end(); ++lightSourcesIter)
			{
				light = *lightSourcesIter;

				//if (viewFrustum->containsBBOX(light->getBoundingBox()))
				if (viewFrustum->containsPoint(light->getPosition()))
				{
					glPushMatrix();
					glTranslatef(light->getPosition()[0],light->getPosition()[1],light->getPosition()[2]);
					/*if (light->getModel()) TODO
					{
						// some lights have no models
						light->getModel()->draw();
					}*/
					glPopMatrix();		

					visibleLightObjects++;
				
					// bboxes
					if (drawBBOX)
					{
						vector3f a,b,c,d,e,f,g,h,p;
						
						p = light->getPosition();

						a = p + vector3f(-lightBoxSize,lightBoxSize,lightBoxSize);
						b = p + vector3f(lightBoxSize,lightBoxSize,lightBoxSize);
						c = p + vector3f(lightBoxSize,lightBoxSize,-lightBoxSize);
						d = p + vector3f(-lightBoxSize,lightBoxSize,-lightBoxSize);

						e = p + vector3f(-lightBoxSize,-lightBoxSize,lightBoxSize);
						f = p + vector3f(lightBoxSize,-lightBoxSize,lightBoxSize);
						g = p + vector3f(lightBoxSize,-lightBoxSize,-lightBoxSize);
						h = p + vector3f(-lightBoxSize,-lightBoxSize,-lightBoxSize);

						// draw a simple light representation.
						glColor3f(1.0f,1.0f,1.0f);
						glLineWidth(2.0f);
						glBegin(GL_LINES);
						{
							glVertex3fv(&a[0]);
							glVertex3fv(&b[0]);

							glVertex3fv(&b[0]);
							glVertex3fv(&c[0]);

							glVertex3fv(&c[0]);
							glVertex3fv(&d[0]);

							glVertex3fv(&d[0]);
							glVertex3fv(&a[0]);

							glVertex3fv(&e[0]);
							glVertex3fv(&f[0]);

							glVertex3fv(&f[0]);
							glVertex3fv(&g[0]);

							glVertex3fv(&g[0]);
							glVertex3fv(&h[0]);

							glVertex3fv(&h[0]);
							glVertex3fv(&e[0]);

							glVertex3fv(&a[0]);
							glVertex3fv(&e[0]);

							glVertex3fv(&b[0]);
							glVertex3fv(&f[0]);

							glVertex3fv(&c[0]);
							glVertex3fv(&g[0]);

							glVertex3fv(&d[0]); 
							glVertex3fv(&h[0]);
						}
						glEnd();
						glLineWidth(1.0f);
						glColor3f(1.0f,1.0f,1.0f);
					}
				}
			}
		}

		string CLightingManager::onAction(string keyword, string params)
		{
			string checkResult = "";
			std::vector<string> tParams;

			if (keyword==LBBOX)
			{
				if (!CConsoleListener::checkParams(params,1,checkResult,tParams))
				{
					return checkResult;
				}

				drawBBOX = (tParams[0]=="0"?false:true);
				return "Light objects bbox "+string(tParams[0]=="0"?"disabled.":"enabled.");
			}
			else if (keyword==LOC)
			{				
				return "Light objects: "+CConversions::intToStr(visibleLightObjects);
			}

			return "<>";
		}

		/* CBlockLightData */
		CBlockLightData::CBlockLightData(): lightsCount(0)
		{
			ZeroMemory(lightSources,sizeof(CLightSource*)*MAX_LIGHTS_PER_BLOCK);
		}

		CBlockLightData::~CBlockLightData()
		{
		}

		GLubyte CBlockLightData::getLightsCount()
		{
			return lightsCount;
		}

		CLightSource *CBlockLightData::getLightSource(GLint index)
		{
			return lightSources[index];
		}

		GLvoid CBlockLightData::reset()
		{
			ZeroMemory(lightSources,sizeof(CLightSource*)*MAX_LIGHTS_PER_BLOCK);
			lightsCount=0;
		}

		GLvoid CBlockLightData::add(CLightSource *newLightSource, CBlock *targetBlock)
		{					
			/*
				Every time a new light gets added we check is it's closer to a block
				that any other light. This way we keep the lights sorted by distance to the
				block and those too far away get eliminated automatically.
			*/

			if (lightsCount<MAX_LIGHTS_PER_BLOCK)
			{
				lightSources[lightsCount++]=newLightSource;
			}

			/*if (lightsCount==0)// TODO
			{
				lightSources[lightsCount++]=newLightSource;
			}
			else
			{
				GLfloat targetMinDistance = (targetBlock->getCenterPosition()-newLightSource->getPosition()).length();

				for (GLint _l=0; _l<lightsCount; _l++)
				{
					GLfloat distance = (targetBlock->getCenterPosition()-lightSources[_l]->getPosition()).length();

					if (targetMinDistance<distance)
					{
						// found an apropriate match. shiftl&&replace
						for (GLint m=MAX_LIGHTS_PER_BLOCK-1; m>_l; m--)
						{
							lightSources[m]=lightSources[m-1];
						}
						lightSources[_l]=newLightSource;

						if (lightsCount<MAX_LIGHTS_PER_BLOCK)
						{
							lightsCount++;
						}
						break;
					}
				}
			}*/

			/*if (lightSources[0]!=NULL&&lightSources[1]!=NULL&&lightSources[2]!=NULL&&lightSources[3]!=NULL)
			{
				GLfloat distance0 = (targetBlock->getCenterPosition()-lightSources[0]->getPosition()).length();
				GLfloat distance1 = (targetBlock->getCenterPosition()-lightSources[1]->getPosition()).length();
				GLfloat distance2 = (targetBlock->getCenterPosition()-lightSources[2]->getPosition()).length();
				GLfloat distance3 = (targetBlock->getCenterPosition()-lightSources[3]->getPosition()).length();				

				CV_GAME_MANAGER->getConsole()->writeLine("D0: "+CConversions::floatToStr(distance0));
				CV_GAME_MANAGER->getConsole()->writeLine("D1: "+CConversions::floatToStr(distance1));
				CV_GAME_MANAGER->getConsole()->writeLine("D2: "+CConversions::floatToStr(distance2));
				CV_GAME_MANAGER->getConsole()->writeLine("D3: "+CConversions::floatToStr(distance3));				
				CV_GAME_MANAGER->getConsole()->writeLine("***");
			}*/
		}

	};
};