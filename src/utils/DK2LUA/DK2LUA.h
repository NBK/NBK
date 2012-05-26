#ifndef DK2LUA_H
#define DK2LUA_H

#include <windows.h>
#include <gl/gl.h>
#include <string>

using namespace std;

#define MAX_SCRIPT_SIZE	1024*50

class CDK2LUA
{
public:
	CDK2LUA();
	~CDK2LUA();

	bool load_script_file(char *filename);

	GLvoid convert_to_LUA();

	GLvoid save_to_file(char *filename);

	const char *get_script();

private:
	string script;
	size_t char_read;
};

#endif  //DK2LUA_H