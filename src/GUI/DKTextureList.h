#ifndef DKTEXTURE_LIST_H
#define DKTEXTURE_LIST_H

#include <map>

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
		char *file_name;
		bool trans, auto_transparent;
		GLubyte R,G,B;
		GLint texture_filter;

		GLvoid set(const char *file, bool trans, GLint texture_filter, bool auto_transparent, GLubyte R, GLubyte G, GLubyte B)
		{
			this->file_name=strdup(file);
			if (this->file_name==NULL)
				fprintf(stderr,"Can't allocate memory for '%s' string\n",file);
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

	struct cmp_str 
	{
		bool operator()(const char *a, const char *b) const
		{
			return strcmp(a, b) < 0;
		}
	};
	
	std::map<const char*,TEX_DATA*,cmp_str> textures;

	bool error;
#ifdef WIN32
	HWND hwnd;
#endif
};

#endif // DKTEXTURE_LIST_H
