extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "ScriptVariables.h"

CScriptVariables script_variables;

/* functions for handling DK scrips */

int set_generate_speed(lua_State *L)
{
	script_variables.set_generate_speed((GLint)lua_tonumber(L,1));
	return 0;
}

int start_money(lua_State *L)
{
	script_variables.set_start_money((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2));
	return 0;
}

int max_creatures(lua_State *L)
{
	script_variables.set_max_creature_count((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2));
	return 0;
}

int get_value(lua_State *L)
{
	lua_pushnumber(L,script_variables.get_value((GLint)lua_tonumber(L,1),(GLint)lua_tonumber(L,2)));
	return 1;
}

/* -------------------------------- */

lua_State* LUA;

void register_functions_to_LUA()
{
	lua_register(LUA,"set_generate_speed",set_generate_speed);
	lua_register(LUA,"start_money",start_money);
	lua_register(LUA,"max_creatures",max_creatures);
	lua_register(LUA,"get_value",get_value);
}

void init_LUA()
{
	LUA = lua_open();
	luaL_openlibs(LUA);

	register_functions_to_LUA();
}

void execute_LUA_script(char *script)
{
	luaL_dofile(LUA, script);
}

void clean_LUA()
{
	lua_close(LUA);
}