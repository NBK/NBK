#include "commons.h"
#include "Block.h"
#include "Perlin.c"
#include "Trap.h"

using namespace cml;
using namespace game_utils;
using namespace game_utils::managers;
using namespace loaders;
using namespace geometry;
using namespace std;
using namespace game_objects::block_objects;

namespace game_objects
{

	CBlock::CBlock():	
		type(-1), 
		owner(CV_PLAYER_UNDEFINED), 
		logicalPosition(vector2i(-1,-1)), 
		realPosition(vector3f(0.0f,0.0f,0.0f)),
		low(false), 
		water(false), 
		lava(false),
		room(false),
		torch(false),
		marked(false),
		taken(false),
		roomIndex(-1),
		finalized(false),
		ceilingHeight(-1),
		highlighted(false)
	{
		GLint tPos = 0;
		for (GLint i=0; i<8; i++)
		{
			animatedTexture[i] = 1;
			animatedTexturePos[i] = 0;
			animatedTextureChange[i] = 1.0f;

			vertices[i] = new GLfloat[CV_FBLR_FACE_VERT_FLOATS];
			ZeroMemory(vertices[i],sizeof(GLfloat)*CV_FBLR_FACE_VERT_FLOATS);

			for (GLint j=0; j<4; j++)
			{
				textureCoordinates[tPos] = new GLfloat[CV_FBLR_FACE_TEX_FLOATS];
				ZeroMemory(textureCoordinates[tPos],sizeof(GLfloat)*CV_FBLR_FACE_TEX_FLOATS);
				tPos++;
			}
		}
	}

	CBlock::~CBlock()
	{
		GLint tPos = 0;
		for (GLint i=0; i<8; i++)
		{
			delete [] vertices[i];

			for (GLint j=0; j<4; j++)
			{
				delete [] textureCoordinates[tPos++];
			}
		}

		disposeDisplayLists();
		disposeBlockObjects(ODT_REMOVE_ALL);
	}

	GLvoid CBlock::finalize()
	{
		CV_GAME_MANAGER->getBlockManager()->generateBlockGeometry(this);

		// now that we have the geometry we can calculate bounding box	
		// TODO: in release an exact bbox should be calculated??
		boundingBox.reset();

		boundingBox.update(vertices[BFS_FRONT]);
		boundingBox.update(vertices[BFS_BACK]);
		boundingBox.update(vertices[BFS_LEFT]);
		boundingBox.update(vertices[BFS_RIGHT]);
		boundingBox.update(vertices[BFS_TOP]);
		boundingBox.update(vertices[BFS_WATER_LAVA]);

		boundingBox.update(vector3f(vertices[BFS_TOP][0],CV_BLOCK_HEIGHT*4.0f,vertices[BFS_TOP][2])); // ceiling can be pretty high
		
		boundingBox.calculateExtents();
		boundingBox.calculateVertices();

		// init the skip flags
		if (lava||water)
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			GLint mapX = logicalPosition[0];
			GLint mapY = logicalPosition[1];

			GLint ltype = lManager->getBlockType(mapX-1,mapY);
			GLint rtype = lManager->getBlockType(mapX+1,mapY);
			GLint utype = lManager->getBlockType(mapX,mapY-1);
			GLint dtype = lManager->getBlockType(mapX,mapY+1);

			GLint lutype = lManager->getBlockType(mapX-1,mapY-1);
			GLint ldtype = lManager->getBlockType(mapX-1,mapY+1);
			GLint rutype = lManager->getBlockType(mapX+1,mapY-1);
			GLint rdtype = lManager->getBlockType(mapX+1,mapY+1);

			bool l = (ltype==CV_BLOCK_TYPE_LAVA_ID || ltype==CV_BLOCK_TYPE_WATER_ID);
			bool r = (rtype==CV_BLOCK_TYPE_LAVA_ID || rtype==CV_BLOCK_TYPE_WATER_ID);
			bool u = (utype==CV_BLOCK_TYPE_LAVA_ID || utype==CV_BLOCK_TYPE_WATER_ID);
			bool d = (dtype==CV_BLOCK_TYPE_LAVA_ID || dtype==CV_BLOCK_TYPE_WATER_ID);

			bool lu = (lutype==CV_BLOCK_TYPE_LAVA_ID || lutype==CV_BLOCK_TYPE_WATER_ID);
			bool ru = (rutype==CV_BLOCK_TYPE_LAVA_ID || rutype==CV_BLOCK_TYPE_WATER_ID);
			bool ld = (ldtype==CV_BLOCK_TYPE_LAVA_ID || ldtype==CV_BLOCK_TYPE_WATER_ID);
			bool rd = (rdtype==CV_BLOCK_TYPE_LAVA_ID || rdtype==CV_BLOCK_TYPE_WATER_ID);

			for (GLint i=0; i<4; i++)
			{
				ZeroMemory(skip[i],sizeof(bool)*4);
			}

			skip[0][0]=skip[1][0]=skip[2][0]=skip[3][0]=!l;
			skip[0][3]=skip[1][3]=skip[2][3]=skip[3][3]=!r;
			skip[0][0]=skip[0][1]=skip[0][2]=skip[0][3]=!u;
			skip[3][0]=skip[3][1]=skip[3][2]=skip[3][3]=!d;

			skip[0][0]=!l||!u||!lu;
			skip[0][3]=!r||!u||!ru;
			skip[3][3]=!r||!d||!rd;
			skip[3][0]=!l||!d||!ld;
		}

