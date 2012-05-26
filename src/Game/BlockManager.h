#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

#include "Block.h"
#include "Manager.h"
#include <map>
#include "boundingBox.h"
#include "SubtileData.h"
#include "RoomConstructor.h"

/*
	This class takes care of loading settings for different blocks.
*/

namespace game_utils
{
	namespace managers
	{
		class CBlockManager: public CManager
		{
		public:
			CBlockManager();
			~CBlockManager();

			/*
				Reads block settings from "data\\resources\\blocks.conf".
			*/
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			/*
				Called by a block, it generates geometry for this block.
				Read texture coordinates for every block subtile from file.
			*/
			GLvoid	generateBlockGeometry(game_objects::CBlock *block);						

			/*
				Used when we want to reload points if we have deformed them. 
				This happens when animating lava and water...
			*/
			GLvoid	reloadSubLevelVertices(game_objects::CBlock *block);

			/*
				Using this function we can update y part of any GDFC point. Usefull in terrain animation.
			*/
			GLvoid	setGlobalDeformedMapPoint(GLint cx, GLint cy, GLint dx, GLint dy, GLint dz, GLfloat ny);

			/*
				Usually called from RoomConstructor and Block when creating geometry.
			*/
			cml::vector3f	getGlobalDeformedMapPoint(GLint cx, GLint cy, GLint dx, GLint dy, GLint dz);

			GLint			getTexturePosInTextureAtlas(GLint terrainType, GLint owner);

		private:

			/*
				...
			*/
			bool readTextureAtlasCoordinates();

			/*
				Parses the line and extracts block parameters:
				SLB index, block name, texture index
			*/
			GLvoid parseLine(std::string &line);

			cml::vector3f calculatePerlin3D(cml::vector3f vec);

			/*
				Temporary structure for level creation. Only gets instatiated
				once per game start. Blocks have same geometry through gameplay.

				We can delete this structure ONLY on shutdown since we need data for dynamic
				ceiling creation.		

				 - water and lava start on index 0 to 1
				 - normal blocks start with bottom on index 1 and end with top on index CV_BLOCK_RESOLUTION_Y (usually 4)
			*/
			cml::vector3f ***globalDeformedMap;

			bool createGlobalDeformedMap();		

			/*
				Temp arrays for forming block types.
			*/

			GLint rock_subtiles[12];
			GLint earth_subtiles[12];
			GLint normal_wall_no_edge[12];
			GLint normal_wall_2_edges[12];
			GLint normal_wall_ending_left[12];
			GLint normal_wall_ending_right[12];

			GLint normal_wall_painting0[12];
			GLint normal_wall_painting1[12];
			GLint normal_wall_painting2[12];
			GLint normal_wall_hang[12];

			GLint room_wall_prison_edge[12];
			GLint room_wall_prison_middle[12];

			GLint room_wall_treasury_edge[12];
			GLint room_wall_treasury_middle[12];

			GLint room_wall_lair_edge[12];
			GLint room_wall_lair_middle[12];

			GLint room_wall_hatchery_edge[12];
			GLint room_wall_hatchery_middle[12];

			GLint room_wall_train_edge[12];
			GLint room_wall_train_middle[12];
			GLint room_wall_train_edge_2[12];
			GLint room_wall_train_left_edge[12];
			GLint room_wall_train_right_edge[12];

			GLint room_wall_library_book_case[12];
			GLint room_wall_library_edge_2[12];
			GLint room_wall_library_left_edge[12];
			GLint room_wall_library_right_edge[12];

			GLint room_wall_torture_middle[12];

			GLint room_wall_barracks_edge[12];
			GLint room_wall_barracks_middle[12];
			GLint room_wall_barracks_edge_2[12];
			GLint room_wall_barracks_left_edge[12];
			GLint room_wall_barracks_right_edge[12];

			GLint room_wall_temple_edge[12];
			GLint room_wall_temple_middle[12];
			GLint room_wall_temple_edge_2[12];
			GLint room_wall_temple_left_edge[12];
			GLint room_wall_temple_right_edge[12];

			GLint room_wall_graveyard_middle[12];
			GLint room_wall_graveyard_edge_2[12];
			GLint room_wall_graveyard_left_edge[12];
			GLint room_wall_graveyard_right_edge[12];

			GLint room_wall_workshop_edge[12];
			GLint room_wall_workshop_edge_2[12];
			GLint room_wall_workshop_left_edge[12];
			GLint room_wall_workshop_right_edge[12];

