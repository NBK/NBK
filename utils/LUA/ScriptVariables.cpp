#include "ScriptVariables.h"

CScriptVariables::CScriptVariables()
{
	generate_speed=0;

	for (GLint i=0; i<MAX_PLAYERS; i++)
	{
		start_money[i]=0;
	}

	initilized.init(MAX_INITIALIZED_VARIABLES);
}

CScriptVariables::~CScriptVariables()
{
}

GLvoid CScriptVariables::set_generate_speed(GLint generate_speed)
{
	if (!initilized.is_init(IV_GENERATE_SPEED))
	{
		this->generate_speed=generate_speed;
		initilized.set_init(IV_GENERATE_SPEED);
	}
}

GLint CScriptVariables::get_generate_speed()
{
	return generate_speed;
}

GLvoid CScriptVariables::set_start_money(GLint player, GLint amount)
{
	if (!initilized.is_init(IV_START_MONEY_PLAYER_0+player))
	{
		start_money[player]=amount;
		initilized.set_init(IV_START_MONEY_PLAYER_0+player);
	}
}

GLint CScriptVariables::get_start_money(GLint player)
{
	return start_money[player];
}

GLvoid CScriptVariables::set_max_creature_count(GLint player, GLint amount)
{
	if (!initilized.is_init(IV_MAX_CREATURE_COUNT_PLAYER_0+player))
	{
		max_creature_count[player]=amount;
		initilized.set_init(IV_MAX_CREATURE_COUNT_PLAYER_0+player);
	}
}

GLint CScriptVariables::get_max_creature_count(GLint player)
{
	return max_creature_count[player];
}

GLint CScriptVariables::get_value(GLint player, GLint VAR)
{
	return 50;
}