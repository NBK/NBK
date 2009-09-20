#include "commons.h"

#include "RoomConstructor.h"
#include "LevelManager.h"

using namespace utils;
using namespace game_objects;
using namespace game_utils;
using namespace game_utils::managers;
using namespace cml;

#define CLAIMED_LAND	11
#define CLAIMED_LAND_TL	111
#define CLAIMED_LAND_TM	112
#define CLAIMED_LAND_TR	113
#define CLAIMED_LAND_ML	114
#define CLAIMED_LAND_MR	115
#define CLAIMED_LAND_BL	116
#define CLAIMED_LAND_BM	117
#define CLAIMED_LAND_BR	118

#define UNCLAIMED_LAND	0
#define UNCLAIMED_LAND0	100
#define UNCLAIMED_LAND1	101
#define UNCLAIMED_LAND2	102
#define UNCLAIMED_LAND3	103
#define UNCLAIMED_LAND4	104

#define PRISON_TL		135
#define PRISON_TM		136
#define PRISON_TR		137
#define PRISON_ML		138
#define PRISON_M		139
#define PRISON_MR		140
#define PRISON_BL		141
#define PRISON_BM		142
#define PRISON_BR		143

#define TEMPLE_BOTTOM		867
#define TEMPLE_SIDE0		914
#define TEMPLE_SIDE1		915

#define TEMPLE_WATER0_0			916
#define TEMPLE_WATER0_1			917
#define TEMPLE_WATER0_2			918
#define TEMPLE_WATER0_3			919
#define TEMPLE_WATER0_4			920
#define TEMPLE_WATER0_5			921
#define TEMPLE_WATER0_6			922
#define TEMPLE_WATER0_7			923
#define TEMPLE_WATER0_8			924
#define TEMPLE_WATER1_0			925
#define TEMPLE_WATER1_1			926
#define TEMPLE_WATER1_2			927
#define TEMPLE_WATER1_3			928
#define TEMPLE_WATER1_4			929
#define TEMPLE_WATER1_5			930
#define TEMPLE_WATER1_6			931
#define TEMPLE_WATER1_7			932
#define TEMPLE_WATER1_8			933
#define TEMPLE_WATER2_0			934
#define TEMPLE_WATER2_1			935
#define TEMPLE_WATER2_2			936
#define TEMPLE_WATER2_3			937
#define TEMPLE_WATER2_4			938
#define TEMPLE_WATER2_5			939
#define TEMPLE_WATER2_6			940
#define TEMPLE_WATER2_7			941
#define TEMPLE_WATER2_8			942
#define TEMPLE_WATER3_0			943
#define TEMPLE_WATER3_1			944
#define TEMPLE_WATER3_2			945
#define TEMPLE_WATER3_3			946
#define TEMPLE_WATER3_4			947
#define TEMPLE_WATER3_5			948
#define TEMPLE_WATER3_6			949
#define TEMPLE_WATER3_7			950
#define TEMPLE_WATER3_8			951

#define TEMPLE_BORDER1_TOP		952
#define TEMPLE_BORDER1_SIDE		953

#define WALL_PAINTING_TEMPLE_9_10_11	864
#define WALL_PAINTING_TEMPLE_COLUMN_1_7	865
#define WALL_PAINTING_TEMPLE_COLUMN_4	866

#define TEMPLE_ANIM_SPEED		0.055f

#define TEMPLE_HEIGHT		1.5f

#define TREASURY_BOTTOM		899

#define TREASURY_COLUMN_0	900
#define TREASURY_COLUMN_1	901
#define TREASURY_COLUMN_2	902
#define TREASURY_COLUMN_3	903
#define TREASURY_COLUMN_TOP 904

#define UNCLAIMED_LAND0		100
#define UNCLAIMED_LAND1		101
#define UNCLAIMED_LAND2		102
#define UNCLAIMED_LAND3		103
#define UNCLAIMED_LAND4		104

#define HATCHERY_COLUMN_0	905
#define HATCHERY_COLUMN_1	906
#define HATCHERY_COLUMN_2	907
#define HATCHERY_COLUMN_3	908
#define HATCHERY_COLUMN_TOP	909

#define HATCHERY_BORDER_SIDE0	910
#define HATCHERY_BORDER_SIDE1	911
#define HATCHERY_BORDER_TOP0	912
#define HATCHERY_BORDER_TOP1	913

#define LAIR_BOTTOM_0	954
#define LAIR_BOTTOM_1	955
#define LAIR_BOTTOM_2	956
#define LAIR_BOTTOM_3	957
#define LAIR_BOTTOM_4	958
#define LAIR_BOTTOM_5	959
#define LAIR_BOTTOM_6	960
#define LAIR_BOTTOM_7	961
#define LAIR_BOTTOM_8	962

#define GRAVEYARD_BOTTOM_0	963
#define GRAVEYARD_BOTTOM_1	964
#define GRAVEYARD_BOTTOM_2	965
#define GRAVEYARD_BOTTOM_3	966
#define GRAVEYARD_BOTTOM_4	967
#define GRAVEYARD_BOTTOM_5	968
#define GRAVEYARD_BOTTOM_6	969
#define GRAVEYARD_BOTTOM_7	970
#define GRAVEYARD_BOTTOM_8	971

#define TRAINING_ROOM_BOTTOM			972
#define WALL_PAINTING_TRAIN_EDGE_1		787
#define WALL_PAINTING_TRAIN_EDGE_4		788
#define WALL_PAINTING_TRAIN_EDGE_7		789
#define WALL_PAINTING_TRAIN_EDGE_10		790
#define WALL_CORNER						700

#define WALL_PAINTING_BARRACKS_COLUMN_0 851
#define WALL_PAINTING_BARRACKS_COLUMN_3 852
#define WALL_PAINTING_BARRACKS_COLUMN_6 853
#define WALL_PAINTING_BARRACKS_COLUMN_9 854
#define BARRACKS_BOTTOM					973
#define BARRACKS_BORDER					974
#define BARRACKS_BORDER_SIDE			975

#define BRIDGE_TOP						976

#define TORTURE_CHAMBER_SPIKE_HOLE		977

#define GUARD_POST_BORDER_SIDE_0	978
#define GUARD_POST_BORDER_SIDE_1	979
#define GUARD_POST_BORDER_SIDE_2	980

#define LIBRARY_BOTTOM_1			831
#define LIBRARY_BOTTOM_2			832

#define LIBRARY_BOOKSHELF_FRONT_0	981
#define LIBRARY_BOOKSHELF_FRONT_1	982
#define LIBRARY_BOOKSHELF_FRONT_2	983
#define LIBRARY_BOOKSHELF_FRONT_3	984
#define LIBRARY_BOOKSHELF_FRONT_4	985
#define LIBRARY_BOOKSHELF_FRONT_5	986
#define LIBRARY_BOOKSHELF_FRONT_6	987
#define LIBRARY_BOOKSHELF_FRONT_7	988
#define LIBRARY_BOOKSHELF_FRONT_8	989
#define LIBRARY_BOOKSHELF_TOP_0		990
#define LIBRARY_BOOKSHELF_TOP_1		991
#define LIBRARY_BOOKSHELF_TOP_2		992
#define LIBRARY_BOOKSHELF_SIDE0		993
#define LIBRARY_BOOKSHELF_SIDE1		994

#define WALL_PAINTING_LIBRARY_COLUMN_0	827
#define WALL_PAINTING_LIBRARY_COLUMN_3	828
#define WALL_PAINTING_LIBRARY_COLUMN_6	829
#define WALL_PAINTING_LIBRARY_COLUMN_9	830

#define WORKSHOP_BOTTOM					1003

#define WALL_PAINTING_WORKSHOP_COLUMN_0	895
#define WALL_PAINTING_WORKSHOP_COLUMN_3 896
#define WALL_PAINTING_WORKSHOP_COLUMN_6 897
#define WALL_PAINTING_WORKSHOP_COLUMN_9 898

#define PORTAL_BOTTOM_OUTER		1004
#define PORTAL_BOTTOM_INNER		1005
#define PORTAL_COLUMN_TOP		1006
#define PORTAL_COLUMN_MIDDLE0	1007
#define PORTAL_COLUMN_MIDDLE1	1008
#define PORTAL_COLUMN_BOTTOM	1009

#define PORTAL_TOP_0			1010
#define PORTAL_TOP_1			1011
#define PORTAL_TOP_2			1012
#define PORTAL_TOP_3			1013
#define PORTAL_TOP_4			1014
#define PORTAL_TOP_5			1015
#define PORTAL_TOP_6			1016
#define PORTAL_TOP_7			1017
#define PORTAL_TOP_8			1018

#define HEART_COLUMN_0			1019
#define HEART_COLUMN_1			1020
#define HEART_COLUMN_2_3_4__0	1021
#define HEART_COLUMN_2_3_4__1	1022
#define HEART_COLUMN_2_3_4__2	1023
#define HEART_PEDESTAL_SIDE_T	1024
#define HEART_PEDESTAL_FLAME_PLACE	1025

namespace game_utils
{
	CRoomConstructor::CRoomConstructor()
	{
		column_location_x=column_location_z=0;
	}

	CRoomConstructor::~CRoomConstructor()
	{
	}

