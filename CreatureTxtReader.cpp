#include "commons.h"
#include "CreatureTxtReader.h"
#include <string>
#include <algorithm>

using namespace std;

namespace game_utils
{
	CCreatureTxtReader::CCreatureTxtReader()
	{
		FILE *file=NULL;

		if (!(file=fopen((CV_RESOURCES_DIRECTORY + "TextFiles/creature.txt").c_str(),"rc")))
		{
			return;
		}

		buff = new char[45000];
		char *_buff=buff;
		size_t read=fread(buff,1,45000,file);
		fclose(file);

		skip(3);
		fill_data(creature_information);

		skip(4);
		fill_data(room_data);

		skip(3);
		fill_data(magic_data);

		skip(4);
		fill_data(general_values);

		skip(3);
		fill_data(job_names);

		skip(3);
		fill_data(anger_job_names);

		skip(5);
		fill_data(spell_data);

		skip(3);
		fill_data(shot_data);

		skip(4);
		fill_data(trap_data);

		buff=_buff;
		delete [] buff;
	}

	CCreatureTxtReader::~CCreatureTxtReader()
	{
		creature_information.clear();
		room_data.clear();
		magic_data.clear();
		general_values.clear();
		job_names.clear();
		anger_job_names.clear();
		spell_data.clear();
		shot_data.clear();
		trap_data.clear();
	}

	GLvoid CCreatureTxtReader::split_line(string &str, char delim, vector<string> &results)
	{
		string tmp="";

		for (size_t i=0; i<str.length(); i++)
		{
			if (str[i]!=delim)
			{
				tmp+=str[i];
			}
			else
			{
				if (tmp.length()>0)
				{
					results.push_back(tmp);
					tmp="";
				}
			}
		}
	}

	GLvoid CCreatureTxtReader::fill_data(std::map< std::string, std::vector<GLint> > &target)
	{
		while (true)
		{
			string name;
			while (*buff!='\t' && *buff!='\n')
			{
				name+=*buff++;	
			}

			if (name.length()==0)
			{
				break;
			}

			buff++;

			string atributes="";
			while (*buff!='\n')
			{
				atributes+=*buff++;
			}
			buff++;

			vector<string> atributes_vector;

			split_line(atributes,'\t',atributes_vector);

			for (vector<string>::iterator iter=atributes_vector.begin(); iter!=atributes_vector.end(); iter++)
			{
				const char *str=iter->c_str();
				int a=atoi(str);
				target[name].push_back(a);
			}
		}
	}

	GLvoid CCreatureTxtReader::skip(GLint n)
	{
		for (GLint i=0; i<n; i++)
		{
			buff+=(strstr(buff,"\n")-buff)+1;
		}
	}

	GLint CCreatureTxtReader::get_room_propery(char *room_class_name, GLint property)
	{
		if (room_data[string(room_class_name)].size()>0)
		{
			return room_data[string(room_class_name)][property];
		}

		return -1;
	}

	GLint CCreatureTxtReader::get_creature_propery(char *creature_class_name, GLint property)
	{
		if (creature_information[string(creature_class_name)].size()>0)
		{
			return creature_information[string(creature_class_name)][property];
		}

		return -1;
	}
}