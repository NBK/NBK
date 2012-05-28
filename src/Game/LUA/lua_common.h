#ifndef COMMON_H
#define COMMON_H

#include "LUAEffect.h"

extern std::string	G_startDir;

// hack... bla bla
#define lua_open()  luaL_newstate()

#ifdef WIN32
#define __luaL_dofile(L,fn) if (luaL_dofile(L, fn)!=0) \
							{ MessageBox(NULL,lua_tostring(LUA_init,-1),"LUA error!",MB_OK | MB_ICONERROR); }
#else
#define __luaL_dofile(L,fn) if (luaL_dofile(L, fn)!=0) \
							{ printf("[ERROR|LUA] %s\n", lua_tostring(LUA_init,-1)); }
#endif

#endif // COMMON_H