	GLvoid CRoomConstructor::createRoomData(CBlock *block)
	{
		this->targetBlock = block;	

		targetBlock->disposeDisplayLists();
		targetBlock->disposeBlockObjects();

		// simple hack
		BTM_S[0].init();
		ntw = BTM_S[0].getTextureAtlasSubtileSize()/BTM_S[0].getTextureAtlasSize()[0]-BTM_S[0].getPixelWidth()*2.0f;
		nth = BTM_S[0].getTextureAtlasSubtileSize()/BTM_S[0].getTextureAtlasSize()[1]-BTM_S[0].getPixelHeight()*2.0f;
		
		GLint terrain_type = targetBlock->getType();

		CBlockManager *bManager = CV_GAME_MANAGER->getBlockManager();
		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		GLint owner = targetBlock->getOwner();

		GLfloat cw3=CV_BLOCK_WIDTH/3.0f;
		GLfloat cd3=CV_BLOCK_DEPTH/3.0f;
		GLfloat ch4=CV_BLOCK_HEIGHT/4.0f;

		column=isColumnTile();
		border=isEdgeTile4();
		bookshelf=false;
		portal_center=false;
		heart_center=false;
		hear_flame=false;

		createDeformedBottom();

		fillRoomSubtiles(BTM_S);

		for (GLint y=0; y<3; y++)
		{
			for (GLint x=0; x<3; x++)
			{
				BTM_S[y*3+x].points[0] = (deformed_bottom.bottom[y][x]);
				BTM_S[y*3+x].points[1] = (deformed_bottom.bottom[y][x+1]);
				BTM_S[y*3+x].points[2] = (deformed_bottom.bottom[y+1][x+1]);
				BTM_S[y*3+x].points[3] = (deformed_bottom.bottom[y+1][x]);	

				BRD0_S[y*3+x].points[0] = (deformed_border0.bottom[y][x]);
				BRD0_S[y*3+x].points[1] = (deformed_border0.bottom[y][x+1]);
				BRD0_S[y*3+x].points[2] = (deformed_border0.bottom[y+1][x+1]);
				BRD0_S[y*3+x].points[3] = (deformed_border0.bottom[y+1][x]);	

				BRD1_S[y*3+x].points[0] = (deformed_border1.bottom[y][x]);
				BRD1_S[y*3+x].points[1] = (deformed_border1.bottom[y][x+1]);
				BRD1_S[y*3+x].points[2] = (deformed_border1.bottom[y+1][x+1]);
				BRD1_S[y*3+x].points[3] = (deformed_border1.bottom[y+1][x]);
			}
		}

		if (terrain_type==CV_BLOCK_TYPE_TREASURE_ROOM_ID)
		{
			border=false;
			column_height=5;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(TREASURY_COLUMN_0,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(TREASURY_COLUMN_2,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(TREASURY_COLUMN_1,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(TREASURY_COLUMN_2,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(TREASURY_COLUMN_3,0));
			CLM_S[5].create(bManager->getTexturePosInTextureAtlas(TREASURY_COLUMN_TOP,0));
		}
		else if (terrain_type==CV_BLOCK_TYPE_HATCHERY_ID)
		{
			column_height=6;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(HATCHERY_COLUMN_0,0));
			for (GLint i=1; i<5; i++)
			{
				CLM_S[i].create(bManager->getTexturePosInTextureAtlas(HATCHERY_COLUMN_1,0));
			}
			CLM_S[5].create(bManager->getTexturePosInTextureAtlas(HATCHERY_COLUMN_3,0));
			CLM_S[6].create(bManager->getTexturePosInTextureAtlas(HATCHERY_COLUMN_TOP,0));
			
			for (GLint i=0; i<9; i++)
			{
				BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(HATCHERY_BORDER_TOP0+rand()%2,0));
				if (i<3)
				{
					SIDE_BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(HATCHERY_BORDER_SIDE0+rand()%2,0));
				}
			}

			// check which tiles represent border

			GLint edges[9];
			GLint count=getEdgeSubtiles(edges);

			for (GLint i=0; i<9; i++)
			{
				border_skip[i]=true;
			}

			for (GLint i=0; i<count; i++)
			{
				border_skip[edges[i]]=false;
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_TEMPLE_ID)
		{
			// we don't need to draw the temple floor, just the "border"
			column_height=5;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(TEMPLE_BORDER1_TOP,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TEMPLE_COLUMN_4,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TEMPLE_COLUMN_1_7,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(TEMPLE_BORDER1_TOP,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TEMPLE_COLUMN_1_7,0));
			CLM_S[5].create(bManager->getTexturePosInTextureAtlas(TEMPLE_BORDER1_TOP,0));

			for (GLint i=0; i<9; i++)
			{
				BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(TEMPLE_BOTTOM,0));
				BRD1_S[i].create(bManager->getTexturePosInTextureAtlas(TEMPLE_BORDER1_TOP,0));
				if (i<3)
				{
					SIDE_BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(TEMPLE_SIDE0+rand()%2,0));
				}
				SIDE_BRD1_S[i].create(bManager->getTexturePosInTextureAtlas(TEMPLE_BORDER1_SIDE,0));
			}

			border=true;

			temple_animation=true;

			targetBlock->setVisible(CBlock::BFS_BOTTOM,false);

			for (GLint i=-1; i<2; i++)
			{
				for (GLint j=-1; j<2; j++)
				{
					if (i==0 && j==0)
					{
						continue;
					}
					else
					{
						if (!lManager->isSameTypeAndOwner(map_x-j,map_y-i,targetBlock))
						{
							temple_animation=false;
						}
					}
				}
			}

			if (temple_animation)
			{
				GLint temple_animation0[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_0,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_0,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_0,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_0,0)
				};

				GLint temple_animation1[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_1,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_1,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_1,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_1,0)
				};

