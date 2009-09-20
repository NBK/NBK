#include <stdio.h>
#include "ScriptControler.h"
#include <iostream>

using namespace std;

int main ( int argc, char *argv[] )
{
	init_LUA();

	//execute_LUA_script("test.lua");
	execute_LUA_script("luac.out");
	//execute_LUA_script("MAP00001.LUA");	

	/*long start = GetTickCount();
	for (int i=0; i<10000; i++)
	{
		//execute_LUA_script("MAP00001.LUA");	
		execute_LUA_script("luac.out");		// 3.6 faster than non compiled
	}

	long dur = GetTickCount() - start;

	cout << "Test: " << dur << " ms for 10000" << endl;
	cout << "Test: " << (float)dur/10000.0f << " ms for 1" << endl;
	cout << "Test: " << (float)dur/600000.0f << " s for 1" << endl;
	cout << "Test: " << 1.0f/((float)dur/600000.0f) << " Hz for 1" << endl;*/	

	clean_LUA();

	printf("generate speed = %d\n",script_variables.get_generate_speed());

	for (int i=0; i<5; i++)
	{
		printf("start money (player%d)= %d\n",i,script_variables.get_start_money(i));
	}

	for (int i=0; i<5; i++)
	{
		printf("max creatures count(player%d)= %d\n",i,script_variables.get_max_creature_count(i));
	}
	return 0;
}