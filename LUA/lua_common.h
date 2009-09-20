#ifndef COMMON_H
#define COMMON_H

#include "LUAEffect.h"

extern std::string	G_startDir;

#define __luaL_dofile(L,fn) if (luaL_dofile(L, fn)!=0) \
							{ MessageBox(NULL,lua_tostring(LUA_init,-1),"LUA error!",MB_OK | MB_ICONERROR); }

#endif // COMMON_H