			#define MAX_ROOM_ALL	11		// every room has this graphics on it's walls

			GLint middle_wall_rooms[MAX_ROOM_ALL];	
			GLint *room_wall_edge[MAX_ROOM_ALL];
			GLint *room_wall_middle[MAX_ROOM_ALL];	

			#define MAX_ROOM_NOT_ALL	6

			GLint left_right_wall_rooms[MAX_ROOM_NOT_ALL];
			GLint *room_wall_edge_2[MAX_ROOM_NOT_ALL];
			GLint *room_wall_left_edge[MAX_ROOM_NOT_ALL];
			GLint *room_wall_right_edge[MAX_ROOM_NOT_ALL];		

			GLint	type,
					ltype,
					rtype,
					utype,
					ultype,
					urtype,
					dtype,
					drtype;			

			// 2000 is just some value that must be bigger than number of entries
			GLint textureAtlasData[2000][6];			

			game_utils::CSubtileData	BTM_S[9],	// bottom subtiles
										TOP_S[9],	// top subtiles
										FNT_S[12],	// front subtiles
										BCK_S[12],	// back subtiles
										LFT_S[12],	// left subtiles
										RGT_S[12];	// right subtiles

			/*
				Each face has 3x3 subtiles, each with it's own texture:
				T - top
				B - bottom
				L - left		
				R - right		
				M - middle
			*/
			enum SUBTILE_REFERENCE{SR_TL=0, SR_TM, SR_TR, SR_ML, SR_M, SR_MR, SR_BL, SR_BM, SR_BR};

			/*
				Copies data from *_S to blocks texture coordinates array.
			*/
			GLvoid updateTextureCoordinates(game_objects::CBlock *block, GLint face);

			bool	setRoomWallLeftRightEdge(GLint ntype, GLint terrainType, GLint room_wall[], game_utils::CSubtileData SUB_DATA[]);
			bool	setFrontRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], game_utils::CSubtileData SUB_DATA[]);
			bool	setBackRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], game_utils::CSubtileData SUB_DATA[]);
			bool	setLeftRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], game_utils::CSubtileData SUB_DATA[]);
			bool	setRightRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], game_utils::CSubtileData SUB_DATA[]);

			GLvoid	setTopWallCorner(game_utils::CSubtileData SUB_DATA[], GLint pos);
			GLvoid	setAnimationWall(game_utils::CSubtileData SUB_DATA[], GLint near_terrainType);
			GLvoid	setNormalWall(game_utils::CSubtileData SUB_DATA[]);

			GLvoid fillSubtiles_FRONT(game_objects::CBlock *block);
			GLvoid fillSubtiles_BACK(game_objects::CBlock *block);
			GLvoid fillSubtiles_LEFT(game_objects::CBlock *block);
			GLvoid fillSubtiles_RIGHT(game_objects::CBlock *block);
			GLvoid fillSubtiles_TOP(game_objects::CBlock *block);
			GLvoid fillSubtiles_BOTTOM(game_objects::CBlock *block);			
			GLvoid fillSubtiles(game_objects::CBlock *block);

			// temp variables holding position of the current block
			GLint	mapX,
					mapY;
			GLint	owner;

			/*
				CEILING CALCULATIONS
			*/						
			bool			isMaxHeightEdgeCeilingPatch(bool edges[], cml::vector2i lPos);
			GLvoid			calculateCeiling(cml::vector2i lPos, cml::vector3f ceilingPatch[4][4]);			

			enum CEILING_SUBPATCH_RELATION{CSR_LOWER=0, CSR_HIGHER, CSR_NOT_EDGE, CSR_SAME};

			GLvoid getEdgeCeilingSubPatches(CEILING_SUBPATCH_RELATION csr[], cml::vector2i lPos);			

			/*
				Must be called every time somethig on the block changes. 
				 - earth to wall <->
                 - room nearby
				 - earth to low
				 - low to lava
				 ...
			*/
			GLvoid setupGFXAndCeiling(game_objects::CBlock *block);

			/*
				Used when a tile is a room tile. 
			*/
			CRoomConstructor *roomConstructor;

			GLfloat textureAtlasWidth,
					textureAtlasHeight,
					textureAtlasSubtileSize,
					pixelWidth,
					pixelHeight;
		};
	};
}

#endif // BLOCK_MANAGER_H