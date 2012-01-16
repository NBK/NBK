#ifndef LUA_EFFECT_LOADER_H
#define LUA_EFFECT_LOADER_H

#include "../system.h"
#include <GL/gl.h>
#include <string>
#include "LUAEffect.h"

namespace LUA_effects
{
	class CLUAEffectLoader
	{
	public:
		CLUAEffectLoader();
		~CLUAEffectLoader();

		static CLUAEffect *loadFromFile(std::string fileName);

		// saving/loading effects
		struct EMITTER_DATA
		{
			char	initFile[MAX_PATH],
					updateFile[MAX_PATH],
					effectDirectory[MAX_PATH];
		};
	};
}

#endif // LUA_EFFECT_LOADER_H