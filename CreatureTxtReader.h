#ifndef CREATURE_TXT_READER_H
#define CREATURE_TXT_READER_H

#include <windows.h>
#include <gl/gl.h>
#include <map>
#include <vector>
#include <string>

namespace game_utils
{
	class CCreatureTxtReader
	{
	public:
		CCreatureTxtReader();
		~CCreatureTxtReader();

		GLint get_room_propery(char *room_class_name, GLint property);
		GLint get_creature_propery(char *creature_class_name, GLint property);

	private:
		std::map< std::string, std::vector<GLint> > creature_information;
		std::map< std::string, std::vector<GLint> > room_data;
		std::map< std::string, std::vector<GLint> > magic_data;
		std::map< std::string, std::vector<GLint> > general_values;
		std::map< std::string, std::vector<GLint> > job_names;
		std::map< std::string, std::vector<GLint> > anger_job_names;
		std::map< std::string, std::vector<GLint> > spell_data;
		std::map< std::string, std::vector<GLint> > shot_data;
		std::map< std::string, std::vector<GLint> > trap_data;
		std::map< std::string, std::vector<GLint> > door_data;
		std::map< std::string, std::vector<GLint> > health_data;
		std::map< std::string, std::vector<GLint> > block_data;
		std::map< std::string, std::vector<GLint> > instance_data;

		GLvoid split_line(std::string &str, char delim, std::vector<std::string> &results);
		GLvoid fill_data(std::map< std::string, std::vector<GLint> > &target);
		GLvoid skip(GLint n);

		char *buff;
	};
}

#endif // CREATURE_TXT_READER_H