#include <windows.h>
#include <gl\gl.h>
#include <stdio.h>

#include "DKTextureLoader.h"
#include "DKTextureList.h"

using namespace std;

CDKTextureList::CDKTextureList(HWND hwnd)
{
	error=false;
	this->hwnd=hwnd;
}

CDKTextureList::~CDKTextureList()
{
	for (map<char*,TEX_DATA*,cmp_str>::iterator iter = textures.begin(); iter != textures.end(); iter++)
	{
		TEX_DATA *tmp = (*iter).second;
		delete tmp;
	}
}

GLvoid CDKTextureList::add_texture(char *name, char *file, bool trans, GLint texture_filter, bool auto_transparent, GLubyte R, GLubyte G, GLubyte B)
{
	TEX_DATA *new_texture_info = new TEX_DATA();

	if (!file)
	{
		new_texture_info->file_name[0]='\0';
		textures[name]=new_texture_info;
	}
	else
	{
		new_texture_info->set(file,trans,texture_filter,auto_transparent,R,G,B);
		textures[name]=new_texture_info;
	}
}

GLvoid CDKTextureList::build_textures()
{
	CDKTextureLoader tl;

	for (map<char*,TEX_DATA*,cmp_str>::iterator iter = textures.begin(); iter != textures.end(); iter++)
	{
		TEX_DATA *tex_data = (*iter).second;

		if (tex_data->file_name[0]=='\0')
		{
			continue;
		}

		CDKTextureLoader::LOADER_RESULT lr = tl.build_texture(tex_data->file_name,tex_data->texture,tex_data->trans,tex_data->texture_filter,tex_data->auto_transparent,tex_data->R,tex_data->G,tex_data->B);

		if (!lr.result)
		{
			char err[512];
			sprintf(err,"Texture error: %s",tex_data->file_name);
			MessageBox( hwnd, err, "Error", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		}
		else
		{
			tex_data->width=lr.texture_dimensions.x;
			tex_data->height=lr.texture_dimensions.y;
		}
	}
}

GLuint CDKTextureList::get_texture_by_name(char *name)
{
	if (!name)
	{
		MessageBox(hwnd,"The texture by this name does not exist!","ERROR",MB_OK|MB_ICONERROR | MB_SETFOREGROUND);
		return 0;
	}

	if (!textures[name])
	{
		MessageBox(hwnd,"The textureby this name does not exist!","ERROR",MB_OK|MB_ICONERROR | MB_SETFOREGROUND);
		return 0;
	}
	else
	{
		return (*textures[name]).texture;
	}
}

CDKTextureList::TEX_DATA CDKTextureList::get_texture_data_by_name(char *name)
{
	if (!name)
	{
		MessageBox(hwnd,"The textureby this name does not exist!","ERROR",MB_OK|MB_ICONERROR | MB_SETFOREGROUND);
		return TEX_DATA();
	}

	if (!textures[name])
	{
		MessageBox(hwnd,"The textureby this name does not exist!","ERROR",MB_OK|MB_ICONERROR | MB_SETFOREGROUND);
		return TEX_DATA();
	}	
	else
	{
		return *textures[name];
	}
}