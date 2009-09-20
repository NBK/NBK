#ifndef SUBTILE_DATA_H
#define SUBTILE_DATA_H

#include <cml/cml.h>
#include <string>

/*
	Texture atlas info (including animations) gets stored in this
	structures. On render time texture coordinates get read from them.
	Texture coordinates must be updated only if we have an animated tile.
*/
namespace game_utils
{
	class CSubtileData
	{
	public:
		
		CSubtileData();
		~CSubtileData();

		GLvoid create(GLint textureAtlasPos, GLint repeat=1);
		GLvoid create(GLint textureAtlasPos[], GLint count, bool randomStart=false, bool reset=false);
		
		GLvoid updateAnimation(GLfloat delta);
		GLvoid resetAnimation();
		GLvoid setAnimSpeed(GLfloat animSpeed);

		cml::vector2f getTextureAtlasPosition();
		cml::vector2f getTextureAtlasSize();
		GLfloat getTextureAtlasSubtileSize();

		cml::vector3f points[4];

		GLfloat getPixelWidth();
		GLfloat getPixelHeight();

		GLint getAnimCount();

		GLvoid setAnimPos(GLint animPos);

		GLvoid init();

	private:

		GLint	texAtlW, 
				texAtlH, 
				texAtlSubSize,
				texturesPerRow;

		GLfloat pixelWidth,
				pixelHeight;

		struct sAnimData
		{
			GLfloat textureAtlasX,
					textureAtlasY;
		};

		sAnimData animData[8]; // each tile may be animated with max 8 animation frames

		GLint	animPos,
				animCount,
				subtile;

		GLfloat animSpeed,
				animChange,
				dtx,
				dty;

		bool	up,
				reset;

		bool inited;

		static bool settingsRead;

		static GLfloat	textureAtlasWidth,
						textureAtlasHeight,
						textureAtlasSubtileSize,
						textureAtlasPixelWidth,
						textureAtlasPixelHeight;
		
		GLvoid init_(GLfloat texAtlW, GLfloat texAtlH, GLfloat texAtlSubSize, GLfloat pixelWidth, GLfloat pixelHeight);
		
	};
};

#endif // SUBTILE_DATA_H