#ifndef BLOCK_H
#define BLOCK_H

#include "BlockObject.h"
#include <windows.h>
#include <gl/gl.h>
#include <cml/cml.h>
#include "BoundingBox.h"

/*
	Games base building block. There are several types of blocks:
	 - gold
	 - gem
	 - earth
	 - impenetrable rock
	 - claimed floor (player dependant)
	 - unclaimed floor
	 - water
	 - lava
	 - several wall types (different for each room type and player)
*/

namespace game_objects
{
	class CBlock
	{
	public:

		CBlock();
		~CBlock();

		/*
			When cleaning up the objects of the block
			we can specify wether we wanna remove them all
			(eg; when deleting block) of removing only the
			marked (flagged) ones  (eg; when building a room)
		*/
		enum OBJECT_DISPOSE_TYPE
		{
			ODT_REMOVE_ALL = 0,	
			ODT_REMOVE_MARKED
		};

		/*
			Sets or returns block type.
		*/
		GLvoid	setType(GLint Type);
		GLint	getType();

		/*
			Sets or returns block logical|real position.
		*/
		GLvoid			setLogicalPosition(cml::vector2i &position);
		cml::vector2i	getLogicalPosition();
		cml::vector3f	getRealPosition();
		cml::vector3f	getCenterPosition();

		/*
			Sets or returns owner.
		*/
		GLvoid	setOwner(GLubyte owner);
		GLubyte getOwner();	

		/*
			Initializes the block:
				- determine visible surfaces
				- calculates triangles				
		*/
		GLvoid init();

		/*
			Must be called afer every block has been inited! Creates
			texture template for this block according to neighbour
			blocks.
		*/
		GLvoid finalize();

		/* 
			Flags for face type
		*/
		enum BLOCK_FACE_SELECTOR{BFS_FRONT=0, BFS_BACK, BFS_LEFT, BFS_RIGHT, BFS_TOP, BFS_BOTTOM, BFS_WATER_LAVA, BFS_CEILING}; 

		/*
			Returns true if block is not a full block.
		*/
		bool isLow();
		bool isWater();
		bool isLava();
		bool isRoom();
		bool isInRoom();
		bool hasTorch();

		GLvoid setRoomIndex(GLint roomIndex);

		GLint getRoomIndex();

		/*
			Checks each face (front=0, back=1, left, right) if there is
			a full block infront of them. This is usefull only with full
			blocks. Usually called by the light manager when detemining
			where and how many lights to position. 
		*/
		GLint	getFreeFaces(GLint faces[4]);

		/*
			Returns accurate bounding box for this block.
		*/
		geometry::sBoundingBox *getBoundingBox();

		/*
			Used check if a face is visible.
		*/
		bool isFaceVisible(BLOCK_FACE_SELECTOR faceSelector);
		bool isFaceVisible(GLint face);

		struct sSimpleFace
		{
			cml::vector3f a,b,c,d;
		};

		/*
			Returns a 4 point approximation of a face. Usefull
			for collision detection. If face is not visible
			function returns false.
		*/
		bool getFaceAproximation(BLOCK_FACE_SELECTOR faceSelector, sSimpleFace &faceApprox);

		GLfloat **getVertices();
		GLfloat **getTextureCoordinates();
		GLfloat *getTextureCoordinates(BLOCK_FACE_SELECTOR faceSelector);

		/*
			Returns the count of subtiles and the subtile index 
			of the subtiles that are on block edge.
		*/
		GLint getEdgeSubtiles(GLint subtiles[]);

		GLvoid setCeilingHeight(GLint ceilingHeight);

		GLint getCeilingHeight();

		/*
			If the block has the texture animation we update
			texture coordinates for the next frame.
		*/
		GLvoid updateTexture(GLfloat delta);

		/*
			Sets the number of frames for a selected block face. 
			Each face may have a different number of animation 
			frames.
		*/
		GLvoid setNumberOfTextureFrames(BLOCK_FACE_SELECTOR faceIndex, GLint numberOfFrames);

		/*
			Returns the skip flags for terrain animation.
		*/
		bool getSkipFlag(GLint x, GLint y);

		/*
			This way we control the visiblity of the specific block face.
		*/
		GLvoid setVisible(BLOCK_FACE_SELECTOR faceSelector, bool visibility);

		std::vector<GLuint> *getDisplayLists();

		std::vector<block_objects::CBlockObject*> *getBlockObjects();

		GLvoid disposeDisplayLists();
		GLvoid disposeBlockObjects(OBJECT_DISPOSE_TYPE odt = ODT_REMOVE_MARKED);

		// returns true is this block is sellable (block is room and player id match)
		bool isSellable(GLint playerID);
		bool isBuildable(GLint playerID);

		// call this to add room model
		GLvoid addModel(std::string modelName, cml::vector3f position, bool marked = false, GLfloat rotateY = 0.0f);
		GLvoid addModel(block_objects::CBlockObject *blockObject);

		// removes a block object from the block object list. Usefull when selling traps, or picking gold...
		GLvoid removeBlockObject(block_objects::CBlockObject *blockObject);

	private:

		GLubyte			owner;
			
		GLint			type,
						ceilingHeight; // Contains ceiling height. Used in ceiling patch creation.

		cml::vector2i	logicalPosition;
		cml::vector3f	realPosition;

		/*
			Points that construct this block. Faces are as follows:

			FRONT:	(RESX-1)*(RESY-1)*4*3
			BACK:		-||-
			LEFT:		-||-
			RIGHT:		-||-
			TOP:	(RESX-1)*(RESZ-1)*4*3
			BOTTOM:		-||-		<== claimed, unclaimed land
			WATER_LAVA: -||-		<== water, lava
			CEILING		-||-
			  ||
			  8

			Last 3 will have same array for floats for optimization purposes.
		*/
		GLfloat	*vertices[8];

		/*
			Each of 8 faces may have max 4 different texture animation frames.
		*/
		GLfloat	*textureCoordinates[8*4];

		/*
			Tells which face is visible.
		*/
		bool	visible[8],
				// true, if this block is low ie.: lava, water, path, room...
				low, 
				// true, if this block is water
				water, 
				// true, if this block is lava
				lava,
				// true, if this block is a room
				room,
				// true, if this block contains a torch
				torch,
				/* 
					Used in terrain animation. Fields marked with true will 
					not get deformed. Must be setup in finalize.
				*/
				skip[4][4],
				// true, whe n the block has finished loading for the first time
				finalized;

		GLint	roomIndex;
				

		GLint	animatedTexture[8],
				animatedTexturePos[8]; // any face may have texture animation

		GLfloat	animatedTextureChange[8];

		/*
			Bounding box of our blocks geometry.
		*/
		geometry::sBoundingBox boundingBox;

		/* ROOM TILES ONLY */

		/*
			All rooms have some special geometries like borders, columns... and
			storing all that data in the RAW form would be a waste of resources.
			So we rather create opengl display lists.
		*/
		std::vector<GLuint> displayLists;

		std::vector<block_objects::CBlockObject*> blockObjects;
	};
};

#endif // BLOCK_H