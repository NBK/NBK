#include "../system.h"
#include <GL/gl.h>
#include "../TextureLoader.h"
#include "LUAEffect.h"
#include "../utils.h"
#include "lua_common.h"

using namespace cml;

namespace LUA_effects
{
	CLUAEffect::CLUAEffect()
	{
		force_nonactive=false;
	}

	CLUAEffect::CLUAEffect(const CLUAEffect &luaEffect)
	{
		for (GLuint i=0; i<luaEffect.emitters.size(); i++)
		{
			addEmitter();

			string init = luaEffect.emitters[i]->getInitLUAFile();
			string update = luaEffect.emitters[i]->getUpdateLUAFile();
			string effDir = luaEffect.emitters[i]->getEffectDirectory();

			GLint mIndex = max((GLint)init.find_last_of('/'),(GLint)init.find_last_of('\\'))+1;
			getEmittter(i)->setInitLUAFile(effDir,init.substr(mIndex));

			mIndex = max((GLint)update.find_last_of('/'),(GLint)update.find_last_of('\\'))+1;
			getEmittter(i)->setUpdateLUAFile(effDir,update.substr(mIndex));
		}
		init();
	}

	CLUAEffect::~CLUAEffect()
	{
		for (emitters_iter=emitters.begin(); emitters_iter!=emitters.end(); emitters_iter++)
		{
			SAFE_DELETE(*emitters_iter);
		}
	}

	GLvoid CLUAEffect::init()
	{
		for (emitters_iter=emitters.begin(); emitters_iter!=emitters.end(); emitters_iter++)
		{
			(*emitters_iter)->init();
		}
	}

	GLvoid CLUAEffect::addEmitter()
	{
		emitters.push_back(new CLUAEmitter());
	}

	GLvoid CLUAEffect::reset()
	{
		force_nonactive=false;
		for (emitters_iter=emitters.begin(); emitters_iter!=emitters.end(); emitters_iter++)
		{
			(*emitters_iter)->reset();
		}
	}

	bool CLUAEffect::draw(bool update)
	{
		dead_count=0;

		for (emitters_iter=emitters.begin(); emitters_iter!=emitters.end(); emitters_iter++)
		{
			CLUAEmitter *emitter = *emitters_iter;

			if (emitter->is_dead())
			{
				dead_count++;
				continue;
			}

			if (update)
			{
				emitter->update();
			}
			emitter->draw();
		}

		glColor3f(1.0f,1.0f,1.0f);

		return (dead_count!=emitters.size());
	}

	bool CLUAEffect::all_particles_dead()
	{
		return ((force_nonactive)||(dead_count==emitters.size()));
	}

	GLvoid CLUAEffect::set_force_nonactive(bool force_nonactive)
	{
		this->force_nonactive=force_nonactive;
	}

	GLint CLUAEffect::getEmittersCount()
	{
		return emitters.size();
	}

	std::vector<CLUAEmitter*> *CLUAEffect::getEmitterList()
	{
		return &emitters;
	}

	CLUAEmitter *CLUAEffect::getEmittter(GLint index)
	{
		if (index>=0 && index<(GLint)emitters.size())
		{
			return emitters[index];
		}
		else
		{
			return NULL;
		}
	}
};