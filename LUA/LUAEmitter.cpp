#include "../commons.h"
#include "LUAEmitter.h"
#include "../utils.h"
#include "lua_common.h"
#include <cml/cml.h>
#include <fstream>
#include "../TextureLoader.h"

using namespace std;
using namespace cml;
using namespace utils;
using namespace game_utils;
using namespace loaders;

namespace LUA_effects
{
	CLUAEmitter *G_currentTempEmitter=NULL;

	CLUAEmitter::CLUAEmitter(): CLUAParticle()
	{
		particles=NULL;
		pCount=0;
		particleInitLUAFile="";
		followPath=false;
		pathPos=0;
		pathMoveSpeed=0.0f;
		transparent=true;	
		depthTest=true;
		timeOut=false;
		connectAlpha=false;
		shakeCamera=false;
		scale=currentTimeOut=startTimeOut=1.0f;
		oldPos=NULL_VEC;
		shakeStrength=1.0f;

		initLUAFile = "data/LUA_effects/0/emitter0_init_test.lua";
		updateLUAFile = "data/LUA_effects/0/emitter0_update_test.lua";	
	}

	// update
	int EMITTER_UPDATE_POSITION(lua_State *L)
	{
		if (!G_currentTempEmitter->doFollowPath())
		{
			G_currentTempEmitter->update_position(vector3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3)));
		}
		return 0;
	}

	int EMITTER_SET_GLOBAL_PARAMS_COUNT(lua_State *L)
	{
		std::vector<int> *globalLUAParams = G_currentTempEmitter->getGlobalLUAParams();
		if (globalLUAParams->size()==0)
		{
			globalLUAParams->resize((GLint)lua_tonumber(L,1));

			// init array
			for(std::vector<int>::iterator iter=globalLUAParams->begin(); iter!=globalLUAParams->end(); iter++)
			{
				*iter=0;
			}
		}
		return 0;
	}

	// init
	int EMITTER_SET_PARTICLE_COUNT(lua_State *L)
	{
		G_currentTempEmitter->setParticleCount((GLint)lua_tonumber(L,1));
		return 0;
	}

	int EMITTER_SET_START_POSITION(lua_State *L)
	{
		if (!G_currentTempEmitter->doFollowPath())
		{
			G_currentTempEmitter->set_start_position(vector3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3)));
		}
		return 0;
	}

	int EMITTER_SET_MOVE_VECTOR(lua_State *L)
	{
		G_currentTempEmitter->set_move_vector(vector3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3)));
		return 0;
	}

	int EMITTER_SET_LIFE_PROPERTIES(lua_State *L)
	{
		G_currentTempEmitter->set_life_properties((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
		return 0;
	}

	int EMITTER_SET_REBORN(lua_State *L)
	{
		G_currentTempEmitter->set_reborn((GLint)lua_tonumber(L,1)==1);
		return 0;
	}

	int EMITTER_SET_PARTICLE_INIT_LUA(lua_State *L)
	{
		G_currentTempEmitter->setParticleInitLUA(string(lua_tostring(L,1)));
		return 0;
	}

	int EMITTER_SET_FOLLOW_PATH(lua_State *L)
	{
		G_currentTempEmitter->setFollowPath(true);
		G_currentTempEmitter->setPathFile(string(lua_tostring(L,1)));
		G_currentTempEmitter->setPathMoveSpeed((GLfloat)lua_tonumber(L,2));
		return 0;
	}

	int EMITTER_ADD_PARTICLE_GRAVITY_POINT(lua_State *L)
	{		
		vector3f position((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
		GLfloat strength = (GLfloat)lua_tonumber(L,4);
		G_currentTempEmitter->addGravityPoint(new CLUAEmitter::GravityPoint(position,strength));
		return 0;
	}

	int EMITTER_ADD_PARTICLE_DEFLECTOR(lua_State *L)
	{		
		vector3f position((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3));
		GLfloat strength = (GLfloat)lua_tonumber(L,4);

		G_currentTempEmitter->addDeflector(new CLUAEmitter::Deflector(position,strength));
		
		return 0;
	}

	int EMITTER_SET_PARTICLE_TEXTURE(lua_State *L)
	{		
		G_currentTempEmitter->setParticleTexture(string(lua_tostring(L,1)));
		return 0;
	}

	int EMITTER_SET_PARTICLE_ANIMATED_TEXTURE(lua_State *L)
	{				
		G_currentTempEmitter->setParticleAnimatedTexture(string(lua_tostring(L,1)),(GLint)lua_tonumber(L,2),(GLint)lua_tonumber(L,3),(GLfloat)lua_tonumber(L,4));
		return 0;
	}

	int EMITTER_LOAD_PARTICLE_START_POSITION(lua_State *L)
	{		
		G_currentTempEmitter->loadParticlePositions(string(lua_tostring(L,1)));
		return 0;
	}

	int EMITTER_SET_PARTICLE_SCALE(lua_State *L)
	{		
		G_currentTempEmitter->setScale((GLfloat)lua_tonumber(L,1));		
		return 0;
	}

	int EMITTER_SET_TIMEOUT(lua_State *L)
	{		
		G_currentTempEmitter->setTimeOut((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
		return 0;
	}

	int EMITTER_SET_CAMERA_SHAKE(lua_State *L)
	{		
		G_currentTempEmitter->setCameraShake((GLfloat)lua_tonumber(L,1));
		return 0;
	}

	CLUAEmitter::~CLUAEmitter()
	{
		//lua_close(LUA_update); ERR?
		SAFE_DELETE_ARRAY_ELEMENTS(particles,this->pCount);
		SAFE_DELETE_ARRAY(particles);

		if (gravityPoints.size()>0)
		{
			for (gravityPointsIter=gravityPoints.begin(); gravityPointsIter!=gravityPoints.end(); gravityPointsIter++)
			{
				delete *gravityPointsIter;
			}
			gravityPoints.clear();
		}

		if (deflectorPoints.size()>0)
		{
			for (deflectorPointsIter=deflectorPoints.begin(); deflectorPointsIter!=deflectorPoints.end(); deflectorPointsIter++)
			{
				delete *deflectorPointsIter;
			}
			deflectorPoints.clear();
		}
	}

	GLvoid CLUAEmitter::setParticleCount(GLint pCount)
	{
		SAFE_DELETE_ARRAY_ELEMENTS(particles,this->pCount);
		SAFE_DELETE_ARRAY(particles);

		this->pCount=pCount;

		particles = new CLUAParticle*[pCount];
		for (GLint i=0; i<pCount; i++)
		{
			particles[i]=new CLUAParticle();

			particles[i]->set_parent((GLvoid*)this);

			if (particleInitLUAFile.length()!=0)
			{
				particles[i]->setEffectDirectory(getEffectDirectory());
				particles[i]->setLUAFile(particleInitLUAFile);				
			}

			if (particleTexture!=0)
			{
				particles[i]->set_texture(particleTexture);
			}			
		}
		setParticlePositions();
	}

	GLvoid CLUAEmitter::init()
	{
		G_currentTempEmitter=this; // MUST

		gravityPoints.clear();
		deflectorPoints.clear();

		// init lua stuff
		LUA_init = lua_open();
		luaL_openlibs(LUA_init);
		lua_register(LUA_init,"EMITTER_SET_PARTICLE_COUNT",EMITTER_SET_PARTICLE_COUNT);
		lua_register(LUA_init,"EMITTER_SET_START_POSITION",EMITTER_SET_START_POSITION);
		lua_register(LUA_init,"EMITTER_SET_MOVE_VECTOR",EMITTER_SET_MOVE_VECTOR);
		lua_register(LUA_init,"EMITTER_SET_LIFE_PROPERTIES",EMITTER_SET_LIFE_PROPERTIES);
		lua_register(LUA_init,"EMITTER_SET_REBORN",EMITTER_SET_REBORN);
		lua_register(LUA_init,"EMITTER_SET_PARTICLE_INIT_LUA",EMITTER_SET_PARTICLE_INIT_LUA);
		lua_register(LUA_init,"EMITTER_SET_FOLLOW_PATH",EMITTER_SET_FOLLOW_PATH);
		lua_register(LUA_init,"EMITTER_ADD_PARTICLE_GRAVITY_POINT",EMITTER_ADD_PARTICLE_GRAVITY_POINT);
		lua_register(LUA_init,"EMITTER_SET_PARTICLE_TEXTURE",EMITTER_SET_PARTICLE_TEXTURE);
		lua_register(LUA_init,"EMITTER_SET_PARTICLE_ANIMATED_TEXTURE",EMITTER_SET_PARTICLE_ANIMATED_TEXTURE);
		lua_register(LUA_init,"EMITTER_ADD_PARTICLE_DEFLECTOR",EMITTER_ADD_PARTICLE_DEFLECTOR);
		lua_register(LUA_init,"EMITTER_LOAD_PARTICLE_START_POSITION",EMITTER_LOAD_PARTICLE_START_POSITION);
		lua_register(LUA_init,"EMITTER_SET_PARTICLE_SCALE",EMITTER_SET_PARTICLE_SCALE);
		lua_register(LUA_init,"EMITTER_SET_TIMEOUT",EMITTER_SET_TIMEOUT);
		lua_register(LUA_init,"EMITTER_SET_CAMERA_SHAKE",EMITTER_SET_CAMERA_SHAKE);		

		string fullPath = G_startDir+"\\"+initLUAFile;

		__luaL_dofile(LUA_init, fullPath.c_str());		
		lua_close(LUA_init);		
		
		for (GLint i=0; i<pCount; i++)
		{
			particles[i]->set_start_position(particles[i]->get_start_position()+current_position);
			particles[i]->init(i,pCount);			
		}

		// update lua stuff
		LUA_update = lua_open();
		luaL_openlibs(LUA_update);
		lua_register(LUA_update,"EMITTER_UPDATE_POSITION",EMITTER_UPDATE_POSITION);
		lua_register(LUA_update,"EMITTER_SET_GLOBAL_PARAMS_COUNT",EMITTER_SET_GLOBAL_PARAMS_COUNT);

		fullPath = G_startDir+"\\"+updateLUAFile;

		// read initial values from LUA
		__luaL_dofile(LUA_update, fullPath.c_str());
		__popLuaGlobals();
	}

	bool CLUAEmitter::is_dead()
	{
		if (dead)
		{
			return true;
		}

		for (GLint i=0; i<pCount; i++)
		{
			if (!particles[i]->is_dead())
			{
				return false;
			}
		}
		return true;
	}

	GLvoid CLUAEmitter::draw()
	{
		glPushMatrix();
		glScalef(scale,scale,scale);

		if (transparent)
		{
			glDepthMask(false);		
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glEnable(GL_TEXTURE_2D);
		}

		if (!depthTest)
		{
			glDisable(GL_DEPTH_TEST);
		}

		for (GLint i=0; i<pCount; i++)
		{
			particles[i]->draw();

			particles[i]->set_start_position_only(particles[i]->get_start_position()+(current_position-oldPos));
		}

		if (!depthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}

		if (transparent)
		{
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glDepthMask(true);
		}
		glPopMatrix();
	}

	// reads global variables from LUA
	GLvoid CLUAEmitter::__popLuaGlobals()
	{
		lua_getglobal(LUA_update, "onEnd");

		lua_call(LUA_update, 0, globalLUAParams.size());

		for (GLint i=globalLUAParams.size()-1; i>=0; i--)
		{
			globalLUAParams[i]=(int)lua_tointeger(LUA_update, -1);
			lua_pop(LUA_update, 1);
		}
	}

	// pushes global variables back to LUA
	GLvoid CLUAEmitter::__pushLuaGlobals()
	{
		lua_getglobal(LUA_update, "onBegin");
		for(std::vector<int>::iterator iter=globalLUAParams.begin(); iter!=globalLUAParams.end(); iter++)
		{
			lua_pushnumber(LUA_update,*iter);
		}
		lua_call(LUA_update, globalLUAParams.size(), 0);
	}

	GLvoid CLUAEmitter::__updateLUAEmitter()
	{
		lua_getglobal(LUA_update,"onUpdate");
		lua_pcall(LUA_update,0,0,0);
	}

	GLvoid CLUAEmitter::doLUAUpdate()
	{
		G_currentTempEmitter=this; // MUST

		__luaL_dofile(LUA_update, (G_startDir+"\\"+updateLUAFile).c_str());

		__pushLuaGlobals();

		__updateLUAEmitter();

		__popLuaGlobals();	
	}

	GLvoid CLUAEmitter::update()
	{
		oldPos=current_position;

		//doLUAUpdate(); PROBLEMATIC SLOWDOWN!

		GLfloat delta = CV_GAME_MANAGER->getDeltaTime();

		if (followPath)
		{
			// move along the path
			vector3f target = pathPoins[pathPos];
			vector3f mv = (target-current_position);
			mv.normalize();
			GLfloat dmv = pathMoveSpeed*delta;
			mv*=dmv;
			current_position+=mv;

			if (fabs(current_position[0]-target[0])<=dmv &&
				fabs(current_position[1]-target[1])<=dmv &&
				fabs(current_position[2]-target[2])<=dmv)
			{
				current_position=target;
				pathPos++;
				if (pathPos>=(GLint)pathPoins.size())
				{
					pathPos=0;
				}
			}
		}

		for (GLint i=0; i<pCount; i++)
		{
			bool nospawn = particles[i]->get_current_spawn_delay()>0.0f;
			particles[i]->update();

			if (particles[i]->get_current_spawn_delay()<=0.0f && nospawn)
			{
				// a particle just became spawnable, adjust its starting pos
				particles[i]->set_start_position(current_position);
			}
		}

		// every particle gets affected by gravity point(s)
		GLfloat pMass = 1.0f;
		GLfloat G = (GLfloat)(6.6726e-5);
		vector3f diff;
		GLfloat F;

		if (gravityPoints.size()>0)
		{
			for (gravityPointsIter=gravityPoints.begin(); gravityPointsIter!=gravityPoints.end(); gravityPointsIter++)
			{
				GravityPoint *gp = *gravityPointsIter;

				for (GLint i=0; i<pCount; i++)
				{
					diff = gp->position-particles[i]->get_current_position();
					diff.normalize();

					// calc Newtons force
					F = (G*pMass*gp->mass)/(gp->position-particles[i]->get_start_position()).length_squared();

					diff*=F;

					particles[i]->update_move_vector(diff*delta);
				}			
			}
		}

		// and by every deflector
		if (deflectorPoints.size()>0)
		{
			for (deflectorPointsIter=deflectorPoints.begin(); deflectorPointsIter!=deflectorPoints.end(); deflectorPointsIter++)
			{
				Deflector *gp = *deflectorPointsIter;

				for (GLint i=0; i<pCount; i++)
				{
					diff = gp->position-particles[i]->get_current_position();
					diff.normalize();

					// calc Newtons force

					F = (G*pMass*gp->mass)/(gp->position-particles[i]->get_start_position()).length_squared();

					diff*=F;

					particles[i]->update_move_vector(diff*delta);
				}			
			}
		}

		// if there is an effect time out we need to check it effect is finished. then just kill all particles
		if (timeOut)
		{
			currentTimeOut-=0.1f*delta;

			if (currentTimeOut<=0.0f)
			{
				// effect timed out
				for (GLint i=0; i<pCount; i++)
				{
					particles[i]->set_dead(true);
				}
			}

			if (connectAlpha)
			{
				current_alpha=currentTimeOut/startTimeOut;

				if (current_alpha<0.0f)
				{
					current_alpha=0.0f;
				}
			}
		}

		if (shakeCamera)
		{
			vector3f cPos = CV_GAME_MANAGER->getControlManager()->getCamera()->getPosition();

			cPos[0]+=(GLfloat)(rand()%100-50)*shakeStrength;
			cPos[1]+=(GLfloat)(rand()%100-50)*shakeStrength;
			cPos[2]+=(GLfloat)(rand()%100-50)*shakeStrength;

			CV_GAME_MANAGER->getControlManager()->getCamera()->setPosition(cPos);
		}
	}

	GLvoid CLUAEmitter::reset()
	{		
		// reset time out
		currentTimeOut=startTimeOut;
		CLUAParticle::reset();		
	
		for (GLint i=0; i<pCount; i++)
		{
			particles[i]->set_start_position_only(particles[i]->get_start_position()+(current_position-oldPos));
			particles[i]->reset();			
		}
		oldPos=current_position;

		if (followPath)
		{
			pathPos=1;
			set_start_position(*pathPoins.begin());
			oldPos=start_position;
		}
	}

	GLint CLUAEmitter::getParticleCount()
	{
		return pCount;
	}

	GLvoid CLUAEmitter::setParticleInitLUA(string particleInitLUAFile)
	{
		this->particleInitLUAFile=particleInitLUAFile;

		if (particles)
		{
			for (GLint i=0; i<pCount; i++)
			{
				particles[i]->setEffectDirectory(getEffectDirectory());
				particles[i]->setLUAFile(particleInitLUAFile);							

				if (particleTexture!=0)
				{
					particles[i]->set_texture(particleTexture);
				}

				setParticlePositions();
			}
		}
	}

	std::vector<int> *CLUAEmitter::getGlobalLUAParams()
	{
		return &globalLUAParams;
	}

	string CLUAEmitter::getInitLUAFile()
	{
		return initLUAFile;
	}

	string CLUAEmitter::getUpdateLUAFile()
	{
		return updateLUAFile;
	}

	GLvoid CLUAEmitter::setInitLUAFile(string initLUAFile)
	{
		string fullDirectory = sGeneralUtils::getCurrentDirectory();
		string effectDir=fullDirectory.substr(fullDirectory.find_first_of(G_startDir)+G_startDir.length()+1);
		this->setEffectDirectory(effectDir);
		this->initLUAFile=effectDir+"\\"+initLUAFile;
	}

	GLvoid CLUAEmitter::setUpdateLUAFile(string updateLUAFile)
	{
		string fullDirectory = sGeneralUtils::getCurrentDirectory();
		string effectDir=fullDirectory.substr(fullDirectory.find_first_of(G_startDir)+G_startDir.length()+1);
		this->updateLUAFile=effectDir+"\\"+updateLUAFile;
	}

	GLvoid CLUAEmitter::setInitLUAFile(string effectDir, string initLUAFile)
	{
		this->setEffectDirectory(effectDir);
		this->initLUAFile=effectDir+"\\"+initLUAFile;
	}

	GLvoid CLUAEmitter::setUpdateLUAFile(string effectDir, string updateLUAFile)
	{
		this->setEffectDirectory(effectDir);
		this->updateLUAFile=effectDir+"\\"+updateLUAFile;
	}

	bool CLUAEmitter::doFollowPath()
	{
		return followPath;
	}

	GLvoid CLUAEmitter::setFollowPath(bool followPath)
	{
		this->followPath=followPath;
	}

	GLvoid CLUAEmitter::setPathFile(string pathFile)
	{
		string _pathFile=G_startDir+"\\"+effectDirectory+"\\"+pathFile;

		if (pathFile.find(".ms3d")!=string::npos)
		{
			// not needed
		}
		else
		{

			// load path points
			ifstream file;
			file.open(_pathFile.c_str());
			string line="";
			vector3f pnt;
			pathPoins.clear();
			while (file>>line)
			{
				sscanf(line.c_str(),"%f,%f,%f",&pnt[0],&pnt[1],&pnt[2]);
				pathPoins.push_back(pnt);
			}
			file.close();
		}
		pathPos=1;
		set_start_position(*pathPoins.begin());
		oldPos=start_position;
	}

	GLvoid CLUAEmitter::setPathMoveSpeed(GLfloat pathMoveSpeed)
	{
		this->pathMoveSpeed=pathMoveSpeed;
	}

	GLvoid CLUAEmitter::addGravityPoint(CLUAEmitter::GravityPoint *gp)
	{
		gravityPoints.push_back(gp);
	}

	GLvoid CLUAEmitter::addDeflector(CLUAEmitter::Deflector *dp)
	{
		deflectorPoints.push_back(dp);
	}

	GLvoid CLUAEmitter::setParticleTexture(string particleTexture)
	{
		if (particleTexture.length()>0)
		{
			CTextureLoader::buildTexture((char*)(G_startDir+"\\"+effectDirectory+"\\"+particleTexture).c_str(),this->particleTexture,true,GL_LINEAR);

			for (GLint i=0; i<pCount; i++)
			{
				if (particles[i])
				{
					particles[i]->set_texture(this->particleTexture);
				}
			}
		}
		else
		{
			this->particleTexture=0;
		}
	}

	GLvoid CLUAEmitter::setParticleAnimatedTexture(string particleTexture, GLint count_x, GLint count_y, GLfloat speed)
	{
		if (particleTexture.length()>0)
		{
			CTextureLoader::buildTexture((char*)(G_startDir+"\\"+effectDirectory+"\\"+particleTexture).c_str(),this->particleTexture,true,GL_LINEAR);

			for (GLint i=0; i<pCount; i++)
			{
				if (particles[i])
				{
					particles[i]->set_texture(this->particleTexture,count_x,count_y,speed);
				}
			}
		}
		else
		{
			this->particleTexture=0;
		}
	}

	GLvoid CLUAEmitter::loadParticlePositions(string fileName)
	{
		particleStartPositions.clear();

		string path=G_startDir+"\\"+effectDirectory+"\\"+fileName;

		if (fileName.find(".ms3d")!=string::npos)
		{
			// not 
		}
		else
		{
			ifstream file;
			file.open(path.c_str());
			string line;
			vector3f pnt;

			while (file>>line)
			{
				sscanf(line.c_str(),"%f,%f,%f",&pnt[0],&pnt[1],&pnt[2]);
				particleStartPositions.push_back(pnt);
			}
		}

		setParticlePositions();
	}

	GLvoid CLUAEmitter::setParticlePositions()
	{
		if (particleStartPositions.size()>0 && particles)
		{
			GLint pos=0;
			for (GLint i=0; i<pCount; i++)
			{
				particles[i]->set_start_position(particleStartPositions[i%particleStartPositions.size()]);
				if (pos>=(GLint)particleStartPositions.size())
				{
					pos=0;
				}
			}
		}
	}

	GLvoid CLUAEmitter::setTransparent(bool transparent)
	{
		this->transparent=transparent;
	}

	GLvoid CLUAEmitter::setDepthTest(bool depthTest)
	{
		this->depthTest=depthTest;
	}

	GLvoid CLUAEmitter::setScale(GLfloat scale)
	{
		this->scale=scale;
	}

	GLvoid CLUAEmitter::setTimeOut(GLfloat timeOut, GLfloat connectAlpha)
	{
		this->startTimeOut=timeOut;
		this->currentTimeOut=timeOut;
		this->timeOut=true;
		this->current_alpha=this->start_alpha;
		this->alpha_mod=1.0f/startTimeOut;
		this->connectAlpha=(connectAlpha<0.0f);				
	}

	bool CLUAEmitter::doAlphaConnection()
	{
		return connectAlpha;
	}

	bool CLUAEmitter::doTimeOut()
	{
		return timeOut;
	}

	GLvoid CLUAEmitter::setCameraShake(GLfloat strength)
	{
		this->shakeCamera=true;
		this->shakeStrength=strength;
	}

	CLUAParticle *CLUAEmitter::getParticle(GLint index)
	{
		return particles[index];
	}

	GLvoid CLUAEmitter::setColor(cml::vector3f color)
	{
		for (GLint i=0; i<pCount; i++)
		{
			particles[i]->set_color(color);
		}
	}
}