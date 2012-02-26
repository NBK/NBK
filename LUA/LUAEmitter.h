#ifndef LUA_EMITTER_H
#define LUA_EMITTER_H

#include "../system.h"
#include <GL/gl.h>
#include "LUAParticle.h"
#include <string>
#include <vector>

namespace LUA_effects
{
	// Emmiter is a particle containing particles. It doesn't get drawn;
	class CLUAEmitter: public CLUAParticle
	{
	public:
		CLUAEmitter();
		~CLUAEmitter();

		virtual GLvoid init();
		virtual GLvoid draw();
		virtual GLvoid update();
		virtual GLvoid reset();
		virtual bool is_dead();

		GLvoid	setParticleCount(GLint count);
		GLint	getParticleCount();
		GLvoid	setParticleInitLUA(std::string particleInitLUAFile);

		std::vector<int> *getGlobalLUAParams();

		std::string getInitLUAFile();
		std::string getUpdateLUAFile();	

		/* used in editor */
		GLvoid setInitLUAFile(std::string initLUAFile);
		GLvoid setUpdateLUAFile(std::string updateLUAFile);

		/* called from game or app */
		GLvoid setInitLUAFile(std::string effectDir, std::string initLUAFile);
		GLvoid setUpdateLUAFile(std::string effectDir, std::string updateLUAFile);

		GLvoid setParticleTexture(std::string particleTexture);
		GLvoid setParticleAnimatedTexture(std::string particleTexture, GLint count_x, GLint count_y, GLfloat speed);

		bool	doFollowPath();
		GLvoid	setFollowPath(bool followPath);

		GLvoid setPathFile(std::string pathFile);

		GLvoid setPathMoveSpeed(GLfloat pathMoveSpeed);

		struct GravityPoint
		{
			cml::vector3f position;
			GLfloat mass;

			GravityPoint(cml::vector3f position, GLfloat mass)
			{
				this->position=position;
				this->mass=mass;
			}
		};

		struct Deflector: public GravityPoint
		{
			Deflector(cml::vector3f position, GLfloat mass): GravityPoint(position,-mass)
			{}
		};

		GLvoid	addGravityPoint(GravityPoint *gp);
		GLvoid	addDeflector(Deflector *dp);

		GLvoid	loadParticlePositions(std::string fileName);

		GLvoid	setTransparent(bool transparent);
		GLvoid	setDepthTest(bool depthTest);
		GLvoid	setScale(GLfloat scale);
		GLvoid	setTimeOut(GLfloat timeOut, GLfloat connectAlpha);
		GLvoid	setCameraShake(GLfloat strength);

		// if we have a effect time out then we can connect effect duration with global effect alpha
		bool doAlphaConnection();
		bool doTimeOut();

		CLUAParticle *getParticle(GLint index);

		// updates all particles with this color
		GLvoid setColor(cml::vector3f color);

	private:

		CLUAParticle	**particles;
		GLint			pCount;
		GLfloat			scale,
						currentTimeOut,
						startTimeOut,
						shakeStrength;

		GLvoid doLUAUpdate();

		std::string particleInitLUAFile,					
					updateLUAFile;

		GLuint particleTexture;

		lua_State	*LUA_update;

		GLvoid __popLuaGlobals();
		GLvoid __updateLUAEmitter();
		GLvoid __pushLuaGlobals();

		//LuaCall<int, int>(LUA_update,"onUpdate").call(1000); TMP

		std::vector<int> globalLUAParams;

		// becomes true if EMITTER_SET_FOLLOW_PATH is set. then path is loaded from .pth file
		bool	followPath,
				transparent,
				depthTest,
				timeOut,
				connectAlpha,
				shakeCamera;

		std::vector<cml::vector3f> pathPoins;
		GLint pathPos;
		GLfloat pathMoveSpeed;

		cml::vector3f oldPos;

		std::vector<GravityPoint*> gravityPoints;
		std::vector<GravityPoint*>::iterator gravityPointsIter;

		std::vector<Deflector*> deflectorPoints;
		std::vector<Deflector*>::iterator deflectorPointsIter;

		std::vector<cml::vector3f> particleStartPositions;

		GLvoid setParticlePositions();
	};
};

#endif // LUA_EMITTER_H