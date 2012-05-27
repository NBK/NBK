#ifndef LUA_EFFECT_H
#define LUA_EFFECT_H

#include "../system.h"
#include <GL/gl.h>
#include <cml/cml.h>
#include <vector>
#include "LUAEmitter.h"

namespace LUA_effects
{
	class CLUAEffect
	{
	public:
		CLUAEffect();
		CLUAEffect(const CLUAEffect &luaEffect);
		~CLUAEffect();

		virtual GLvoid init();
		virtual bool draw(bool update=true);
		virtual	GLvoid reset();	

		GLvoid	addEmitter();

		GLvoid	set_force_nonactive(bool force_nonactive);

		bool	all_particles_dead();
		GLint	getEmittersCount();
		std::vector<CLUAEmitter*> *getEmitterList();
		CLUAEmitter *getEmittter(GLint index);

		//GLvoid drawEmitters();

	protected:

		std::vector<CLUAEmitter*>			emitters;
		std::vector<CLUAEmitter*>::iterator emitters_iter;

		bool force_nonactive;

		GLuint dead_count;
	};
};

#endif // EFFECT_H