				GLint temple_animation2[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_2,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_2,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_2,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_2,0)
				};

				GLint temple_animation3[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_3,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_3,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_3,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_3,0)
				};

				GLint temple_animation4[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_4,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_4,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_4,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_4,0)
				};

				GLint temple_animation5[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_5,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_5,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_5,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_5,0)
				};

				GLint temple_animation6[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_6,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_6,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_6,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_6,0)
				};

				GLint temple_animation7[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_7,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_7,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_7,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_7,0)
				};

				GLint temple_animation8[]=
				{
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER0_8,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER1_8,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER2_8,0),
					bManager->getTexturePosInTextureAtlas(TEMPLE_WATER3_8,0)
				};

				GLint *arr[9]={temple_animation0,temple_animation1,temple_animation2,temple_animation3,temple_animation4,temple_animation5,temple_animation6,temple_animation7,temple_animation8};

				for (GLint i=0; i<9; i++)
				{
					BRD0_S[i].create(arr[i],4,false,true);
					BRD0_S[i].setAnimSpeed(TEMPLE_ANIM_SPEED);
				}
			}

			// set the temple border1 type
			if (temple_animation)
			{
				temple_border1_type=TB_NONE;
			}
			else
			{
				if (lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) &&
					lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) &&
					lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock) &&
					lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock))
				{
					temple_border1_type=TB_ALL;
				}
				else if (lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock))
				{
					temple_border1_type=TB_RIGHT;
				}
				else if (lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock))
				{
					temple_border1_type=TB_LEFT;
				}
				else if (lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock))
				{
					temple_border1_type=TB_UP;
				}
				else if (lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) &&
						lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock))
				{
					temple_border1_type=TB_DOWN;
				}
				else if (isColumnTile())
				{
					if (column_location_x==0 && column_location_z==0)
					{
						temple_border1_type=TB_TL;
					}
					else if (column_location_x==2 && column_location_z==0)
					{
						temple_border1_type=TB_TR;
					}
					else if (column_location_x==2 && column_location_z==2)
					{
						temple_border1_type=TB_BR;
					}
					else if (column_location_x==0 && column_location_z==2)
					{
						temple_border1_type=TB_BL;
					}
					else
					{
						temple_border1_type=TB_NONE;
					}
				}
				else
				{
					temple_border1_type=TB_NONE;
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_TRAINING_ROOM_ID)
		{
			border=false;
			column_height=5;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TRAIN_EDGE_1,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TRAIN_EDGE_4,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TRAIN_EDGE_4,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TRAIN_EDGE_7,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_TRAIN_EDGE_10,0));
			CLM_S[5].create(bManager->getTexturePosInTextureAtlas(WALL_CORNER,0));
		}
		else if (terrain_type==CV_BLOCK_TYPE_BARRACKS_ID)
		{
			border=false;
			if (isCenterTile())
			{
				border=true;
				targetBlock->setVisible(CBlock::BFS_BOTTOM,false); // TODO?
			}
			column_height=5;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_BARRACKS_COLUMN_0,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_BARRACKS_COLUMN_3,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_BARRACKS_COLUMN_6,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_BARRACKS_COLUMN_0,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_BARRACKS_COLUMN_0,0));
			CLM_S[5].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_BARRACKS_COLUMN_0,0));

			for (GLint i=0; i<9; i++)
			{
				BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(BARRACKS_BORDER,0));
				if (i<3)
				{
					SIDE_BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(BARRACKS_BORDER_SIDE,0));
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_GUARD_POST_ID)
		{
			border=true;
			targetBlock->setVisible(CBlock::BFS_BOTTOM,false);
			column=false;

			for (GLint i=0; i<9; i++)
			{
				BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(BRIDGE_TOP,0));			
				if (i<3)
				{
					SIDE_BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(GUARD_POST_BORDER_SIDE_0+i,0));
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_LIBRARY_ID)
		{
			border=false;

			column_height=4;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY_COLUMN_0,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY_COLUMN_3,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY_COLUMN_6,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY_COLUMN_9,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(WALL_CORNER,0));

			for (GLint i=0; i<9; i++)
			{
				bookshelf_front[i].create(bManager->getTexturePosInTextureAtlas(LIBRARY_BOOKSHELF_FRONT_0+i,0));			
				bookshelf_back[i].create(bManager->getTexturePosInTextureAtlas(LIBRARY_BOOKSHELF_FRONT_0+i,0));			
				if (i<3)
				{
					bookshelf_left_side[i].create(bManager->getTexturePosInTextureAtlas(LIBRARY_BOOKSHELF_SIDE0+rand()%2,0));
					bookshelf_right_side[i].create(bManager->getTexturePosInTextureAtlas(LIBRARY_BOOKSHELF_SIDE0+rand()%2,0));
					bookshelf_top[i].create(bManager->getTexturePosInTextureAtlas(LIBRARY_BOOKSHELF_TOP_0+i,0));
				}
			}
			bookshelf=isCenterTile();
		}
		else if (terrain_type==CV_BLOCK_TYPE_WORKSHOP_ID)
		{
			border=false;
			column_height=4;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP_COLUMN_0,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP_COLUMN_3,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP_COLUMN_6,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP_COLUMN_9,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
		}
		else if (terrain_type==CV_BLOCK_TYPE_PORTAL_ID)
		{
			column_height=5;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(PORTAL_COLUMN_TOP,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(PORTAL_COLUMN_MIDDLE0,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(PORTAL_COLUMN_MIDDLE0,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(PORTAL_COLUMN_BOTTOM,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
			CLM_S[5].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

			// there's a hole in the middle
			targetBlock->setVisible(CBlock::BFS_BOTTOM,!isCenterTile());
			border=true;

			// do we have a area?
			if (!targetBlock->isFaceVisible(CBlock::BFS_BOTTOM))
			{
				portal_center=true;

				GLint mapping[]={-1,-1,-1,-1,-1,-1,0,1,2,-1,-1,3,4,5,-1,-1,6,7,8};

				GLint portal_animation[]=
				{
					bManager->getTexturePosInTextureAtlas(PORTAL_TOP_4,0),
					bManager->getTexturePosInTextureAtlas(PORTAL_TOP_4,1),
					bManager->getTexturePosInTextureAtlas(PORTAL_TOP_4,2),
					bManager->getTexturePosInTextureAtlas(PORTAL_TOP_4,3),
					bManager->getTexturePosInTextureAtlas(PORTAL_TOP_4,4),
					bManager->getTexturePosInTextureAtlas(PORTAL_TOP_4,5)
				};

				// setup area gfx
				for (GLint i=0; i<25; i++)
				{
					if (i<19 && mapping[i]!=-1)
					{
						if (i==12 && owner==5)
						{
							PORTAL_ROOF_TOP[12].create(portal_animation,5,true,true);
							PORTAL_ROOF_TOP[12].setAnimSpeed(TEMPLE_ANIM_SPEED);

							PORTAL_ROOF_BOTTOM[12].create(portal_animation,5,true,true);
							PORTAL_ROOF_BOTTOM[12].setAnimSpeed(TEMPLE_ANIM_SPEED);
						}
						else
						{
							PORTAL_ROOF_TOP[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_TOP_0+mapping[i],(mapping[i]==4?owner:0)));
							PORTAL_ROOF_BOTTOM[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_TOP_0+mapping[i],(mapping[i]==4?owner:0)));
						}
					}
					else
					{
						PORTAL_ROOF_TOP[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_COLUMN_MIDDLE0,0));
						PORTAL_ROOF_BOTTOM[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_COLUMN_MIDDLE0,0));
					}
					if (i<5)
					{				
						PORTAL_ROOF_SIDE[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_COLUMN_MIDDLE0,0));
					}
				}

				// setup roof area points
				for (GLint y=0; y<5; y++)
				{
					for (GLint x=0; x<5; x++)
					{
						PORTAL_ROOF_TOP[y*5+x].points[0] = (deformed_roof_top.area[y][x]);
						PORTAL_ROOF_TOP[y*5+x].points[1] = (deformed_roof_top.area[y][x+1]);
						PORTAL_ROOF_TOP[y*5+x].points[2] = (deformed_roof_top.area[y+1][x+1]);
						PORTAL_ROOF_TOP[y*5+x].points[3] = (deformed_roof_top.area[y+1][x]);	

						PORTAL_ROOF_BOTTOM[y*5+x].points[0] = (deformed_roof_bottom.area[y][x]);
						PORTAL_ROOF_BOTTOM[y*5+x].points[1] = (deformed_roof_bottom.area[y][x+1]);
						PORTAL_ROOF_BOTTOM[y*5+x].points[2] = (deformed_roof_bottom.area[y+1][x+1]);
						PORTAL_ROOF_BOTTOM[y*5+x].points[3] = (deformed_roof_bottom.area[y+1][x]);	
					}
				}
			}
			else
			{
				for (GLint i=0; i<9; i++)
				{
					BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
					if (i<3)
					{				
						SIDE_BRD0_S[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
					}
				}
			}

			for (GLint i=0; i<9; i++)
			{
				border_skip[i]=true;
			}
			
			if (tile_type==TT_TL)
			{
				border_skip[4]=false;
				border_skip[5]=false;
				border_skip[7]=false;
			}
			else if (tile_type==TT_TR)
			{
				border_skip[3]=false;
				border_skip[4]=false;
				border_skip[7]=false;
			}
			else if (tile_type==TT_BR)
			{
				border_skip[1]=false;
				border_skip[4]=false;
				border_skip[3]=false;
			}
			else if (tile_type==TT_BL)
			{
				border_skip[1]=false;
				border_skip[4]=false;
				border_skip[5]=false;
			}
			else if (tile_type==TT_L || tile_type==TT_R)
			{
				border_skip[1]=false;
				border_skip[4]=false;
				border_skip[7]=false;
			}
			else if (tile_type==TT_T || tile_type==TT_B)
			{
				border_skip[3]=false;
				border_skip[4]=false;
				border_skip[5]=false;
			}
			else if (tile_type==TT_M)
			{
				for (GLint i=0; i<9; i++)
				{
					border_skip[i]=false;
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_HEART_ID)
		{
			GLfloat hpx=cw3/2.0f+cw3;
			GLfloat hpz=cd3/2.0f+cd3;
			column_height=5;
			CLM_S[0].create(bManager->getTexturePosInTextureAtlas(HEART_COLUMN_0,0));
			CLM_S[1].create(bManager->getTexturePosInTextureAtlas(HEART_COLUMN_1,0));
			CLM_S[2].create(bManager->getTexturePosInTextureAtlas(HEART_COLUMN_2_3_4__0+rand()%3,0));
			CLM_S[3].create(bManager->getTexturePosInTextureAtlas(HEART_COLUMN_2_3_4__0+rand()%3,0));
			CLM_S[4].create(bManager->getTexturePosInTextureAtlas(HEART_COLUMN_2_3_4__0+rand()%3,0));
			CLM_S[5].create(bManager->getTexturePosInTextureAtlas(PORTAL_TOP_4,owner));

			// there's a hole in the middle
			targetBlock->setVisible(CBlock::BFS_BOTTOM,!isCenterTile());
			border=false;
			
			for (GLint i=0; i<49; i++)
			{			
				if (i<5)
				{
					HEART_PEDESTAL_SIDE[i].create(bManager->getTexturePosInTextureAtlas(HEART_PEDESTAL_SIDE_T,0));
				}

				if (i<25)
				{
					HEART_PEDESTAL_UPPER_TOP[i].create(bManager->getTexturePosInTextureAtlas(TREASURY_BOTTOM,0));
				}

				if (i==0 || i==6 || i==42 || i==48)
				{
					HEART_PEDESTAL_LOWER_TOP[i].create(bManager->getTexturePosInTextureAtlas(HEART_PEDESTAL_FLAME_PLACE,0));
				}
				else
				{
					HEART_PEDESTAL_LOWER_TOP[i].create(bManager->getTexturePosInTextureAtlas(TREASURY_BOTTOM,0));
				}
			}

			// the upper heart pedestal
			if (!targetBlock->isFaceVisible(CBlock::BFS_BOTTOM))
			{
				heart_center=true;
				border=true;

				// setup upper hard pedestal area points
				for (GLint y=0; y<5; y++)
				{
					for (GLint x=0; x<5; x++)
					{
						HEART_PEDESTAL_UPPER_TOP[y*5+x].points[0] = (deformed_heart_pedestal_upper_top.area[y][x]);
						HEART_PEDESTAL_UPPER_TOP[y*5+x].points[1] = (deformed_heart_pedestal_upper_top.area[y][x+1]);
						HEART_PEDESTAL_UPPER_TOP[y*5+x].points[2] = (deformed_heart_pedestal_upper_top.area[y+1][x+1]);
						HEART_PEDESTAL_UPPER_TOP[y*5+x].points[3] = (deformed_heart_pedestal_upper_top.area[y+1][x]);	

						HEART_PEDESTAL_UPPER_BOTTOM[y*5+x].points[0] = (deformed_heart_pedestal_upper_bottom.area[y][x]);
						HEART_PEDESTAL_UPPER_BOTTOM[y*5+x].points[1] = (deformed_heart_pedestal_upper_bottom.area[y][x+1]);
						HEART_PEDESTAL_UPPER_BOTTOM[y*5+x].points[2] = (deformed_heart_pedestal_upper_bottom.area[y+1][x+1]);
						HEART_PEDESTAL_UPPER_BOTTOM[y*5+x].points[3] = (deformed_heart_pedestal_upper_bottom.area[y+1][x]);	
					}
				}

				// setup upper hard pedestal area points
				for (GLint y=0; y<7; y++)
				{
					for (GLint x=0; x<7; x++)
					{
						HEART_PEDESTAL_LOWER_TOP[y*7+x].points[0] = (deformed_heart_pedestal_lower_top.area[y][x]);
						HEART_PEDESTAL_LOWER_TOP[y*7+x].points[1] = (deformed_heart_pedestal_lower_top.area[y][x+1]);
						HEART_PEDESTAL_LOWER_TOP[y*7+x].points[2] = (deformed_heart_pedestal_lower_top.area[y+1][x+1]);
						HEART_PEDESTAL_LOWER_TOP[y*7+x].points[3] = (deformed_heart_pedestal_lower_top.area[y+1][x]);	

						HEART_PEDESTAL_LOWER_BOTTOM[y*7+x].points[0] = (deformed_heart_pedestal_lower_bottom.area[y][x]);
						HEART_PEDESTAL_LOWER_BOTTOM[y*7+x].points[1] = (deformed_heart_pedestal_lower_bottom.area[y][x+1]);
						HEART_PEDESTAL_LOWER_BOTTOM[y*7+x].points[2] = (deformed_heart_pedestal_lower_bottom.area[y+1][x+1]);
						HEART_PEDESTAL_LOWER_BOTTOM[y*7+x].points[3] = (deformed_heart_pedestal_lower_bottom.area[y+1][x]);	
					}
				}
			}
			else if (isColumnTile())
			{
				hear_flame=true;
				border=true;

				for (GLint y=0; y<3; y++)
				{
					for (GLint x=0; x<3; x++)
					{
						HEART_PEDESTAL_CORNER_TOP[y*3+x].points[0] = (deformed_heart_pedestal_corner_top.area[y][x]);
						HEART_PEDESTAL_CORNER_TOP[y*3+x].points[1] = (deformed_heart_pedestal_corner_top.area[y][x+1]);
						HEART_PEDESTAL_CORNER_TOP[y*3+x].points[2] = (deformed_heart_pedestal_corner_top.area[y+1][x+1]);
						HEART_PEDESTAL_CORNER_TOP[y*3+x].points[3] = (deformed_heart_pedestal_corner_top.area[y+1][x]);

						HEART_PEDESTAL_CORNER_MIDDLE[y*3+x].points[0] = (deformed_heart_pedestal_corner_middle.area[y][x]);
						HEART_PEDESTAL_CORNER_MIDDLE[y*3+x].points[1] = (deformed_heart_pedestal_corner_middle.area[y][x+1]);
						HEART_PEDESTAL_CORNER_MIDDLE[y*3+x].points[2] = (deformed_heart_pedestal_corner_middle.area[y+1][x+1]);
						HEART_PEDESTAL_CORNER_MIDDLE[y*3+x].points[3] = (deformed_heart_pedestal_corner_middle.area[y+1][x]);
					}
				}

				for (GLint i=0; i<9; i++)
				{
					HEART_PEDESTAL_CORNER_TOP[i].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
					HEART_PEDESTAL_CORNER_MIDDLE[i].create(bManager->getTexturePosInTextureAtlas(HEART_COLUMN_2_3_4__0+rand()%3,0));
					border_skip[i]=false;
				}
				
				if (tile_type==TT_TL)
				{
					border_skip[0]=true;
					border_skip[1]=true;
					border_skip[3]=true;
					border_skip[4]=true;
				}
				else if (tile_type==TT_TR)
				{
					border_skip[1]=true;
					border_skip[2]=true;
					border_skip[4]=true;
					border_skip[5]=true;
				}
				else if (tile_type==TT_BR)
				{
					border_skip[4]=true;
					border_skip[5]=true;
					border_skip[7]=true;
					border_skip[8]=true;
				}
				else if (tile_type==TT_BL)
				{
					border_skip[3]=true;
					border_skip[4]=true;
					border_skip[6]=true;
					border_skip[7]=true;
				}
				else
				{
					for (GLint i=0; i<9; i++)
					{
						border_skip[i]=true;
					}
				}
			}
		}
		else
		{
			border=column=false;
			column_height=0;
		}
		if (column)
		{
			// nr. points = column_height+1
			for (GLint yc=0; yc<column_height+1; yc++)
			{
				for (GLint zc=0; zc<2; zc++)
				{
					for (GLint xc=0; xc<2; xc++)
					{	
						deformed_column.column[zc][yc][xc] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,column_location_x+xc,column_height+1-yc+((terrain_type==CV_BLOCK_TYPE_TEMPLE_ID || terrain_type==CV_BLOCK_TYPE_HEART_ID)?2:((terrain_type==CV_BLOCK_TYPE_HATCHERY_ID)?-1:0)),zc+column_location_z));
					}
				}
			}
		}

		// we lower the hatchery one level
		if (terrain_type==CV_BLOCK_TYPE_HATCHERY_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				for (GLint j=0; j<4; j++)
				{
					BRD0_S[i].points[j] = (BTM_S[i].points[j]);
					BTM_S[i].points[j] += vector3f(0.0f,-ch4,0.0f);
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_LIBRARY_ID && bookshelf)
		{
			GLint arr_diff_x[]={0,1,1,0};
			GLint arr_diff_y[]={0,0,1,1};
			for (GLint i=0; i<9; i++)
			{
				for (GLint j=0; j<4; j++)
				{
					bookshelf_front[i].points[j] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,i%3+arr_diff_x[j],i/3+1+arr_diff_y[j],2));
					bookshelf_back[i].points[j] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,i%3+arr_diff_x[j],i/3+1+arr_diff_y[j],1));

					if (i<3)
					{
						bookshelf_top[i].points[j] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,i%3+arr_diff_x[j],4,1+arr_diff_y[j]));
						bookshelf_left_side[i].points[j] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,0,i+1+arr_diff_y[j],1+arr_diff_x[j]));
						bookshelf_right_side[i].points[j] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,3,i+1+arr_diff_y[j],1+arr_diff_x[j]));
					}
				}
			}

			GLuint bookshelf_display_list=glGenLists(1);
			glNewList(bookshelf_display_list,GL_COMPILE);

			for (GLint subtile_pos=0; subtile_pos<9; subtile_pos++)
			{
				// F
				glTexCoord2f(bookshelf_front[subtile_pos].getTextureAtlasPosition()[0],bookshelf_front[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&bookshelf_front[subtile_pos].points[3][0]);

				glTexCoord2f(bookshelf_front[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_front[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&bookshelf_front[subtile_pos].points[2][0]);

				glTexCoord2f(bookshelf_front[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_front[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&bookshelf_front[subtile_pos].points[1][0]);

				glTexCoord2f(bookshelf_front[subtile_pos].getTextureAtlasPosition()[0],bookshelf_front[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&bookshelf_front[subtile_pos].points[0][0]);

				// B
				glTexCoord2f(bookshelf_back[subtile_pos].getTextureAtlasPosition()[0],bookshelf_back[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&bookshelf_back[subtile_pos].points[3][0]);

				glTexCoord2f(bookshelf_back[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_back[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&bookshelf_back[subtile_pos].points[2][0]);

				glTexCoord2f(bookshelf_back[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_back[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&bookshelf_back[subtile_pos].points[1][0]);

				glTexCoord2f(bookshelf_back[subtile_pos].getTextureAtlasPosition()[0],bookshelf_back[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&bookshelf_back[subtile_pos].points[0][0]);

				if (subtile_pos<3)
				{
					// L
					glTexCoord2f(bookshelf_top[subtile_pos].getTextureAtlasPosition()[0],bookshelf_top[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&bookshelf_top[subtile_pos].points[0][0]);

					glTexCoord2f(bookshelf_top[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_top[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&bookshelf_top[subtile_pos].points[1][0]);

					glTexCoord2f(bookshelf_top[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_top[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&bookshelf_top[subtile_pos].points[2][0]);

					glTexCoord2f(bookshelf_top[subtile_pos].getTextureAtlasPosition()[0],bookshelf_top[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&bookshelf_top[subtile_pos].points[3][0]);

					// L
					glTexCoord2f(bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[0],bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&bookshelf_left_side[subtile_pos].points[0][0]);

					glTexCoord2f(bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&bookshelf_left_side[subtile_pos].points[1][0]);

					glTexCoord2f(bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&bookshelf_left_side[subtile_pos].points[2][0]);

					glTexCoord2f(bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[0],bookshelf_left_side[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&bookshelf_left_side[subtile_pos].points[3][0]);

					// L
					glTexCoord2f(bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[0],bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&bookshelf_right_side[subtile_pos].points[0][0]);

					glTexCoord2f(bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&bookshelf_right_side[subtile_pos].points[1][0]);

					glTexCoord2f(bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[0]+ntw,bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&bookshelf_right_side[subtile_pos].points[2][0]);

					glTexCoord2f(bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[0],bookshelf_right_side[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&bookshelf_right_side[subtile_pos].points[3][0]);

				}
			}
			glEndList();

			targetBlock->getDisplayLists()->push_back(bookshelf_display_list);
		}

		if (column)
		{
			initColumn();
		}

		if (border)
		{
			initBorder();
		}

		addTileItems();

		if (targetBlock->isFaceVisible(CBlock::BFS_BOTTOM) || targetBlock->getType()==CV_BLOCK_TYPE_TEMPLE_ID)
		{
			GLuint floor_display_list=glGenLists(1);
			glNewList(floor_display_list,GL_COMPILE);

			for (GLint subtile_pos=0; subtile_pos<9; subtile_pos++)
			{
				glTexCoord2f(BTM_S[subtile_pos].getTextureAtlasPosition()[0],BTM_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BTM_S[subtile_pos].points[0][0]);

				glTexCoord2f(BTM_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BTM_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BTM_S[subtile_pos].points[1][0]);

				glTexCoord2f(BTM_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BTM_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

				glTexCoord2f(BTM_S[subtile_pos].getTextureAtlasPosition()[0],BTM_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[3][0]);
			}

			glEndList();

			targetBlock->getDisplayLists()->push_back(floor_display_list);
		}

		if (portal_center)
		{
			GLuint portal_display_list=glGenLists(1);
			glNewList(portal_display_list,GL_COMPILE);

			for (GLint subtile_pos=0; subtile_pos<25; subtile_pos++)
			{
				// T
				glTexCoord2f(PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[0],PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[0][0]);

				glTexCoord2f(PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[1][0]);

				glTexCoord2f(PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[2][0]);

				glTexCoord2f(PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[0],PORTAL_ROOF_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[3][0]);

				// B
				glTexCoord2f(PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[0],PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[0][0]);

				glTexCoord2f(PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[1][0]);

				glTexCoord2f(PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[2][0]);

				glTexCoord2f(PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[0],PORTAL_ROOF_BOTTOM[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[3][0]);

				// L
				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[0][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[3][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[3][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[0][0]);

				// R
				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[1][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[2][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[2][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[1][0]);

				// F
				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[3][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[2][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[2][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[3][0]);

				// B
				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[0][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]);
				glVertex3fv(&PORTAL_ROOF_TOP[subtile_pos].points[1][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0]+ntw,PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[1][0]);

				glTexCoord2f(PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[0],PORTAL_ROOF_SIDE[subtile_pos/5].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&PORTAL_ROOF_BOTTOM[subtile_pos].points[0][0]);
			}

			glEndList();

			targetBlock->getDisplayLists()->push_back(portal_display_list);
		}

		if (terrain_type==CV_BLOCK_TYPE_TEMPLE_ID)
		{
			GLuint temple_display_list=glGenLists(1);
			glNewList(temple_display_list,GL_COMPILE);

			for (GLint subtile_pos=0; subtile_pos<9; subtile_pos++)
			{
				// T
				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0],BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0],BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);
			}

			glEndList();

			targetBlock->getDisplayLists()->push_back(temple_display_list);
		}
	}

	GLint CRoomConstructor::getEdgeSubtiles(GLfloat xps[8], GLfloat yps[8])
	{
		GLint array_pos=0;

		/*
				_____________
				|	|	|	| ly2
				|___|___|___|
				|	|	|	| ly1
				|___|___|___|
				|	|	|	| ly0
				|___|___|___|
				lx0 lx1 lx2
		*/

		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		GLfloat x = targetBlock->getRealPosition()[0];
		GLfloat z = targetBlock->getRealPosition()[2];

		GLfloat lx0=CV_BLOCK_WIDTH/3.0f/2.0f;
		GLfloat lx1=(CV_BLOCK_WIDTH/3.0f)+CV_BLOCK_WIDTH/3.0f/2.0f;
		GLfloat lx2=2.0f*(CV_BLOCK_WIDTH/3.0f)+CV_BLOCK_WIDTH/3.0f/2.0f;

		GLfloat ly0=2.0f*(CV_BLOCK_DEPTH/3.0f)+CV_BLOCK_DEPTH/3.0f/2.0f;
		GLfloat ly1=(CV_BLOCK_DEPTH/3.0f)+CV_BLOCK_DEPTH/3.0f/2.0f;
		GLfloat ly2=CV_BLOCK_DEPTH/3.0f/2.0f;	

		// return the left bottom subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y+1,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx0;
			yps[array_pos++]=z+ly0;
		}

		// return the left middle subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx0;
			yps[array_pos++]=z+ly1;
		}

		// return the left top subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y-1,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx0;
			yps[array_pos++]=z+ly2;
		}

		// return the top middle subtile
		if (lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx1;
			yps[array_pos++]=z+ly2;
		}

		// return the right top subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y-1,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx2;
			yps[array_pos++]=z+ly2;
		}

		// return the right middle subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx2;
			yps[array_pos++]=z+ly1;
		}

		// return the right bottom subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y+1,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx2;
			yps[array_pos++]=z+ly0;
		}

		// return the bottom middle subtile
		if (lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			xps[array_pos]=x+lx1;
			yps[array_pos++]=z+ly0;
		}

		return array_pos;
	}

	GLint CRoomConstructor::getEdgeSubtiles(GLint subtiles[])
	{
		GLint array_pos=0;

		/*
				_____________
				|	|	|	| ly2
				|___|___|___|
				|	|	|	| ly1
				|___|___|___|
				|	|	|	| ly0
				|___|___|___|
				lx0 lx1 lx2
		*/
		
		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		// return the left bottom subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y+1,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=6;
		}

		// return the left middle subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=3;
		}

		// return the left top subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y-1,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=0;
		}

		// return the top middle subtile
		if (lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=1;
		}

		// return the right top subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y-1,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=2;
		}

		// return the right middle subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=5;
		}

		// return the right bottom subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x) ||
			lManager->getBlockTypeOld(map_y+1,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=8;
		}

		// return the bottom middle subtile
		if (lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=7;
		}

		return array_pos;
	}

	GLint CRoomConstructor::getEdgeSubtiles4(GLint subtiles[])
	{
		GLint array_pos=0;

		/*
				_____________
				|	|	|	| ly2
				|___|___|___|
				|	|	|	| ly1
				|___|___|___|
				|	|	|	| ly0
				|___|___|___|
				lx0 lx1 lx2
		*/

		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		// return the left bottom subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=6;
		}

		// return the left middle subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=3;
		}

		// return the left top subtile
		if (lManager->getBlockTypeOld(map_y,map_x-1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=0;
		}

		// return the top middle subtile
		if (lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=1;
		}

		// return the right top subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y-1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=2;
		}

		// return the right middle subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=5;
		}

		// return the right bottom subtile
		if (lManager->getBlockTypeOld(map_y,map_x+1)!=lManager->getBlockTypeOld(map_y,map_x) || 
			lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=8;
		}

		// return the bottom middle subtile
		if (lManager->getBlockTypeOld(map_y+1,map_x)!=lManager->getBlockTypeOld(map_y,map_x))
		{
			subtiles[array_pos++]=7;
		}

		return array_pos;
	}

	GLint CRoomConstructor::getEdgeSubtilesWhereNoFullBlock(GLfloat xps[8], GLfloat yps[8], GLint subtiles[8])
	{
		GLint array_pos=0;

		/*
				_____________
				|	|	|	| ly2
				|___|___|___|
				|	|	|	| ly1
				|___|___|___|
				|	|	|	| ly0
				|___|___|___|
				lx0 lx1 lx2
		*/

		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		GLfloat x = targetBlock->getRealPosition()[0];
		GLfloat z = targetBlock->getRealPosition()[2];

		GLfloat lx0=CV_BLOCK_WIDTH/3.0f/2.0f;
		GLfloat lx1=(CV_BLOCK_WIDTH/3.0f)+CV_BLOCK_WIDTH/3.0f/2.0f;
		GLfloat lx2=2.0f*(CV_BLOCK_WIDTH/3.0f)+CV_BLOCK_WIDTH/3.0f/2.0f;

		GLfloat ly0=2.0f*(CV_BLOCK_DEPTH/3.0f)+CV_BLOCK_DEPTH/3.0f/2.0f;
		GLfloat ly1=(CV_BLOCK_DEPTH/3.0f)+CV_BLOCK_DEPTH/3.0f/2.0f;
		GLfloat ly2=CV_BLOCK_DEPTH/3.0f/2.0f;	


		GLint block_type=lManager->getBlockTypeOld(map_y,map_x);

		// return the left bottom subtile
		if ((!lManager->isFullBlock(lManager->getBlockOld(map_y,map_x-1)) && !lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock)) || 
			(!lManager->isFullBlock(lManager->getBlockOld(map_y+1,map_x))&& !lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock))  ||
			(!lManager->isFullBlock(lManager->getBlockOld(map_y+1,map_x-1)) && !lManager->isFullBlock(lManager->getBlockOld(map_y+1,map_x)) && !lManager->isFullBlock(lManager->getBlockOld(map_y,map_x-1)) && !lManager->isSameTypeAndOwner(map_x-1,map_y+1,targetBlock)))
		{
			if (lManager->getBlockTypeOld(map_y,map_x-1)!=block_type || 
				lManager->getBlockTypeOld(map_y+1,map_x)!=block_type ||
				lManager->getBlockTypeOld(map_y+1,map_x-1)!=block_type)
			{
				xps[array_pos]=x+lx0;
				yps[array_pos]=z+ly0;
				subtiles[array_pos++]=6;
			}
		}

		// return the left middle subtile
		if (!lManager->isFullBlock(lManager->getBlockOld(map_y,map_x-1)))
		{
			if (lManager->getBlockTypeOld(map_y,map_x-1)!=block_type)
			{
				xps[array_pos]=x+lx0;
				yps[array_pos]=z+ly1;
				subtiles[array_pos++]=3;
			}	
		}

		// return the left top subtile
		if ((!lManager->isFullBlock(lManager->getBlockOld(map_y,map_x-1)) && !lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock)) || 
			(!lManager->isFullBlock(lManager->getBlockOld(map_y-1,map_x)) && !lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock)) ||
			(!lManager->isFullBlock(lManager->getBlockOld(map_y-1,map_x-1)) && !lManager->isFullBlock(lManager->getBlockOld(map_y-1,map_x)) && !lManager->isFullBlock(lManager->getBlockOld(map_y,map_x-1)) && !lManager->isSameTypeAndOwner(map_x-1,map_y-1,targetBlock)))
		{
			if (lManager->getBlockTypeOld(map_y,map_x-1)!=block_type || 
				lManager->getBlockTypeOld(map_y-1,map_x)!=block_type ||
				lManager->getBlockTypeOld(map_y-1,map_x-1)!=block_type)
			{
				xps[array_pos]=x+lx0;
				yps[array_pos]=z+ly2;
				subtiles[array_pos++]=0;
			}
		}

		// return the top middle subtile
		if (!lManager->isFullBlock(lManager->getBlockOld(map_y-1,map_x)))
		{
			if (lManager->getBlockTypeOld(map_y-1,map_x)!=block_type)
			{
				xps[array_pos]=x+lx1;
				yps[array_pos]=z+ly2;
				subtiles[array_pos++]=1;
			}
		}

		// return the right top subtile
		if ((!lManager->isFullBlock(lManager->getBlockOld(map_y,map_x+1)) && !lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock)) || 
			(!lManager->isFullBlock(lManager->getBlockOld(map_y-1,map_x)) && !lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock)) ||
			(!lManager->isFullBlock(lManager->getBlockOld(map_y-1,map_x+1)) && !lManager->isFullBlock(lManager->getBlockOld(map_y-1,map_x)) && !lManager->isFullBlock(lManager->getBlockOld(map_y,map_x+1)) && !lManager->isSameTypeAndOwner(map_x+1,map_y-1,targetBlock)))
		{
			if (lManager->getBlockTypeOld(map_y,map_x+1)!=block_type || 
				lManager->getBlockTypeOld(map_y-1,map_x)!=block_type ||
				lManager->getBlockTypeOld(map_y-1,map_x+1)!=block_type)
			{
				xps[array_pos]=x+lx2;
				yps[array_pos]=z+ly2;
				subtiles[array_pos++]=2;
			}
		}

		// return the right middle subtile
		if (!lManager->isFullBlock(lManager->getBlockOld(map_y,map_x+1)))
		{
			if (lManager->getBlockTypeOld(map_y,map_x+1)!=block_type)
			{
				xps[array_pos]=x+lx2;
				yps[array_pos]=z+ly1;
				subtiles[array_pos++]=5;
			}
		}

		// return the right bottom subtile
		if ((!lManager->isFullBlock(lManager->getBlockOld(map_y,map_x+1)) && !lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock)) || 
			(!lManager->isFullBlock(lManager->getBlockOld(map_y+1,map_x))&& !lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock)) ||
			(!lManager->isFullBlock(lManager->getBlockOld(map_y+1,map_x+1)) && !lManager->isFullBlock(lManager->getBlockOld(map_y+1,map_x)) && !lManager->isFullBlock(lManager->getBlockOld(map_y,map_x+1)) && !lManager->isSameTypeAndOwner(map_x+1,map_y+1,targetBlock)))
		{
			if (lManager->getBlockTypeOld(map_y,map_x+1)!=block_type || 
				lManager->getBlockTypeOld(map_y+1,map_x)!=block_type ||
				lManager->getBlockTypeOld(map_y+1,map_x+1)!=block_type)
			{
				xps[array_pos]=x+lx2;
				yps[array_pos]=z+ly0;
				subtiles[array_pos++]=8;
			}
		}

		// return the bottom middle subtile
		if (!lManager->isFullBlock(lManager->getBlockOld(map_y+1,map_x)))
		{
			if (lManager->getBlockTypeOld(map_y+1,map_x)!=block_type)
			{
				xps[array_pos]=x+lx1;
				yps[array_pos]=z+ly0;
				subtiles[array_pos++]=7;
			}
		}

		return array_pos;
	}

	bool CRoomConstructor::isColumnTile()
	{
		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		if ((lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) && lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock)) &&
			(!lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) && !lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock)))
		{
			column_location_x=2;
			column_location_z=0;
			return true;
		}

		if ((lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) && lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock)) &&
			(!lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) && !lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock)))
		{
			column_location_x=0;
			column_location_z=0;
			return true;
		}

		if ((lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) && lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock)) &&
			(!lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) && !lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock)))
		{
			column_location_x=0;
			column_location_z=2;
			return true;
		}

		if ((lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) && lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock)) &&
			(!lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) && !lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock)))
		{
			column_location_x=2;
			column_location_z=2;
			return true;
		}

		return false;
	}

	bool CRoomConstructor::isEdgeTile4()
	{
		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		if (!lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) || !lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock) ||
			!lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) || !lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool CRoomConstructor::isEdgeTile8()
	{
		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		if (!lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) || !lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock) ||
			!lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) || !lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock) ||
			!lManager->isSameTypeAndOwner(map_x-1,map_y+1,targetBlock) || !lManager->isSameTypeAndOwner(map_x-1,map_y-1,targetBlock) ||
			!lManager->isSameTypeAndOwner(map_x+1,map_y+1,targetBlock) || !lManager->isSameTypeAndOwner(map_x+1,map_y-1,targetBlock))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool CRoomConstructor::isCenterTile()
	{
		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		if (lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock) && lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock) &&
			lManager->isSameTypeAndOwner(map_x+1,map_y+1,targetBlock) && lManager->isSameTypeAndOwner(map_x+1,map_y-1,targetBlock) &&
			lManager->isSameTypeAndOwner(map_x-1,map_y+1,targetBlock) && lManager->isSameTypeAndOwner(map_x-1,map_y-1,targetBlock) &&
			lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock) && lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	GLvoid CRoomConstructor::createDeformedBottom()
	{
		CBlockManager *bManager = CV_GAME_MANAGER->getBlockManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		GLint terrain_type = targetBlock->getType();

		// clear lists just in case
		//targetBlock->getDisplayLists()->clear();
		//targetBlock->getBlockObjects()->clear();

		for (GLint yc=0; yc<4; yc++)
		{
			for (GLint xc=0; xc<4; xc++)
			{			
				deformed_bottom.bottom[yc][xc] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,1,yc));						
				deformed_border0.bottom[yc][xc] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,2,yc));
				deformed_border1.bottom[yc][xc] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,3,yc));
			}
		}

		// if we have the portal we create the portal area
		if (terrain_type==CV_BLOCK_TYPE_PORTAL_ID && isCenterTile())
		{
			for (GLint yc=-1; yc<5; yc++)
			{
				for (GLint xc=-1; xc<5; xc++)
				{			
					deformed_roof_bottom.area[yc+1][xc+1] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,6,yc));
					deformed_roof_top.area[yc+1][xc+1] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,7,yc));
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_HEART_ID)
		{
			if (isCenterTile())
			{
				for (GLint yc=-1; yc<5; yc++)
				{
					for (GLint xc=-1; xc<5; xc++)
					{	
						deformed_heart_pedestal_upper_top.area[yc+1][xc+1] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,3,yc));
						deformed_heart_pedestal_upper_bottom.area[yc+1][xc+1] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,2,yc));
					}
				}

				for (GLint yc=-2; yc<6; yc++)
				{
					for (GLint xc=-2; xc<6; xc++)
					{	
						deformed_heart_pedestal_lower_top.area[yc+2][xc+2] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,2,yc));
						deformed_heart_pedestal_lower_bottom.area[yc+2][xc+2] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,1,yc));
					}
				}
			}
			else if (isColumnTile())
			{
				for (GLint yc=0; yc<4; yc++)
				{
					for (GLint xc=0; xc<4; xc++)
					{	
						deformed_heart_pedestal_corner_top.area[yc][xc] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,3,yc));
						deformed_heart_pedestal_corner_middle.area[yc][xc] = (bManager->getGlobalDeformedMapPoint(map_x,map_y,xc,2,yc));
					}
				}
			}
		}
	}

	GLvoid CRoomConstructor::fillRoomSubtiles(CSubtileData SUB_DATA[])
	{
		CBlockManager *bManager = CV_GAME_MANAGER->getBlockManager();
		CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

		GLint map_x = targetBlock->getLogicalPosition()[0];
		GLint map_y = targetBlock->getLogicalPosition()[1];

		GLint terrain_type = targetBlock->getType();		

		if (terrain_type==CV_BLOCK_TYPE_LIBRARY_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(rand()%2+LIBRARY_BOTTOM_1,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_TEMPLE_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(TEMPLE_BOTTOM,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_TREASURE_ROOM_ID || terrain_type==CV_BLOCK_TYPE_HEART_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(TREASURY_BOTTOM,0));
			}

			if (lManager->isSameTypeAndOwner(map_x+2,map_y+2,targetBlock))
			{
				tile_type=TT_TL;
			}
			else if (lManager->isSameTypeAndOwner(map_x-2,map_y+2,targetBlock))
			{
				tile_type=TT_TR;
			}
			else if (lManager->isSameTypeAndOwner(map_x-2,map_y-2,targetBlock))
			{
				tile_type=TT_BR;
			}
			else if (lManager->isSameTypeAndOwner(map_x+2,map_y-2,targetBlock))
			{
				tile_type=TT_BL;
			}
			else
			{
				tile_type=TT_M;
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_HATCHERY_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(UNCLAIMED_LAND0+rand()%5,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_LAIR_ID)
		{
			GLint subtiles[9];
			byte set[]={0,0,0,0,0,0,0,0,0};

			GLint count = getEdgeSubtiles(subtiles);

			GLint ls[]={LAIR_BOTTOM_0,LAIR_BOTTOM_1,LAIR_BOTTOM_2,
						LAIR_BOTTOM_3,LAIR_BOTTOM_4,LAIR_BOTTOM_5,
						LAIR_BOTTOM_6,LAIR_BOTTOM_7,LAIR_BOTTOM_8};

			for (GLint i=0; i<count; i++)
			{
				SUB_DATA[subtiles[i]].create(bManager->getTexturePosInTextureAtlas(ls[subtiles[i]],0));
				set[subtiles[i]]=1;
			}

			for (GLint i=0; i<9; i++)
			{
				if (set[i]==0)
				{
					SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(LAIR_BOTTOM_4,0));
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_GRAVEYARD_ID)
		{
			GLint subtiles[9];
			byte set[]={0,0,0,0,0,0,0,0,0};

			GLint count = getEdgeSubtiles(subtiles);

			GLint ls[]={GRAVEYARD_BOTTOM_0,GRAVEYARD_BOTTOM_1,GRAVEYARD_BOTTOM_2,
						GRAVEYARD_BOTTOM_3,GRAVEYARD_BOTTOM_4,GRAVEYARD_BOTTOM_5,
						GRAVEYARD_BOTTOM_6,GRAVEYARD_BOTTOM_7,GRAVEYARD_BOTTOM_8};

			for (GLint i=0; i<count; i++)
			{
				SUB_DATA[subtiles[i]].create(bManager->getTexturePosInTextureAtlas(ls[subtiles[i]],0));
				set[subtiles[i]]=1;
			}

			for (GLint i=0; i<9; i++)
			{
				if (set[i]==0)
				{
					SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(GRAVEYARD_BOTTOM_4,0));
				}
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_TRAINING_ROOM_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(TRAINING_ROOM_BOTTOM,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_BARRACKS_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(BARRACKS_BOTTOM,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_BRIDGE_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(BRIDGE_TOP,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_PRISON_ID || terrain_type==CV_BLOCK_TYPE_TORTURE_CHAMBER_ID)
		{
			GLint subtiles[8];
			byte set[]={0,0,0,0,0,0,0,0,0};

			// get the edge subtiles and correct if necesary
			GLint count = getEdgeSubtiles4(subtiles);

			GLint prison_tile[]=
			{
				PRISON_TL,PRISON_TM,PRISON_TR,
				PRISON_ML,PRISON_M,PRISON_MR,
				PRISON_BL,PRISON_BM,PRISON_BR
			};

			for (GLint i=0; i<count; i++)
			{
				if ((subtiles[i]==0 || subtiles[i]==6) && lManager->isSameTypeAndOwner(map_x-1,map_y,targetBlock))
				{
					SUB_DATA[subtiles[i]].create(bManager->getTexturePosInTextureAtlas(subtiles[i]==0?PRISON_TM:PRISON_BM,0));
				}
				else if ((subtiles[i]==2 || subtiles[i]==8) && lManager->isSameTypeAndOwner(map_x+1,map_y,targetBlock))
				{
					SUB_DATA[subtiles[i]].create(bManager->getTexturePosInTextureAtlas(subtiles[i]==2?PRISON_TM:PRISON_BM,0));
				}
				else if ((subtiles[i]==0 || subtiles[i]==2) && lManager->isSameTypeAndOwner(map_x,map_y-1,targetBlock))
				{
					SUB_DATA[subtiles[i]].create(bManager->getTexturePosInTextureAtlas(subtiles[i]==0?PRISON_ML:PRISON_MR,0));
				}
				else if ((subtiles[i]==6 || subtiles[i]==8) && lManager->isSameTypeAndOwner(map_x,map_y+1,targetBlock))
				{
					SUB_DATA[subtiles[i]].create(bManager->getTexturePosInTextureAtlas(subtiles[i]==6?PRISON_ML:PRISON_MR,0));
				}
				else
				{
					SUB_DATA[subtiles[i]].create(bManager->getTexturePosInTextureAtlas(prison_tile[subtiles[i]],0));
				}
				set[subtiles[i]]=1;
			}

			for (GLint i=0; i<9; i++)
			{
				if (set[i]==0)
				{
					SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(PRISON_M,0));
				}
			}

			if (terrain_type==CV_BLOCK_TYPE_TORTURE_CHAMBER_ID && isEdgeTile8())
			{
				SUB_DATA[4].create(bManager->getTexturePosInTextureAtlas(TORTURE_CHAMBER_SPIKE_HOLE,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_WORKSHOP_ID)
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_PORTAL_ID)
		{
			// TL
			if (lManager->isSameTypeAndOwner(map_x+2,map_y+2,targetBlock))
			{
				SUB_DATA[0].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[1].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[2].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

				SUB_DATA[3].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[4].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[5].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));

				SUB_DATA[6].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[7].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[8].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));

				tile_type=TT_TL;
			}
			// TR
			else if (lManager->isSameTypeAndOwner(map_x-2,map_y+2,targetBlock))
			{
				SUB_DATA[0].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[1].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[2].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

				SUB_DATA[3].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[4].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[5].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

				SUB_DATA[6].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[7].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[8].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

				tile_type=TT_TR;
			}
			// BR
			else if (lManager->isSameTypeAndOwner(map_x-2,map_y-2,targetBlock))
			{
				SUB_DATA[0].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[1].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[2].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

				SUB_DATA[3].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[4].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[5].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

				SUB_DATA[6].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[7].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[8].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				tile_type=TT_BR;
			}
			// BL
			else if (lManager->isSameTypeAndOwner(map_x+2,map_y-2,targetBlock))
			{
				SUB_DATA[0].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[1].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[2].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));

				SUB_DATA[3].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[4].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
				SUB_DATA[5].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));

				SUB_DATA[6].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[7].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				SUB_DATA[8].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));

				tile_type=TT_BL;
			}
			// L
			else if (lManager->isSameTypeAndOwner(map_x+2,map_y,targetBlock))
			{
				for (GLint i=0; i<9; i++)
				{
					if (i%3==0)
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
					}
					else
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
					}
				}
				tile_type=TT_L;
			}
			// R
			else if (lManager->isSameTypeAndOwner(map_x-2,map_y,targetBlock))
			{
				for (GLint i=0; i<9; i++)
				{
					if ((i-2)%3==0)
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
					}
					else 
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
					}
				}
				tile_type=TT_R;
			}
			// T
			else if (lManager->isSameTypeAndOwner(map_x,map_y+2,targetBlock))
			{
				for (GLint i=0; i<9; i++)
				{
					if (i<3)
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
					}
					else
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
					}
				}

				tile_type=TT_T;
			}
			// B
			else if (lManager->isSameTypeAndOwner(map_x,map_y-2,targetBlock))
			{
				for (GLint i=0; i<9; i++)
				{
					if (i>5)
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
					}
					else
					{
						SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(PORTAL_BOTTOM_INNER,0));
					}
				}
				tile_type=TT_B;
			}
			else
			{
				for (GLint i=0; i<9; i++)
				{
					SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(WORKSHOP_BOTTOM,0));
				}
				tile_type=TT_M;
			}
		}
		else if (targetBlock->isRoom())
		{
			for (GLint i=0; i<9; i++)
			{
				SUB_DATA[i].create(bManager->getTexturePosInTextureAtlas(PRISON_M,0));
			}
		}
	}

	GLvoid CRoomConstructor::initColumn()
	{		
		GLuint column_display_list=glGenLists(1);
		glNewList(column_display_list,GL_COMPILE);

		for (GLint yc=0; yc<column_height; yc++)
		{	
			// F
			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[1][yc][0][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[1][yc][1][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[1][yc+1][1][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[1][yc+1][0][0]);

			// B
			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[0][yc][0][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[0][yc][1][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[0][yc+1][1][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[0][yc+1][0][0]);

			// L
			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[1][yc][0][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[0][yc][0][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[0][yc+1][0][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[1][yc+1][0][0]);

			// R
			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[1][yc][1][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]);
			glVertex3fv(&deformed_column.column[0][yc][1][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0]+ntw,CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[0][yc+1][1][0]);

			glTexCoord2f(CLM_S[yc].getTextureAtlasPosition()[0],CLM_S[yc].getTextureAtlasPosition()[1]-nth);
			glVertex3fv(&deformed_column.column[1][yc+1][1][0]);
		}

		// T
		glTexCoord2f(CLM_S[column_height].getTextureAtlasPosition()[0],CLM_S[column_height].getTextureAtlasPosition()[1]);
		glVertex3fv(&deformed_column.column[0][0][0][0]);

		glTexCoord2f(CLM_S[column_height].getTextureAtlasPosition()[0]+ntw,CLM_S[column_height].getTextureAtlasPosition()[1]);
		glVertex3fv(&deformed_column.column[0][0][1][0]);

		glTexCoord2f(CLM_S[column_height].getTextureAtlasPosition()[0]+ntw,CLM_S[column_height].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&deformed_column.column[1][0][1][0]);

		glTexCoord2f(CLM_S[column_height].getTextureAtlasPosition()[0],CLM_S[column_height].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&deformed_column.column[1][0][0][0]);

		glEndList();

		targetBlock->getDisplayLists()->push_back(column_display_list);
	}

	GLvoid CRoomConstructor::draw_border1(GLint subtile_pos)
	{
		// T
		glTexCoord2f(BRD1_S[subtile_pos].getTextureAtlasPosition()[0],BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[0][0]);

		glTexCoord2f(BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[1][0]);

		glTexCoord2f(BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD1_S[subtile_pos].points[2][0]);

		glTexCoord2f(BRD1_S[subtile_pos].getTextureAtlasPosition()[0],BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD1_S[subtile_pos].points[3][0]);

		// L
		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[0][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[3][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

		// R
		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[1][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[2][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

		// U
		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[0][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[1][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

		// D
		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[3][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]);
		glVertex3fv(&BRD1_S[subtile_pos].points[2][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

		glTexCoord2f(SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD1_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
		glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);
	}

	GLvoid CRoomConstructor::initBorder()
	{
		GLfloat ntw = BTM_S[0].getTextureAtlasSubtileSize()/BTM_S[0].getTextureAtlasSize()[0]-BTM_S[0].getPixelWidth()*2.0f;
		GLfloat nth = BTM_S[0].getTextureAtlasSubtileSize()/BTM_S[0].getTextureAtlasSize()[1]-BTM_S[0].getPixelHeight()*2.0f;

		GLuint display_list=glGenLists(1);
		glNewList(display_list,GL_COMPILE);

		GLint terrain_type = targetBlock->getType();

		GLint subtile_pos = 0;

		if (terrain_type==CV_BLOCK_TYPE_HATCHERY_ID || (terrain_type==CV_BLOCK_TYPE_PORTAL_ID && targetBlock->isFaceVisible(CBlock::BFS_BOTTOM)))
		{
			for (subtile_pos=0; subtile_pos<9; subtile_pos++)
			{
				if (border_skip[subtile_pos])
				{
					continue;
				}
				// T
				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0],BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0],BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

				// B
				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[1][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[0][0]);

				// F
				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[3][0]);

				// L
				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[3][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[0][0]);

				// R
				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[1][0]);
			}
		}
		else  if (terrain_type==CV_BLOCK_TYPE_TEMPLE_ID)
		{
			if (!temple_animation)
			{
				// B
				for (subtile_pos=0; subtile_pos<3; subtile_pos++)
				{
					glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[1][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[0][0]);
				}

				// F
				for (subtile_pos=6; subtile_pos<9; subtile_pos++)
				{
					glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[3][0]);
				}

				// L
				for (subtile_pos=0; subtile_pos<7; subtile_pos+=3)
				{
					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[3][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[0][0]);
				}

				// R
				for (subtile_pos=2; subtile_pos<9; subtile_pos+=3)
				{
					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[1]);
					glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

					glTexCoord2f(SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3-2].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[1][0]);
				}
			}

			if (temple_border1_type!=TB_NONE)
			{
				if (temple_border1_type==TB_ALL)
				{
					for (subtile_pos=0; subtile_pos<9; subtile_pos++)
					{
						if (subtile_pos==4)
						{
							subtile_pos++;
						}
						draw_border1(subtile_pos);
					}
				}
				else if (temple_border1_type==TB_RIGHT)
				{
					for (subtile_pos=2; subtile_pos<9; subtile_pos+=3)
					{
						draw_border1(subtile_pos);
					}
				}
				else if (temple_border1_type==TB_LEFT)
				{
					for (subtile_pos=0; subtile_pos<7; subtile_pos+=3)
					{
						draw_border1(subtile_pos);
					}
				}
				else if (temple_border1_type==TB_UP)
				{
					for (subtile_pos=0; subtile_pos<3; subtile_pos++)
					{
						draw_border1(subtile_pos);
					}
				}
				else if (temple_border1_type==TB_DOWN)
				{
					for (subtile_pos=6; subtile_pos<9; subtile_pos++)
					{
						draw_border1(subtile_pos);
					}
				}
				else if (temple_border1_type==TB_TL)
				{
					draw_border1(0);
				}
				else if (temple_border1_type==TB_TR)
				{
					draw_border1(2);
				}
				else if (temple_border1_type==TB_BR)
				{
					draw_border1(8);
				}
				else if (temple_border1_type==TB_BL)
				{
					draw_border1(6);
				}
			}
		}
		else  if (terrain_type==CV_BLOCK_TYPE_BARRACKS_ID || terrain_type==CV_BLOCK_TYPE_GUARD_POST_ID)
		{
			for (subtile_pos=0; subtile_pos<9; subtile_pos++)
			{
				// T
				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0],BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

				glTexCoord2f(BRD0_S[subtile_pos].getTextureAtlasPosition()[0],BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);
			}

			// B
			for (subtile_pos=0; subtile_pos<3; subtile_pos++)
			{
				glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[1][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[0][0]);
			}

			// F
			for (subtile_pos=6; subtile_pos<9; subtile_pos++)
			{
				glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos-6].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[3][0]);
			}

			// L
			for (subtile_pos=0; subtile_pos<7; subtile_pos+=3)
			{
				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[0][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[3][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[3][0]);

				glTexCoord2f(SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[subtile_pos/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[0][0]);
			}

			// R
			for (subtile_pos=2; subtile_pos<9; subtile_pos+=3)
			{
				glTexCoord2f(SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[1][0]);

				glTexCoord2f(SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[1]);
				glVertex3fv(&BRD0_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[0]+ntw,SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

				glTexCoord2f(SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[0],SIDE_BRD0_S[(subtile_pos-2)/3].getTextureAtlasPosition()[1]-nth);
				glVertex3fv(&BTM_S[subtile_pos].points[1][0]);
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_HEART_ID)
		{
			if (heart_center)
			{
				for (subtile_pos=0; subtile_pos<49; subtile_pos++)
				{
					if (subtile_pos<25)
					{
						// T
						glTexCoord2f(HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[0][0]);

						glTexCoord2f(HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[1][0]);

						glTexCoord2f(HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[2][0]);

						glTexCoord2f(HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_UPPER_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[3][0]);

						// L
						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[0][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[3][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[3][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[0][0]);

						// R
						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[1][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[2][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[2][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[1][0]);

						// F
						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[3][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[2][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[2][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[3][0]);

						// B
						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[0][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
						glVertex3fv(&HEART_PEDESTAL_UPPER_TOP[subtile_pos].points[1][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[1][0]);

						glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
						glVertex3fv(&HEART_PEDESTAL_UPPER_BOTTOM[subtile_pos].points[0][0]);
					}

					// T
					glTexCoord2f(HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_LOWER_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[3][0]);

					// L
					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[0][0]);

					// R
					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[1][0]);

					// F
					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[3][0]);

					// B
					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_LOWER_TOP[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[0],HEART_PEDESTAL_SIDE[subtile_pos%5].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_LOWER_BOTTOM[subtile_pos].points[0][0]);
				}
			}
			else if (isColumnTile())
			{
				for (subtile_pos=0; subtile_pos<9; subtile_pos++)
				{
					if (border_skip[subtile_pos])
					{
						continue;
					}
					// T
					glTexCoord2f(HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_TOP[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[3][0]);

					// L-
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[0][0]);

					// L_
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[0][0]);

					// R-
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[1][0]);

					// R_
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[1][0]);

					// F-
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[3][0]);

					// F_
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[3][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[2][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[3][0]);

					
					// B-
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_TOP[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[0][0]);

					// B_
					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[0][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]);
					glVertex3fv(&HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0]+ntw,HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[1][0]);

					glTexCoord2f(HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[0],HEART_PEDESTAL_CORNER_MIDDLE[subtile_pos].getTextureAtlasPosition()[1]-nth);
					glVertex3fv(&BTM_S[subtile_pos].points[0][0]);

				}
			}
		}

		glEndList();

		targetBlock->getDisplayLists()->push_back(display_list);
	}

	GLvoid CRoomConstructor::addTileItems()
	{
		GLint terrain_type = targetBlock->getType();

		GLfloat cw3=CV_BLOCK_WIDTH/3.0f;
		GLfloat ch4=CV_BLOCK_HEIGHT/4.0f;
		GLfloat cd3=CV_BLOCK_DEPTH/3.0f;

		GLfloat cw2=CV_BLOCK_WIDTH/2.0f;
		GLfloat cd2=CV_BLOCK_DEPTH/2.0f;

		GLfloat x = targetBlock->getRealPosition()[0];
		GLfloat y = ch4;
		GLfloat z = targetBlock->getRealPosition()[2];

		if (terrain_type==CV_BLOCK_TYPE_PRISON_ID)
		{
			/*
				Prison has only one special item. It has prison bars around edges not surrounded
				by full blocks.
			*/
			GLfloat xps[9];
			GLfloat yps[9];
			GLint subtiles[8];
			GLint prison_bar_count=getEdgeSubtilesWhereNoFullBlock(xps,yps,subtiles);

			for (GLint i=0; i<prison_bar_count; i++)
			{
				targetBlock->addModel("MODEL_BAR",vector3f(xps[i],y,yps[i]),true);
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_GRAVEYARD_ID)
		{
			targetBlock->addModel("MODEL_GRAVE",vector3f(x+cw2,y,z+cd2),true);
		}
		else if (terrain_type==CV_BLOCK_TYPE_TRAINING_ROOM_ID)
		{
			if (isCenterTile())
			{
				targetBlock->addModel("MODEL_ROD",vector3f(x+cw2,y,z+cd2),true);
			}

			// we need to create the torch
			/*if (column)
			{
				GLint a=rand()%2;
				GLfloat px,pz;

				if (a==0)
				{
					px=cw3*(GLfloat)column_location_x+((rand()%2==0)?(cw3/1.5f):(-cw3/1.5f));
					pz=cd3*(GLfloat)column_location_z;
				}
				else
				{
					px=cw3*(GLfloat)column_location_x;
					pz=cd3*(GLfloat)column_location_z+((rand()%2==0)?(cd3/1.5f):(-cd3/1.5f));
				}

				targetBlock->addModel("MODEL_TORCH",vector3f(x+px,3.0f*ch4,z+pz));
			}*/
		}
		else if (terrain_type==CV_BLOCK_TYPE_HATCHERY_ID)
		{
			// we need to create the torch
			/*if (column)
			{
				GLint a=rand()%2;
				GLfloat px,pz;

				if (a==0)
				{
					px=cw3*(GLfloat)column_location_x+((rand()%2==0)?(cw3/1.5f):(-cw3/1.5f));
					pz=cd3*(GLfloat)column_location_z;
				}
				else
				{
					px=cw3*(GLfloat)column_location_x;
					pz=cd3*(GLfloat)column_location_z+((rand()%2==0)?(cd3/1.5f):(-cd3/1.5f));
				}

				dk_map->add_tile_item(map_x,map_y,TORCH_MODEL,x+px,3.0f*ch4,z+pz);
			}*/
		}
		else if (terrain_type==CV_BLOCK_TYPE_TORTURE_CHAMBER_ID)
		{
			if (isCenterTile())
			{
				targetBlock->addModel("MODEL_TORTURER",vector3f(x+cw2,y,z+cd2),true);
			}
			else if (isEdgeTile8())
			{
				targetBlock->addModel("MODEL_SPIKE",vector3f(x+cw2,y,z+cd2),true);
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_TEMPLE_ID)
		{
			if (column)
			{							
				GLfloat rotY = 0.0f;

				if (column_location_x==2 && column_location_z==0)
				{
					rotY = -45.0f;
				}
				else if (column_location_x==0 && column_location_z==0)
				{
					rotY = 45.0f;
				}
				else if (column_location_x==0 && column_location_z==2)
				{
					rotY = 135.0f;
				}
				else if (column_location_x==2 && column_location_z==2)
				{
					rotY = 225.0f;
				}

				targetBlock->addModel("MODEL_STATUE0",vector3f(x+cw2,y+ch4,z+cd2),true,rotY);
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_WORKSHOP_ID)
		{
			if (column)
			{
				targetBlock->addModel("MODEL_ANVIL",vector3f(x+cw2,y,z+cd2),true);
			}
			else if (isCenterTile())
			{
				targetBlock->addModel("MODEL_GADGET",vector3f(x+cw2,y,z+cd2),true);
			}
		}
		else if (terrain_type==CV_BLOCK_TYPE_HEART_ID)
		{
			if (!targetBlock->isFaceVisible(CBlock::BFS_BOTTOM))
			{
				targetBlock->addModel("MODEL_HEART",vector3f(x+cw2,HEART_PEDESTAL_UPPER_TOP[12].points[0][1],z+cd2),false);
			}
		}
	}
};