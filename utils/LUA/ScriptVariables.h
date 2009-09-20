#ifndef SCRIPT_VARIABLES_H
#define SCRIPT_VARIABLES_H

#include <windows.h>
#include <gl/gl.h>

#define GET(name,var,type) type name(){return var;}
#define SET(name,var,type) GLvoid name(type var) {this->var=var;}

#define MAX_PLAYERS 5
#define MAX_INITIALIZED_VARIABLES 64

#define MAX_MESSAGES_PER_LEVEL 100

class InitilizedVars
{
public:

	InitilizedVars()
	{
	}

	GLvoid init(int count)
	{
		initialized = new bool[count];
		for (GLint i=0; i<count; i++)
		{
			initialized[i]=false;
		}
	}

	~InitilizedVars()
	{
		delete [] initialized;
	}

	bool is_init(GLint VAR)
	{
		return initialized[VAR];
	}

	GLvoid set_init(GLint VAR)
	{
		initialized[VAR]=true;
	}
		
private:
	bool *initialized;
};

class CScriptVariables
{
public:
	CScriptVariables();
	~CScriptVariables();

	/*SET(set_generate_speed,generate_speed,int);
	GET(get_generate_speed,generate_speed,int);*/

	GLvoid set_generate_speed(GLint generate_speed);
	GLint get_generate_speed();

	GLvoid set_start_money(GLint player, GLint amount);
	GLint get_start_money(GLint player);

	GLvoid set_max_creature_count(GLint player, GLint amount);
	GLint get_max_creature_count(GLint player);

	GLint get_value(GLint player, GLint VAR);

private:

	/* this are variables that get initialzed at begining */

	/* this keeps the track of what has already been initialized */
		

	enum INITIALIZED_VARIABLES
	{
		IV_GENERATE_SPEED=0, 
		
		IV_START_MONEY_PLAYER_0, IV_START_MONEY_PLAYER_1, 
		IV_START_MONEY_PLAYER_2, IV_START_MONEY_PLAYER_3, 
		IV_START_MONEY_PLAYER_4, IV_START_MONEY_PLAYER_5,

		IV_MAX_CREATURE_COUNT_PLAYER_0, IV_MAX_CREATURE_COUNT_PLAYER_1, 
		IV_MAX_CREATURE_COUNT_PLAYER_2, IV_MAX_CREATURE_COUNT_PLAYER_3, 
		IV_MAX_CREATURE_COUNT_PLAYER_4, IV_MAX_CREATURE_COUNT_PLAYER_5
	};

	/* some things get initialized only once */
	InitilizedVars initilized;

	/* the game generate speed */
	GLint generate_speed;

	/* starting money for all players */
	GLint start_money[MAX_PLAYERS];

	/* starting creature number for all players */
	GLint max_creature_count[MAX_PLAYERS];

	/* if the message has already been displayed we don't display it again */
	InitilizedVars messages;

};

#endif // SCRIPT_VARIABLES_H
