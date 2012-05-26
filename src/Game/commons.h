#ifndef COMMONS_H
#define COMMONS_H

#include "GameManager.h"
#include "system.h"
#include <string>
#include "TextureList.h"
#include "DeltaTime.h"
#include "Logger.h"

namespace game_utils
{

	static const GLfloat CV_PLAYER_COLOR[5][3] =
	{
		{1.0f,0.0f,0.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,1.0f,0.0f},
		{1.0f,1.0f,0.0f},
		{1.0f,1.0f,1.0f}
	};

	// commons for window settings. CV stands for "common variable"
	extern GLint	CV_SETTINGS_WINDOW_WIDTH;
	extern GLint	CV_SETTINGS_WINDOW_HEIGHT;
	extern GLint	CV_SETTINGS_WINDOW_WIDTH_HALF;
	extern GLint	CV_SETTINGS_WINDOW_HEIGHT_HALF;
	static const GLfloat	CV_SETTINGS_FOV					= 45.0f;
	static const bool		CV_SETTINGS_WINDOW_FULLSCREEN	= false;
	static const GLint		CV_NORMAL_MAP_START_INDEX		= 5000;

	//static const GLint	CV_MAX_VBO_QUADS			= 112;
	static const GLint	CV_MAX_VBO_QUADS			= 512;
	static const GLint	CV_MAX_VERTEX_BUFFER		= CV_MAX_VBO_QUADS*4;
	static const GLint	CV_MAX_TEX_COORD_BUFFER		= CV_MAX_VBO_QUADS*4;

	// commons for different properties
	static GLfloat		CV_BLOCK_HEIGHT			= 0.3f;
	static GLfloat		CV_WIDTH_HEIGHT_FACTOR	= 0.7f;
	static GLfloat		CV_BLOCK_WIDTH			= CV_BLOCK_HEIGHT*CV_WIDTH_HEIGHT_FACTOR;
	static GLfloat		CV_BLOCK_DEPTH			= CV_BLOCK_HEIGHT*CV_WIDTH_HEIGHT_FACTOR;
	static const GLuint	CV_LEVEL_MAP_SIZE		= 85;	// every DK map measures 85x85 blocks
	static const GLint	CV_BLOCK_RESOLUTION_XZ	= 4;
	static const GLint	CV_BLOCK_RESOLUTION_Y	= 5;

	static const GLint	CV_DEFORMED_CUBE_WIDTH	= 256;
	static const GLint	CV_DEFORMED_CUBE_HEIGHT	= 9+7; // 7 more for the ceiling
	static const GLint	CV_DEFORMED_CUBE_DEPTH	= 256;

	#define CV_FBLR_FACE_VERT_FLOATS	(CV_BLOCK_RESOLUTION_XZ-1)*(CV_BLOCK_RESOLUTION_Y-1)*4*3
	#define CV_TBWLC_FACE_VERT_FLOATS	(CV_BLOCK_RESOLUTION_XZ-1)*(CV_BLOCK_RESOLUTION_XZ-1)*4*3

	#define CV_FBLR_FACE_TEX_FLOATS		(CV_BLOCK_RESOLUTION_XZ-1)*(CV_BLOCK_RESOLUTION_Y-1)*4*2
	#define CV_TBWLC_FACE_TEX_FLOATS	(CV_BLOCK_RESOLUTION_XZ-1)*(CV_BLOCK_RESOLUTION_XZ-1)*4*2

	#define CV_FBLR_W_L_FACE_VERT_FLOATS	(CV_BLOCK_RESOLUTION_XZ-1)*4*3
	#define CV_FBLR_W_L_FACE_TEX_FLOATS		(CV_BLOCK_RESOLUTION_XZ-1)*4*2

	// camera settings
	#define CV_CAMERA_INITIAL_HEIGHT		CV_BLOCK_HEIGHT*4.0f
	#define CV_CAMERA_MAX_HEIGHT			CV_BLOCK_HEIGHT*15.0f
	#define CV_CAMERA_MIN_HEIGHT			CV_BLOCK_HEIGHT*2.0f
	#define CV_CAMERA_MIN_FPS_Y_POS			(CV_BLOCK_HEIGHT/10.0f)*3.0f

	extern cml::vector3f					CV_CAMERA_OLD_POSITION;

	#define MAX_CEILING_HEIGHT	2

	// commonst for window access
#ifdef WIN32
	extern HWND CV_WINDOW_HANDLE;
	extern HDC	CV_DEVICE_CONTEXT;
#else
#define CV_WINDOW_HANDLE
#define CV_DEVICE_CONTEXT
#endif

	// a very important variable stating the current player
	extern GLint CV_CURRENT_PLAYER_ID;

