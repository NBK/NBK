#ifndef ROOM_CONSTRUCTOR_H
#define ROOM_CONSTRUCTOR_H

#include <windows.h>
#include <gl/gl.h>
#include "SubtileData.h"
#include "Block.h"

/*
	Room tiles are (usually) of a very complex structure. 
	So instead of dropping everything into BlockManager
	and making it even bigger. I decided to make this
	"static" class that handles the room geometry 
	construction.
*/

namespace game_utils
{
	class CRoomConstructor
	{
	public:
		CRoomConstructor();
		~CRoomConstructor();

		/*
			Constructs room geometry and texture info for this room block.
		*/
		GLvoid createRoomData(game_objects::CBlock *block);

		GLint getEdgeSubtiles(GLfloat xps[], GLfloat yps[]);
		GLint getEdgeSubtiles(GLint subtiles[]);
		GLint getEdgeSubtiles4(GLint subtiles[]);
		GLint getEdgeSubtilesWhereNoFullBlock(GLfloat xps[8], GLfloat yps[8], GLint subtiles[8]);

		bool isColumnTile();
		bool isEdgeTile4();
		bool isEdgeTile8();
		bool isCenterTile();

	private:

		game_objects::CBlock *targetBlock;

		/*
			If this room tile has a column then 
			this 2 variables represent its position
			within the tile.
		*/
		GLint	column_location_x,
				column_location_z;

		// creates vertices for specific bottom types
		GLvoid createDeformedBottom();

		// variables for tile state
		bool	column,
				border,
				bookshelf,
				portal_center,
				heart_center,
				hear_flame;

		// temp bottom structure
		struct DEFORMED_BOTTOM
		{
			cml::vector3f bottom[4][4];
		};

		DEFORMED_BOTTOM deformed_bottom,
						deformed_border0,
						deformed_border1;		

		game_utils::CSubtileData	BTM_S[9],
									CLM_S[7],
									BRD0_S[9],	
									SIDE_BRD0_S[3];

		// temp column structure
		struct DEFORMED_COLUMN
		{
			//			   z  y  x
			cml::vector3f column[2][7][2];
		};

		DEFORMED_COLUMN deformed_column;
		GLint			column_height;

		// PORTAL specific
		struct DEFORMED_ROOF
		{
			cml::vector3f area[6][6];
		};

		DEFORMED_ROOF	deformed_roof_top,
						deformed_roof_bottom;

		game_utils::CSubtileData	PORTAL_ROOF_TOP[25],
									PORTAL_ROOF_BOTTOM[25],
									PORTAL_ROOF_SIDE[5];
		
		// HEART specific
		typedef DEFORMED_ROOF DEFORMED_HEART_PEDESTAL_UPPER;

		DEFORMED_HEART_PEDESTAL_UPPER	deformed_heart_pedestal_upper_top,
										deformed_heart_pedestal_upper_bottom,
										deformed_heart_pedestal_corner_top,
										deformed_heart_pedestal_corner_middle;		

		game_utils::CSubtileData	HEART_PEDESTAL_UPPER_TOP[25],
									HEART_PEDESTAL_UPPER_BOTTOM[25],
									HEART_PEDESTAL_SIDE[5],
									HEART_PEDESTAL_LOWER_TOP[49],
									HEART_PEDESTAL_LOWER_BOTTOM[49],
									HEART_PEDESTAL_CORNER_TOP[9],
									HEART_PEDESTAL_CORNER_MIDDLE[9];

		struct DEFORMED_HEART_PEDESTAL_LOWER
		{
			cml::vector3f area[8][8];
		};

		DEFORMED_HEART_PEDESTAL_LOWER	deformed_heart_pedestal_lower_top,
										deformed_heart_pedestal_lower_bottom;		

		// TEMPLE specific variables
		bool temple_animation;

		game_utils::CSubtileData	BRD1_S[9],
									SIDE_BRD1_S[9];

		enum TEMPLE_BORDER1_TYPE
		{
			TB_LEFT,
			TB_RIGHT,
			TB_UP,
			TB_DOWN,
			TB_ALL,
			TB_NONE,
			TB_TL,
			TB_TR,
			TB_BR,
			TB_BL
		};

		TEMPLE_BORDER1_TYPE temple_border1_type;

		/*
			Fills-in the correct texture coordinates for every possible room type.
		*/
		GLvoid fillRoomSubtiles(game_utils::CSubtileData SUB_DATA[]);

		enum TILE_TYPE
		{ 
			TT_TL,
			TT_T,
			TT_TR,
			TT_L,
			TT_R,
			TT_BL,
			TT_B,
			TT_BR,
			TT_M
		};
		TILE_TYPE tile_type;

		// HATCHERY specific
		bool border_skip[9];

		// LIBRARY specific
		game_utils::CSubtileData	bookshelf_front[9],
									bookshelf_back[9],
									bookshelf_top[3],
									bookshelf_left_side[3],
									bookshelf_right_side[3];


		// generate geometry for a column
		GLvoid initColumn();

		// generate geometry for border
		GLvoid initBorder();

		// draw special borders
		GLvoid draw_border1(GLint subtile_pos);

		GLfloat ntw,
				nth;

		/* 
			Some rooms have items that are the part of their visual representation. Here
			we setup this models.
		*/
		GLvoid addTileItems();
	};
};

#endif // ROOM_CONSTRUCTOR_H