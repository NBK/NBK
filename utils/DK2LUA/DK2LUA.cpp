#include <stdio.h>

#include "DK2LUA.h"

CDK2LUA::CDK2LUA()
{
}

CDK2LUA::~CDK2LUA()
{
}

bool CDK2LUA::load_script_file(char *filename)
{
	FILE *f = NULL;

	if (!(f=fopen(filename,"r"))) 
	{
		return false;
	}

	char *tmp = new char[MAX_SCRIPT_SIZE];
	char_read=fread(tmp,1,MAX_SCRIPT_SIZE,f);
	tmp[char_read]='\0';
	script.assign(tmp);
	delete [] tmp;

	fclose(f);

	return true;
}

GLvoid CDK2LUA::convert_to_LUA()
{
	/* first convert all to lower case */
	bool in=false;

	for (string::iterator i=script.begin(); i!=script.end(); i++)
	{
		if (*i=='"')
		{
			in=!in;
		}
		(*i)=((*i>='A' && *i<='Z' && !in)?tolower(*i):*i);
	}

	/* now replace some things */
	GLint pos=0;
		
	for (GLint i=10; i<14; i++)
	{
		pos=0;
		while ((pos=script.find("if ",pos))!=string::npos)
		{
			int p=pos+2;

			while (script[p] == ' ')
			{
				script.replace(p,1,"");
			}
		}
	}


	char *replace[][2] =
	{
		{"rem","--"},	
		{"endif","end"},
		{">=","~¡"},
		{"<=","^¢"},
		{">","°²"},
		{"==","`ÿ"},
		{"~¡",") >="},
		{"^¢",") <="},
		{"°²",") >"},
		{"`ÿ",") =="},
		{"if(","if (( get_value("},
		{"if_action_point","if ( action_point"},
		{"if_available","if ( available"},
		{"win_game","°¡"},
		{"lose_game","^¢"},
		{"°¡","win_game()"},
		{"^¢","lose_game()"},
		{"next_command_reusable","next_command_reusable()"}
	};


	pos=0;
		
	for (GLint i=10; i<13; i++)
	{
		pos=0;
		while ((pos=script.find(replace[i][0],pos))!=string::npos)
		{
			while (script.at(pos)!='\n')
			{
				pos++;
			}
			script.insert(pos," ) then");
			//script.insert(pos," then");
		}
	}

	for (GLint i=0; i<18; i++)
	{
		pos=0;
		while ((pos=script.find(replace[i][0],pos))!=string::npos)
		{			
			script.replace(script.find(replace[i][0],pos),strlen(replace[i][0]),replace[i][1]);
			pos++;
		}
	}
}

const char *CDK2LUA::get_script()
{
	return script.c_str();
}

GLvoid CDK2LUA::save_to_file(char *filename)
{
	FILE *f = NULL;

	if (!(f=fopen(filename,"wc")))
	{
		return;
	}

	fwrite(script.c_str(),1,script.length(),f);
	fclose(f);
}