	// common variables for location of config files
	static std::string	CV_CONFIG_BLOCKS		= "blocks.conf";
	static std::string	CV_CONFIG_LEVELS		= "levels.conf";
	static std::string	CV_CONFIG_TEXTURES		= "textures.conf";
	static std::string	CV_CONFIG_MODELS		= "models.conf";
	static std::string	CV_CONFIG_EFFECTS		= "effects.conf";
	static std::string	CV_CONFIG_ATLAS_COORDS	= "atlas_coordinates.conf";
	static std::string	CV_CONFIG_SETTINGS		= "settings.conf";
	static std::string	CV_RESOURCES_DIRECTORY	= "data" PATH_SEP "resources" PATH_SEP;

	// common game lists texture list, model list...
	extern lists::CTextureList		*CV_TEXTURE_LIST;

	// common acces to game manager and from it all other modules it controls
	extern managers::CGameManager	*CV_GAME_MANAGER;

	// blocks have owners and here are owners values
	#define CV_PLAYER_0			0
	#define CV_PLAYER_1			1
	#define CV_PLAYER_2			2
	#define CV_PLAYER_3			3
	#define CV_PLAYER_4			4
	#define CV_PLAYER_UNDEFINED	5
	#define CV_MAX_PLAYERS		6

	// block types
	#define  CV_BLOCK_TYPE_ROCK_ID 0
	#define  CV_BLOCK_TYPE_GOLD_ID 1
	#define  CV_BLOCK_TYPE_EARTH_ID 2
	#define  CV_BLOCK_TYPE_EARTH_WITH_TORCH_PLATE_ID 3
	#define  CV_BLOCK_TYPE_WALL_ID 46789
	#define  CV_BLOCK_TYPE_WALL_0_ID 4
	#define  CV_BLOCK_TYPE_WALL_1_ID 6
	#define  CV_BLOCK_TYPE_WALL_2_ID 7
	#define  CV_BLOCK_TYPE_WALL_3_ID 8
	#define  CV_BLOCK_TYPE_WALL_4_ID 9
	#define  CV_BLOCK_TYPE_WALL_WITH_TORCH_PLATE_ID 5
	#define  CV_BLOCK_TYPE_UNCLAIMED_LAND_ID 10
	#define  CV_BLOCK_TYPE_CLAIMED_LAND_ID 11
	#define  CV_BLOCK_TYPE_LAVA_ID 12
	#define  CV_BLOCK_TYPE_WATER_ID 13
	#define  CV_BLOCK_TYPE_PORTAL_ID 14
	#define  CV_BLOCK_TYPE_LIBRARY_ID 18
	#define  CV_BLOCK_TYPE_TREASURE_ROOM_ID 16
	#define  CV_BLOCK_TYPE_PRISON_ID 20
	#define  CV_BLOCK_TYPE_TORTURE_CHAMBER_ID 22
	#define  CV_BLOCK_TYPE_TRAINING_ROOM_ID 24
	#define  CV_BLOCK_TYPE_HEART_ID 26
	#define  CV_BLOCK_TYPE_WORKSHOP_ID 28
	#define  CV_BLOCK_TYPE_TEMPLE_ID 32
	#define  CV_BLOCK_TYPE_GRAVEYARD_ID 34
	#define  CV_BLOCK_TYPE_HATCHERY_ID 36
	#define  CV_BLOCK_TYPE_LAIR_ID 38
	#define  CV_BLOCK_TYPE_BARRACKS_ID 40

	#define  CV_BLOCK_TYPE_WOODEN_DOORS_ID 423
	#define  CV_BLOCK_TYPE_WOODEN_DOORS_0_ID 42
	#define  CV_BLOCK_TYPE_WOODEN_DOORS_1_ID 43

	#define  CV_BLOCK_TYPE_BRACED_DOORS_ID 445
	#define  CV_BLOCK_TYPE_BRACED_DOORS_0_ID 44
	#define  CV_BLOCK_TYPE_BRACED_DOORS_1_ID 45

	#define  CV_BLOCK_TYPE_IRON_DOORS_ID 467
	#define  CV_BLOCK_TYPE_IRON_DOORS_0_ID 46
	#define  CV_BLOCK_TYPE_IRON_DOORS_1_ID 47

	#define  CV_BLOCK_TYPE_MAGIC_DOORS_ID 489
	#define  CV_BLOCK_TYPE_MAGIC_DOORS_0_ID 48
	#define  CV_BLOCK_TYPE_MAGIC_DOORS_1_ID 49

	#define  CV_BLOCK_TYPE_BRIDGE_ID 51
	#define  CV_BLOCK_TYPE_GEM_ID 52
	#define  CV_BLOCK_TYPE_GUARD_POST_ID 53

	// light types
	#define CV_LIGHT_TYPE_TORCH		"TORCH"
	#define CV_LIGHT_TYPE_CANDLE	"CANDLE"
	#define CV_LIGHT_TYPE_LAVA		"LAVA"
	#define CV_LIGHT_TYPE_WATER		"WATER"
};

#endif // COMMONS_H