		finalized = true;
	}

	GLvoid CBlock::init()
	{
		// 1. setup visible surfaces		
		if (isLow())
		{
			visible[BFS_TOP]		= false;			
			visible[BFS_WATER_LAVA]	= isWater() || isLava();
			visible[BFS_BOTTOM]		= !visible[BFS_WATER_LAVA];
			visible[BFS_CEILING]	= true;
		}
		else
		{
			visible[BFS_TOP]		= true;
			visible[BFS_BOTTOM]		= false;
			visible[BFS_WATER_LAVA]	= false;
			visible[BFS_CEILING]	= false;
		}

		//visible[BFS_TOP]		= false; // uncomment to see the optimization

		vector2i tests[4] =
		{
			vector2i(0,1),	// BFS_FRONT
			vector2i(0,-1), // BFS_BACK
			vector2i(-1,0), // BFS_LEFT
			vector2i(1,0),	// BFS_RIGHT
		};  

		CBlock *testBlock = NULL;
		for (GLint i=BFS_FRONT; i<=BFS_RIGHT; i++)
		{
			testBlock = CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition+tests[i]);
			visible[i] = isLow()?false:(testBlock && testBlock->isLow() || !testBlock);

			if (water || lava)
			{
				// since water and lava are submerged by 1 level we nned to handle them differently
				visible[i] = testBlock && (!testBlock->low || 
											testBlock->type==CV_BLOCK_TYPE_CLAIMED_LAND_ID ||
											testBlock->type==CV_BLOCK_TYPE_UNCLAIMED_LAND_ID);	
			}
		}

		// tmp 
		/*for (GLint v=BFS_FRONT+1; v<=BFS_WATER_LAVA; v++)
		{
			//if (v!=BFS_BOTTOM)
			{
				visible[v]=false;
			}
		}*/

		/*
			For every block change you must call:
			- block init
			- calc block height if needed
			- finalize
		*/
	}

	GLvoid CBlock::setType(GLint type)	
	{
		if(this->type == CV_BLOCK_TYPE_UNCLAIMED_LAND_ID)
			CV_GAME_MANAGER->getLevelManager()->removeUnclaimedBlock(this);

		this->type=type;
		this->taken=false;

		disposeBlockObjects();
		disposeDisplayLists();

		// join all wall types into 1
		if (type==CV_BLOCK_TYPE_WALL_0_ID ||
			type==CV_BLOCK_TYPE_WALL_1_ID ||
			type==CV_BLOCK_TYPE_WALL_2_ID ||
			type==CV_BLOCK_TYPE_WALL_3_ID ||
			type==CV_BLOCK_TYPE_WALL_4_ID)
		{
			this->type = CV_BLOCK_TYPE_WALL_ID;
		}

		if (this->type==CV_BLOCK_TYPE_EARTH_WITH_TORCH_PLATE_ID)
		{
			torch = true;
			this->type = CV_BLOCK_TYPE_EARTH_ID;
		}
		else if (this->type==CV_BLOCK_TYPE_WALL_WITH_TORCH_PLATE_ID)
		{
			torch = true;
			this->type = CV_BLOCK_TYPE_WALL_ID;
		}

		// tmp hack, since doors are not supported yet
		if (this->type==CV_BLOCK_TYPE_WOODEN_DOORS_ID	||
			this->type==CV_BLOCK_TYPE_BRACED_DOORS_ID	||
			this->type==CV_BLOCK_TYPE_IRON_DOORS_ID		||
			this->type==CV_BLOCK_TYPE_MAGIC_DOORS_ID)
		{
			this->type=CV_BLOCK_TYPE_UNCLAIMED_LAND_ID;
		}

		// setup quick variables

		low		= !(this->type==CV_BLOCK_TYPE_ROCK_ID ||
					this->type==CV_BLOCK_TYPE_GOLD_ID ||
					this->type==CV_BLOCK_TYPE_EARTH_ID ||				 
					this->type==CV_BLOCK_TYPE_WALL_ID ||
					this->type==CV_BLOCK_TYPE_GEM_ID ||
					this->type==CV_BLOCK_TYPE_EARTH_WITH_TORCH_PLATE_ID ||
					this->type==CV_BLOCK_TYPE_WALL_WITH_TORCH_PLATE_ID);

		water	= (this->type==CV_BLOCK_TYPE_WATER_ID);
		lava	= (this->type==CV_BLOCK_TYPE_LAVA_ID);

		room	= low && !water && !lava && this->type!=CV_BLOCK_TYPE_CLAIMED_LAND_ID && this->type!=CV_BLOCK_TYPE_UNCLAIMED_LAND_ID;	
		marked  = false;
		
		if (roomIndex!=-1)
		{
			// if we were are a part of a room, we need to inform this room that we are being modified
			CV_GAME_MANAGER->getRoomManager()->getRoom(roomIndex)->removeBlock(this,true);
		}
		else if (finalized && room && roomIndex==-1)
		{
			// we are a room, notify the RoomManager
			CV_GAME_MANAGER->getRoomManager()->addRoomTile(this);
		}

		if(this->type == CV_BLOCK_TYPE_UNCLAIMED_LAND_ID)
			CV_GAME_MANAGER->getLevelManager()->addUnclaimedBlock(this);

		if(this->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->addUnfortifiedBlock(this);

		if(this->type == CV_BLOCK_TYPE_WALL_ID)
			CV_GAME_MANAGER->getLevelManager()->removeUnfortifiedBlock(this);

		resetLife();
	}

	GLint CBlock::getType()
	{
		return type;
	}

	GLvoid CBlock::setLogicalPosition(vector2i &position)
	{
		this->logicalPosition=position;

		// calculate real position
		realPosition = vector3f((GLfloat)position[0]*CV_BLOCK_WIDTH,0.0f,(GLfloat)position[1]*CV_BLOCK_DEPTH);
	}
	
	vector2i CBlock::getLogicalPosition()
	{
		return logicalPosition;
	}
	
	vector3f CBlock::getRealPosition()
	{
		return realPosition;	
	}

	vector3f CBlock::getCenterPosition()
	{
		return realPosition+vector3f(CV_BLOCK_WIDTH/2.0f,0.0,CV_BLOCK_DEPTH/2.0f);
	}

	GLvoid CBlock::setOwner(GLubyte owner)
	{
		this->owner=owner;
	}

	GLubyte CBlock::getOwner()
	{
		return owner;
	}

	bool CBlock::isLow()
	{
		return low;
	}

	bool CBlock::isWalkable(bool walkOnLava)
	{
		return low && (walkOnLava?true:!lava);
	}

	bool CBlock::isWater()
	{		
		return water;
	}

	bool CBlock::isLava()
	{		
		return lava;
	}

	bool CBlock::isRoom()
	{		
		return room;
	}

	bool CBlock::hasTorch()
	{		
		return torch;
	}

	bool CBlock::isHighlighted()
	{
		return highlighted;
	}

	GLvoid CBlock::setHighlighted(bool highlighted)
	{
		this->highlighted = highlighted;
	}

	sBoundingBox *CBlock::getBoundingBox()
	{
		return &boundingBox;
	}

	bool CBlock::isFaceVisible(BLOCK_FACE_SELECTOR faceSelector)
	{
		return (faceSelector>=BFS_FRONT && faceSelector<=BFS_CEILING)?visible[faceSelector]:false;
	}

	bool CBlock::isFaceVisible(GLint face)
	{
		return (face>=BFS_FRONT && face<=BFS_CEILING)?visible[face]:false;
	}

	bool CBlock::getFaceAproximation(BLOCK_FACE_SELECTOR faceSelector, sSimpleFace &faceApprox)
	{
		if (faceSelector>BFS_BOTTOM || faceSelector==BFS_TOP || !isFaceVisible(faceSelector))
		{
			return false;
		}

		switch (faceSelector)
		{
			case BFS_FRONT:
			{
				faceApprox.a = boundingBox.E;
				faceApprox.b = boundingBox.F;
				faceApprox.c = boundingBox.B;
				faceApprox.d = boundingBox.A;
				break;
			}

			case BFS_BACK:
			{
				faceApprox.a = boundingBox.G;
				faceApprox.b = boundingBox.H;
				faceApprox.c = boundingBox.D;
				faceApprox.d = boundingBox.C;
				break;
			}

			case BFS_RIGHT:
			{
				faceApprox.a = boundingBox.C;
				faceApprox.b = boundingBox.B;
				faceApprox.c = boundingBox.F;
				faceApprox.d = boundingBox.G;
				break;
			}

			case BFS_LEFT:
			{
				faceApprox.a = boundingBox.H;
				faceApprox.b = boundingBox.E;
				faceApprox.c = boundingBox.A;
				faceApprox.d = boundingBox.D;
				break;
			}
		}

		return true;
	}

	GLfloat **CBlock::getVertices()
	{
		return vertices;
	}

	GLfloat *CBlock::getTextureCoordinates(BLOCK_FACE_SELECTOR faceSelector)
	{
		return textureCoordinates[faceSelector+animatedTexturePos[faceSelector]*8];
	}

	GLfloat **CBlock::getTextureCoordinates()
	{
		return textureCoordinates;
	}

	GLint CBlock::getEdgeSubtiles(GLint subtiles[])
	{		
		/*
			_____________
			|0	|1	|2	| ly2
			|___|___|___|
			|3	|4	|5	| ly1
			|___|___|___|
			|6	|7	|8	| ly0
			|___|___|___|
			lx0 lx1 lx2
		*/

		GLint arrayPos=0;

		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint mapX = logicalPosition[0];
		GLint mapY = logicalPosition[1];

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX-1,mapY,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX,mapY-1,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX-1,mapY-1,this))
		{
			subtiles[arrayPos++]=0;
		}

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX,mapY-1,this))
		{
			subtiles[arrayPos++]=1;
		}

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX+1,mapY,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX,mapY-1,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX+1,mapY-1,this))
		{
			subtiles[arrayPos++]=2;
		}

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX-1,mapY,this))
		{
			subtiles[arrayPos++]=3;
		}

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX+1,mapY,this))
		{
			subtiles[arrayPos++]=5;
		}

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX-1,mapY,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX,mapY+1,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX-1,mapY+1,this))
		{
			subtiles[arrayPos++]=6;
		}

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX,mapY+1,this))
		{
			subtiles[arrayPos++]=7;
		}

		if (lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX+1,mapY,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX,mapY+1,this) ||
			lManager->isNotSameTypeAndOwnerAndNotRockOrEarth(mapX+1,mapY+1,this))
		{
			subtiles[arrayPos++]=8;
		}

		return arrayPos;
	}

	
	GLvoid CBlock::setCeilingHeight(GLint ceilingHeight)
	{
		this->ceilingHeight=ceilingHeight;
	}

	GLint CBlock::getCeilingHeight()
	{
		return ceilingHeight;
	}

	GLvoid CBlock::setNumberOfTextureFrames(BLOCK_FACE_SELECTOR faceIndex, GLint numberOfFrames)
	{
		numberOfFrames=(numberOfFrames>4?4:numberOfFrames);

		if ((type==CV_BLOCK_TYPE_LAVA_ID || type==CV_BLOCK_TYPE_WATER_ID)&& faceIndex>=BFS_FRONT && faceIndex<=BFS_RIGHT)
		{
			// hack: there was a bug in previous version of texture animation. 
			// some blocks near lava were flashing. this fixes the problem.
			numberOfFrames=0;
		}

		animatedTexture[faceIndex] = numberOfFrames;

		animatedTexturePos[faceIndex] = numberOfFrames>0?rand()%numberOfFrames:0;
	}

	GLvoid CBlock::updateTexture(GLfloat delta)
	{

		for (GLint f=BFS_FRONT; f<=BFS_WATER_LAVA; f++)
		{
			if (animatedTexture[f]>1)
			{
				animatedTextureChange[f]-=0.01f*delta;

				if (animatedTextureChange[f]<=0.0f)
				{
					animatedTextureChange[f]=1.0f;
					animatedTexturePos[f]++;

					if (animatedTexturePos[f]>=animatedTexture[f])
					{
						animatedTexturePos[f]=0;
					}
				}
			}
		}
	}

	bool CBlock::getSkipFlag(GLint x, GLint y)
	{
		return skip[y][x];
	}

	GLint CBlock::getFreeFaces(GLint faces[4])
	{
		GLint cnt = 0;

		if (isLow())
		{
			// a special type of light. could be described as a torch in the air.
			return -1;
		}

		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		if (lManager->getBlock(logicalPosition+vector2i(0,1))->isLow())
		{
			faces[cnt++]=BFS_FRONT;
		}

		if (lManager->getBlock(logicalPosition+vector2i(0,-1))->isLow())
		{
			faces[cnt++]=BFS_BACK;
		}

		if (lManager->getBlock(logicalPosition+vector2i(1,0))->isLow())
		{
			faces[cnt++]=BFS_LEFT;
		}

		if (lManager->getBlock(logicalPosition+vector2i(-1,0))->isLow())
		{
			faces[cnt++]=BFS_RIGHT;
		}

		return cnt;
	}

	bool CBlock::isInRoom()
	{
		return (roomIndex!=-1);
	}

	GLvoid CBlock::setRoomIndex(GLint roomIndex)
	{
		this->roomIndex = roomIndex;
	}

	GLint CBlock::getRoomIndex()
	{
		return roomIndex;
	}

	GLvoid CBlock::resetLife()
	{
		if (type==CV_BLOCK_TYPE_WALL_ID || type==CV_BLOCK_TYPE_WALL_WITH_TORCH_PLATE_ID)
		{
			life=9.0f;
		}
		else if (type==CV_BLOCK_TYPE_EARTH_ID || type==CV_BLOCK_TYPE_EARTH_WITH_TORCH_PLATE_ID)
		{
			life=1.5f;
		}
		else if (type==CV_BLOCK_TYPE_GOLD_ID)
		{
			life=12.0f;
		}
		else if (type==CV_BLOCK_TYPE_UNCLAIMED_LAND_ID)
		{
			life=0.6f;
		}
		else
		{
			life=3.0f;
		}
	}

	GLfloat CBlock::getLife()
	{
		return life;
	}

	GLvoid CBlock::addLife(GLfloat life)
	{
		this->life+=life;
	}

	GLvoid CBlock::decLife(GLfloat life)
	{
		this->life-=life;
		if(life< 0.0f)
			life = 0.0f;
	}

	GLvoid CBlock::setVisible(BLOCK_FACE_SELECTOR faceSelector, bool visibility)
	{
		visible[faceSelector] = visibility;
	}

	GLvoid CBlock::digBlock()
	{
		if(!CV_GAME_MANAGER->getLevelManager()->isFullBlock(this))
			return;

		setType(CV_BLOCK_TYPE_UNCLAIMED_LAND_ID);
		setMarked(false);
		
		//init and finalize surrounding blocks aswell as this one
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->init();
		init();
		
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->finalize();
		finalize();

		if(CV_GAME_MANAGER->getControlManager()->getViewFrustum()->containsBBOX(getBoundingBox()))
		{
			//since the block is visible, create the digg effect (TODO)
		}
	}

	GLvoid CBlock::claimBlock(GLubyte owner)
	{
		this->owner = owner;
		setType(CV_BLOCK_TYPE_CLAIMED_LAND_ID);
		setTaken(false);
		
		//init and finalize surrounding blocks aswell as this one
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->init();
		init();
		
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->finalize();
		finalize();

		// Add to unfortified list.
		if(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->addUnfortifiedBlock(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1));

		if(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->addUnfortifiedBlock(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1]));

		if(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->addUnfortifiedBlock(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1]));

		if(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->addUnfortifiedBlock(CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1));

		if(CV_GAME_MANAGER->getControlManager()->getViewFrustum()->containsBBOX(getBoundingBox()))
		{
			//since the block is visible, create the claim effect
			CBlockObject *obj = new CBlockObject("CLAIM_EFFECT",this->getRealPosition()+vector3f(CV_BLOCK_WIDTH/2.0f,CV_BLOCK_HEIGHT/4.0f,CV_BLOCK_DEPTH/2.0f), NULL);
			obj->setEffect(CV_GAME_MANAGER->getResourceManager()->getEffect("EFFECTS_CLAIM"));
			if(obj->getEffect())
				obj->getEffect()->getEmittter(0)->setColor(vector3f(1.0f,0.2f,0.6f));
			this->addModel(obj);
		}
	}

	GLvoid CBlock::fortifyBlock(GLubyte owner)
	{
		this->owner = owner;
		setType(CV_BLOCK_TYPE_WALL_ID);
		
		//init and finalize surrounding blocks aswell as this one
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->init();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->init();
		init();
		
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->finalize();
		CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->finalize();
		finalize();

		//Calim corners
		if(((CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1]+1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->getOwner() == owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->isLow()) || (
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1]-1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->getOwner() == owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->isLow())) &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->fortifyBlock(owner);

		if(((CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1]-1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->getOwner() == owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->isLow()) || (
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1]-1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->getOwner()== owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->isLow())) &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->fortifyBlock(owner);

		if(((CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1]+1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->getOwner() == owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->isLow()) || (
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1]-1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->getOwner() == owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]-1)->isLow())) &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->fortifyBlock(owner);

		if(((CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1]+1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->getOwner() == owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]-1,logicalPosition[1])->isLow()) || (
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1]+1)->type == CV_BLOCK_TYPE_WALL_ID &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->getOwner() == owner &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0]+1,logicalPosition[1])->isLow())) &&
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->type == CV_BLOCK_TYPE_EARTH_ID)
			CV_GAME_MANAGER->getLevelManager()->getBlock(logicalPosition[0],logicalPosition[1]+1)->fortifyBlock(owner);

		if(CV_GAME_MANAGER->getControlManager()->getViewFrustum()->containsBBOX(getBoundingBox()))
		{
			//since the block is visible, create the fortify effect (TODO)
		}
	}

	std::vector<GLuint> *CBlock::getDisplayLists()
	{
		return &displayLists;
	}

	std::vector<CBlockObject*> *CBlock::getBlockObjects()
	{
		return &blockObjects;
	}

	GLvoid CBlock::disposeDisplayLists()
	{
		for (std::vector<GLuint>::iterator dsIter = displayLists.begin(); dsIter != displayLists.end(); dsIter++)
		{
			glDeleteLists(*dsIter,1);
		}
		displayLists.clear();
	}

	GLvoid CBlock::disposeBlockObjects(CBlock::OBJECT_DISPOSE_TYPE odt)
	{
		bool dispose = false;

		CBlockObject *bObject = NULL;

		// return to the model pool
		for (std::vector<CBlockObject*>::iterator rmIter = blockObjects.begin(); rmIter != blockObjects.end();)
		{
			bObject = *rmIter;

			dispose = ((odt == ODT_REMOVE_ALL) || (odt == ODT_REMOVE_MARKED && bObject->isMarked()));

			if (dispose)
			{
				CV_GAME_MANAGER->getResourceManager()->returnModel(bObject->getName(), bObject->getModel());	//bug on this line		

				if (bObject->getEffect())
				{
					CV_GAME_MANAGER->getResourceManager()->returnEffect(bObject->getEffectName(), bObject->getEffect());
				}

				delete bObject;

				rmIter = blockObjects.erase(rmIter);
			}
			else
			{
				rmIter++;
			}
			
		}

		if (odt == ODT_REMOVE_ALL)
		{
			blockObjects.clear();
		}
	}

	bool CBlock::isSellable(GLint playerID)
	{
		bool sellable = (isRoom() && this->type != CV_BLOCK_TYPE_HEART_ID && this->type != CV_BLOCK_TYPE_PORTAL_ID);

		if (!sellable)
		{
			CBlockObject *bObject = NULL;

			// might be that we're not trying to sell a room tile but a trap
			for (std::vector<CBlockObject*>::iterator rmIter = blockObjects.begin(); rmIter != blockObjects.end(); rmIter++)
			{
				bObject = *rmIter;

				if (bObject->getClassName()=="TRAP")
				{
					sellable = true;
					break;
				}
			}

		}

		return (sellable && playerID == owner);
	}

	bool CBlock::isBuildable(GLint playerID)
	{
		if (isWater() || isLava())
		{
			return true;
		}

		return (type == CV_BLOCK_TYPE_CLAIMED_LAND_ID && playerID == owner && !isSellable(playerID));
	}

	bool CBlock::isTaken()
	{
		return taken;
	}

	bool CBlock::isMarked()
	{
		return marked;
	}

	GLvoid CBlock::setTaken(bool taken)
	{
		this->taken = taken;
	}

	GLvoid CBlock::setMarked(bool marked)
	{
		this->marked = marked;
	
		if (marked)
			CV_GAME_MANAGER->getLevelManager()->addMarkedBlock(this);
		else
			CV_GAME_MANAGER->getLevelManager()->removeMarkedBlock(this);	

		this->finalize();
	}

	GLvoid CBlock::addModel(string modelName, vector3f position, bool marked, GLfloat rotateY)
	{
		CBlockObject *nObject = new CBlockObject
		(
			modelName,
			position,
			CV_GAME_MANAGER->getResourceManager()->getModel(modelName),
			rotateY
		);

		nObject->setMarked(marked);

		blockObjects.push_back(nObject);
	}

	GLvoid CBlock::addModel(CBlockObject *blockObject)
	{
		blockObjects.push_back(blockObject);
	}

	GLvoid CBlock::removeBlockObject(CBlockObject *blockObject)
	{
		CBlockObject *bObject = NULL;
		for (std::vector<CBlockObject*>::iterator rmIter = blockObjects.begin(); rmIter != blockObjects.end();)
		{
			bObject = *rmIter;

			if (bObject == blockObject)
			{
				CV_GAME_MANAGER->getResourceManager()->returnModel(bObject->getName(), bObject->getModel());			

				if (bObject->getEffect())
				{
					CV_GAME_MANAGER->getResourceManager()->returnEffect(bObject->getEffectName(), bObject->getEffect());
				}

				blockObjects.erase(rmIter);	

				break; // only 1 istance of the same object per block
			}
			else
			{
				rmIter++;
			}
		}
	}
};