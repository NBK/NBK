// luaGen.cpp : Defines the entry point for the console application.
//

#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

template <
typename T1, 
typename T2
>
class  Duo 
{
};

// type that represents unused type parameters
class  NullT 
{
};

class LuaCallBase
{

public:
	LuaCallBase(lua_State *luaState, const std::string& functionName)
	{
		L = luaState;
		lua_getglobal(L, functionName.c_str());
	}

protected:

	void push(const int &value)
	{
		lua_pushinteger(L, value);
	}

	void push(const float &value)
	{
		lua_pushnumber(L, value);
	}

	void push(const double &value)
	{
		lua_pushnumber(L, value);
	}

	void push(const std::string &value)
	{
		lua_pushstring(L, value.c_str());
	}

	void get(int &value) const
	{
		value = lua_tointeger(L, -1);
	}

	void get(float &value) const
	{
		value = (float)lua_tonumber(L, -1);
	}

	void get(double &value) const
	{
		value = lua_tonumber(L, -1);
	}

	void get(std::string &value) const 
	{
		 value = (char*)lua_tostring(L, -1);
	}

	void get(NullT &value) const 
	{
	}

protected:

	lua_State *L;
};

template <
typename TR,
typename T1 = NullT,
typename T2 = NullT,
typename T3 = NullT,
typename T4 = NullT
>
class  LuaCall
	: public Duo<TR, typename LuaCall<T1,T2,T3,T4,NullT> >
	, public LuaCallBase
{
public:

	LuaCall(lua_State *L, const std::string& functionName)
		: LuaCallBase(L, functionName)
	{

	}

	TR call(T1 a1, T2 a2, T3 a3, T4 a4)
	{
		TR returnValue;

		push(a1);
		push(a2);
		push(a3);
		push(a4);

		if(lua_pcall(L, 4, 1, 0) != 0)
		{
			throw std::string(std::string(lua_tostring(L, -1)));
		}

		get(returnValue);

		return returnValue;
	}

};


template <
typename TR, 
typename T1, 
typename T2, 
typename T3
>
class  LuaCall<TR,T1,T2,T3,NullT> 
	: public Duo<TR,T1> 
	, public LuaCallBase
{
public:
	LuaCall(lua_State *L, const std::string& functionName)
		: LuaCallBase(L, functionName)
	{

	}

	TR call(T1 a1, T2 a2, T3 a3)
	{
		TR returnValue;

		push(a1);
		push(a2);
		push(a3);

		if(lua_pcall(L, 3, 1, 0) != 0)
		{
			throw std::string(std::string(lua_tostring(L, -1)));
		}

		get(returnValue);

		return returnValue;
	}
};

// specialization to end deriving recursion
template <
typename TR, 
typename T1, 
typename T2
>
class  LuaCall<TR,T1,T2,NullT,NullT> 
	: public Duo<TR,T1> 
	, public LuaCallBase
{
public:
	LuaCall(lua_State *L, const std::string& functionName)
		: LuaCallBase(L, functionName)
	{

	}

	TR call(T1 a1, T2 a2)
	{
		TR returnValue;

		push(a1);
		push(a2);

		if(lua_pcall(L, 2, 1, 0) != 0)
		{
			throw std::string(std::string(lua_tostring(L, -1)));
		}

		get(returnValue);

		return returnValue;
	}
};

// specialization to end deriving recursion
template <
typename TR, 
typename T1
>
class  LuaCall<TR,T1,NullT,NullT,NullT> 
	: public Duo<TR,T1> 
	, public LuaCallBase
{
public:
	LuaCall(lua_State *L, const std::string& functionName)
		: LuaCallBase(L, functionName)
	{

	}

	TR call(T1 a1)
	{
		TR returnValue;

		push(a1);

		if(lua_pcall(L, 1, 1, 0) != 0)
		{
			throw std::string(std::string(lua_tostring(L, -1)));
		}

		get(returnValue);

		return returnValue;
	}
};

template <typename TR>
class  LuaCall<TR,NullT,NullT,NullT,NullT> 
	: public Duo<TR,NullT> 
	, public LuaCallBase
{
public:
	LuaCall(lua_State *L, const std::string& functionName)
		: LuaCallBase(L, functionName)
	{
	}

	TR call(void)
	{
		TR returnValue;

		if(lua_pcall(L, 0, 1, 0) != 0)
		{
			throw std::string(std::string(lua_tostring(L, -1)));
		}

		get(returnValue);

		return returnValue;
	}
};

template <>
class  LuaCall<NullT,NullT,NullT,NullT,NullT> 
	: public LuaCallBase
{
public:
	LuaCall(lua_State *L, const std::string& functionName)
		: LuaCallBase(L, functionName)
	{
	}

	void call(void)
	{
		if(lua_pcall(L, 0, 0, 0) != 0)
		{
			throw std::string(std::string(lua_tostring(L, -1)));
		}
	}
};


#include <iostream>
/*
int main()
{

	try
	{	
		lua_State *L;

		L = luaL_newstate();

		if(luaL_loadfile(L, "test.lua") || lua_pcall(L, 0, 0, 0))
		{
			throw std::string(std::string(lua_tostring(L, -1)));
		}
		
		std::cout << LuaCall<float, int, float, int, int>(L, "sum4").call(5, 1.5, 5, 5) << std::endl;
		std::cout << LuaCall<std::string, std::string, std::string>(L, "catenate").call("Renato", "Bianca") << std::endl;
		LuaCall<NullT, int>(L, "incVar").call(10);
		std::cout << LuaCall<int>(L, "getVar").call() << std::endl;

		
		if(L != NULL)
		{
			lua_close(L);
		}
	}
	catch (const std::string &e)
	{
		std::cout << e << std::endl;
	}


	return 0;
}
*/

