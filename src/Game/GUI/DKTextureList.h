#ifndef DKTEXTURE_LIST_H
#define DKTEXTURE_LIST_H

#include <map>
#include <string>

class CDKTextureList
{
public:
#ifdef WIN32
	CDKTextureList(HWND hwnd);
#else
	CDKTextureList();
#endif
	~CDKTextureList();

	GLvoid add_texture(const char *name, const char *file, bool trans=false, GLint texture_filter=GL_LINEAR, bool auto_transparent=true, GLubyte R=0, GLubyte G=0, GLubyte B=0);
	GLvoid build_textures();

	struct TEX_DATA
	{		
		GLuint texture,width,height;
		std::string file_name;
		bool trans, auto_transparent;
		GLubyte R,G,B;
		GLint texture_filter;

		GLvoid set(const char *file, bool trans, GLint texture_filter, bool auto_transparent, GLubyte R, GLubyte G, GLubyte B)
		{
			this->file_name=file;
			this->texture_filter=texture_filter;
			this->auto_transparent=auto_transparent;
			this->R=R;
			this->G=G;
			this->B=B;
			this->trans=trans;
		}
	};

	GLuint get_texture_by_name(const char *name);
	TEX_DATA get_texture_data_by_name(const char *name);

private:
	
	typedef std::map<std::string,TEX_DATA*> TexMap;
	typedef TexMap::iterator				TexMapItr;

	TexMap textures;

	bool error;
#ifdef WIN32
	HWND hwnd;
#endif
};

#endif // DKTEXTURE_LIST_H
