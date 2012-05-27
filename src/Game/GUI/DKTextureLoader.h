#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

class CDKTextureLoader  
{
public:
	CDKTextureLoader();
	~CDKTextureLoader();

	struct LOADER_RESULT
	{
		POINT texture_dimensions;
		bool result;

		LOADER_RESULT()
		{
			result=false;
		}
	};

	LOADER_RESULT build_texture(char *file, GLuint &texture, bool trans=false, GLint texture_filter=GL_LINEAR, bool auto_transparent=true, GLubyte R=0, GLubyte G=0, GLubyte B=0);
};

#endif // TEXTURE_LOADER_H
