#include "commons.h"
#include "settings.h"
#include "BlockManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Conversions.h"
#include "utils.h"
#include <time.h>
#include "perlin.h"
#include "Logger.h"

using namespace std;
using namespace utils;
using namespace game_objects;
using namespace game_utils;
using namespace cml;

#define CLAIMED_PLAYER1			58
#define CLAIMED_PLAYER2			59
#define CLAIMED_PLAYER3			60
#define CLAIMED_PLAYER4			61
#define CLAIMED_PLAYER5			62

#define E_TL_N	203
#define E_TM_N	204
#define E_TR_N	205
#define E_ML_N	206
#define E_M_N	207
#define E_MR_N	208
#define E_BL_N	209	
#define E_BM_N	210	
#define E_BR_N	211
#define EARTH_TORCH_PLATE		212

#define R_TL_N	250
#define R_TM_N	251
#define R_TR_N	252
#define R_ML_N	253
#define R_M_N	254
#define R_MR_N	255
#define R_BL_N	256
#define R_BM_N	257
#define R_BR_N	258

#define CLAIMED_LAND_TL	111
#define CLAIMED_LAND_TM	112
#define CLAIMED_LAND_TR	113
#define CLAIMED_LAND_ML	114
#define CLAIMED_LAND_MR	115
#define CLAIMED_LAND_BL	116
#define CLAIMED_LAND_BM	117
#define CLAIMED_LAND_BR	118

#define UNCLAIMED_LAND0	100
#define UNCLAIMED_LAND1	101
#define UNCLAIMED_LAND2	102
#define UNCLAIMED_LAND3	103
#define UNCLAIMED_LAND4	104

// supports animation
#define LAVA_CLAIMED	301
#define LAVA_UNCLAIMED	302
#define LAVA_EARTH		303

#define LAVA0			304
#define LAVA1			305
#define LAVA2			306
#define LAVA3			307
#define LAVA_ANIM1_COUNT	4

#define LAVA_0			308
#define LAVA_1			309
#define LAVA_2			310
#define LAVA_3			311
#define LAVA_ANIM2_COUNT	4

#define LAVA__0			312
#define LAVA__1			313
#define LAVA__2			314
#define LAVA__3			315
#define LAVA_ANIM3_COUNT	4

#define LAVA___0		316
#define LAVA___1		317
#define LAVA___2		318
#define LAVA___3		319	
#define LAVA_ANIM4_COUNT	4

#define LAVA_ANIM_SPEED	0.015f

#define WATER_CLAIMED		401
#define WATER_UNCLAIMED		402

#define WATER_EARTH0		995
#define WATER_EARTH1		996
#define WATER_EARTH2		997
#define WATER_EARTH3		998
#define WATER_EARTH4		999
#define WATER_EARTH5		1000
#define WATER_EARTH6		1001
#define WATER_EARTH7		1002

#define WATER_EARTH_ANIM_SPEED	0.01f

#define WATER0			404
#define WATER1			405
#define WATER2			406
#define WATER3			407
#define WATER_ANIM1_COUNT	4

#define WATER_0			408
#define WATER_1			409
#define WATER_2			410
#define WATER_3			411
#define WATER_ANIM2_COUNT	4

#define WATER_ANIM_SPEED 0.015f

#define EARTH_AT_LAVA	300
#define EARTH_M			201
#define ROCK_M			202

#define GEM0	500
#define GEM1	501
#define GEM_ANIM_COUNT	2

#define GEM_ANIM_SPEED 0.025f

#define GEM_GOLD_LOW 504

#define GOLD0 600
#define GOLD1 601
#define GOLD2 602
#define GOLD3 603
#define GOLD_ANIM1_COUNT	4

#define GOLD_0 604
#define GOLD_1 605
#define GOLD_2 606
#define GOLD_3 607
#define GOLD_ANIM2_COUNT	4

#define GOLD__0 608
#define GOLD__1 609
#define GOLD__2 610
#define GOLD__3 611
#define GOLD_ANIM3_COUNT	4

#define GOLD_LAVA0	612
#define GOLD_LAVA1	613
#define GOLD_LAVA2	614
#define GOLD_LAVA3	615
#define GOLD_ANIM4_COUNT	4

#define GOLD_ANIM_SPEED 0.025f

#define WALL_CORNER	700
#define WALL_EDGE	701
#define WALL_CENTER 702

#define WALL_TOP			703
#define WALL_DECORATION		704
#define WALL_EMPTY			705
#define WALL_BOTTOM			706
#define WALL_MIDDLE_TOP0	707
#define WALL_MIDDLE_TOP1	708
#define WALL_MIDDLE_TOP2	709
#define WALL_MIDDLE0		710
#define WALL_MIDDLE1		711
#define WALL_MIDDLE2		712

#define WALL_MIDDLE_BOTTOM0 713
#define WALL_MIDDLE_BOTTOM1 714
#define WALL_MIDDLE_BOTTOM2 715

#define WALL_HANG_LEFT_P0	716
#define WALL_HANG_MIDDLE_P0	717
#define WALL_HANG_RIGHT_P0	718

#define WALL_HANG_LEFT_P1	716
#define WALL_HANG_MIDDLE_P1	717
#define WALL_HANG_RIGHT_P1	718

#define WALL_HANG_LEFT_P2	716
#define WALL_HANG_MIDDLE_P2	717
#define WALL_HANG_RIGHT_P2	718

#define WALL_HANG_LEFT_P3	716
#define WALL_HANG_MIDDLE_P3	717
#define WALL_HANG_RIGHT_P3	718

#define WALL_HANG_LEFT_P4	716
#define WALL_HANG_MIDDLE_P4	717
#define WALL_HANG_RIGHT_P4	718

#define WALL_HANG_LEFT_P5	716
#define WALL_HANG_MIDDLE_P5	717
#define WALL_HANG_RIGHT_P5	718

#define WALL_PAINTING0_0	719
#define WALL_PAINTING0_1	720
#define WALL_PAINTING0_2	721
#define WALL_PAINTING0_3	722
#define WALL_PAINTING0_4	723
#define WALL_PAINTING0_5	724
#define WALL_PAINTING0_6	725
#define WALL_PAINTING0_7	726
#define WALL_PAINTING0_8	727
#define WALL_PAINTING0_9	728
#define WALL_PAINTING0_10	729
#define WALL_PAINTING0_11	730

#define WALL_PAINTING1_0	731
#define WALL_PAINTING1_1	732
#define WALL_PAINTING1_2	733
#define WALL_PAINTING1_3	734
#define WALL_PAINTING1_4	735
#define WALL_PAINTING1_5	736
#define WALL_PAINTING1_6	737
#define WALL_PAINTING1_7	738
#define WALL_PAINTING1_8	739
#define WALL_PAINTING1_9	740
#define WALL_PAINTING1_10	741
#define WALL_PAINTING1_11	742

#define WALL_PAINTING2_0	743
#define WALL_PAINTING2_1	744
#define WALL_PAINTING2_2	745
#define WALL_PAINTING2_3	746
#define WALL_PAINTING2_4	747
#define WALL_PAINTING2_5	748
#define WALL_PAINTING2_6	749
#define WALL_PAINTING2_7	750
#define WALL_PAINTING2_8	751
#define WALL_PAINTING2_9	752
#define WALL_PAINTING2_10	753
#define WALL_PAINTING2_11	754

#define WALL_TORCH_PLATE	755

#define WALL_PAINTING_PRISON_0	756
#define WALL_PAINTING_PRISON_1	757
#define WALL_PAINTING_PRISON_2	758
#define WALL_PAINTING_PRISON_3	759
#define WALL_PAINTING_PRISON_4	760
#define WALL_PAINTING_PRISON_5	761

#define WALL_STONE_0 762
#define WALL_STONE_1 763

#define WALL_PAINTING_TREASURY_0	764
#define WALL_PAINTING_TREASURY_1	765
#define WALL_PAINTING_TREASURY_2	766
#define WALL_PAINTING_TREASURY_3	767
#define WALL_PAINTING_TREASURY_4	768
#define WALL_PAINTING_TREASURY_5	769

#define WALL_PAINTING_LAIR_0		770
#define WALL_PAINTING_LAIR_1		771
#define WALL_PAINTING_LAIR_2		772
#define WALL_PAINTING_LAIR_4		773
#define WALL_PAINTING_LAIR_7		774
#define WALL_PAINTING_LAIR_9		775
#define WALL_PAINTING_LAIR_10		776
#define WALL_PAINTING_LAIR_11		777

#define WALL_PAINTING_HATCHERY_3	778
#define WALL_PAINTING_HATCHERY_4	779
#define WALL_PAINTING_HATCHERY_5	780
#define WALL_PAINTING_HATCHERY_6	781
#define WALL_PAINTING_HATCHERY_7	782
#define WALL_PAINTING_HATCHERY_8	783
#define WALL_PAINTING_HATCHERY_9	784
#define WALL_PAINTING_HATCHERY_10	785
#define WALL_PAINTING_HATCHERY_11	786

#define WALL_PAINTING_TRAIN_EDGE_1		787
#define WALL_PAINTING_TRAIN_EDGE_4		788
#define WALL_PAINTING_TRAIN_EDGE_7		789
#define WALL_PAINTING_TRAIN_EDGE_10		790
#define WALL_PAINTING_TRAIN_MIDDLE_3	791
#define WALL_PAINTING_TRAIN_MIDDLE_4	792
#define WALL_PAINTING_TRAIN_MIDDLE_5	793
#define WALL_PAINTING_TRAIN_MIDDLE_6	794
#define WALL_PAINTING_TRAIN_MIDDLE_7	795
#define WALL_PAINTING_TRAIN_MIDDLE_8	796

#define WALL_PAINTING_LIBRARY0_3		797
#define WALL_PAINTING_LIBRARY0_4		798
#define WALL_PAINTING_LIBRARY0_5		799
#define WALL_PAINTING_LIBRARY0_6		800
#define WALL_PAINTING_LIBRARY0_7		801
#define WALL_PAINTING_LIBRARY0_8		802
#define WALL_PAINTING_LIBRARY1_3		803
#define WALL_PAINTING_LIBRARY1_4		804
#define WALL_PAINTING_LIBRARY1_5		805
#define WALL_PAINTING_LIBRARY1_6		806
#define WALL_PAINTING_LIBRARY1_7		807
#define WALL_PAINTING_LIBRARY1_8		808
#define WALL_PAINTING_LIBRARY2_3		809
#define WALL_PAINTING_LIBRARY2_4		810
#define WALL_PAINTING_LIBRARY2_5		811
#define WALL_PAINTING_LIBRARY2_6		812
#define WALL_PAINTING_LIBRARY2_7		813
#define WALL_PAINTING_LIBRARY2_8		814
#define WALL_PAINTING_LIBRARY3_3		815
#define WALL_PAINTING_LIBRARY3_4		816
#define WALL_PAINTING_LIBRARY3_5		817
#define WALL_PAINTING_LIBRARY3_6		818
#define WALL_PAINTING_LIBRARY3_7		819
#define WALL_PAINTING_LIBRARY3_8		820

#define LIBRARY_ANIM_SPEED 0.025f

#define WALL_PAINTING_LIBRARY_EDGE_3	821
#define WALL_PAINTING_LIBRARY_EDGE_4	822
#define WALL_PAINTING_LIBRARY_EDGE_5	823
#define WALL_PAINTING_LIBRARY_EDGE_6	824
#define WALL_PAINTING_LIBRARY_EDGE_7	825
#define WALL_PAINTING_LIBRARY_EDGE_8	826

#define WALL_PAINTING_LIBRARY_COLUMN_0	827
#define WALL_PAINTING_LIBRARY_COLUMN_3	828
#define WALL_PAINTING_LIBRARY_COLUMN_6	829
#define WALL_PAINTING_LIBRARY_COLUMN_9	830

#define WALL_PAINTING_TORTURE_0			833
#define WALL_PAINTING_TORTURE_1			834
#define WALL_PAINTING_TORTURE_2			835
#define WALL_PAINTING_TORTURE_3			836
#define WALL_PAINTING_TORTURE_4			837
#define WALL_PAINTING_TORTURE_5			838
#define WALL_PAINTING_TORTURE_6			839
#define WALL_PAINTING_TORTURE_7			840
#define WALL_PAINTING_TORTURE_8			841

#define WALL_PAINTING_BARRACKS_0		842
#define WALL_PAINTING_BARRACKS_1		843
#define WALL_PAINTING_BARRACKS_2		844
#define WALL_PAINTING_BARRACKS_3		845
#define WALL_PAINTING_BARRACKS_4		846
#define WALL_PAINTING_BARRACKS_5		847
#define WALL_PAINTING_BARRACKS_6		848
#define WALL_PAINTING_BARRACKS_7		849
#define WALL_PAINTING_BARRACKS_8		850
#define WALL_PAINTING_BARRACKS_COLUMN_0 851
#define WALL_PAINTING_BARRACKS_COLUMN_3 852
#define WALL_PAINTING_BARRACKS_COLUMN_6 853
#define WALL_PAINTING_BARRACKS_COLUMN_9 854

#define WALL_PAINTING_TEMPLE_0			855
#define WALL_PAINTING_TEMPLE_1			856
#define WALL_PAINTING_TEMPLE_2			857
#define WALL_PAINTING_TEMPLE_3			858
#define WALL_PAINTING_TEMPLE_4			859
#define WALL_PAINTING_TEMPLE_5			860
#define WALL_PAINTING_TEMPLE_6			861
#define WALL_PAINTING_TEMPLE_7			862
#define WALL_PAINTING_TEMPLE_8			863
#define WALL_PAINTING_TEMPLE_9_10_11	864
#define WALL_PAINTING_TEMPLE_COLUMN_1_7	865
#define WALL_PAINTING_TEMPLE_COLUMN_4	866

#define WALL_PAINTING_GRAVEYARD_FENCE_0	868
#define WALL_PAINTING_GRAVEYARD_FENCE_1	869

#define WALL_PAINTING_WORKSHOP0_3		871
#define WALL_PAINTING_WORKSHOP0_4		872
#define WALL_PAINTING_WORKSHOP0_5		873
#define WALL_PAINTING_WORKSHOP0_6		874
#define WALL_PAINTING_WORKSHOP0_7		875
#define WALL_PAINTING_WORKSHOP0_8		876
#define WALL_PAINTING_WORKSHOP1_3		877
#define WALL_PAINTING_WORKSHOP1_4		878
#define WALL_PAINTING_WORKSHOP1_5		879
#define WALL_PAINTING_WORKSHOP1_6		880
#define WALL_PAINTING_WORKSHOP1_7		881
#define WALL_PAINTING_WORKSHOP1_8		882
#define WALL_PAINTING_WORKSHOP2_3		883
#define WALL_PAINTING_WORKSHOP2_4		884
#define WALL_PAINTING_WORKSHOP2_5		885
#define WALL_PAINTING_WORKSHOP2_6		886
#define WALL_PAINTING_WORKSHOP2_7		887
#define WALL_PAINTING_WORKSHOP2_8		888
#define WALL_PAINTING_WORKSHOP3_3		889
#define WALL_PAINTING_WORKSHOP3_4		890
#define WALL_PAINTING_WORKSHOP3_5		891
#define WALL_PAINTING_WORKSHOP3_6		892
#define WALL_PAINTING_WORKSHOP3_7		893
#define WALL_PAINTING_WORKSHOP3_8		894
#define WALL_PAINTING_WORKSHOP_COLUMN_0	895
#define WALL_PAINTING_WORKSHOP_COLUMN_3 896
#define WALL_PAINTING_WORKSHOP_COLUMN_6 897
#define WALL_PAINTING_WORKSHOP_COLUMN_9 898

#define WORKSHOP_ANIM_SPEED 0.025f

#define MARKED_EARTH0	1110
#define MARKED_EARTH1	1111
#define MARKED_EARTH2	1112
#define MARKED_EARTH3	1113

#define MARKED_GOLD_GEM0	1114
#define MARKED_GOLD_GEM1	1115
#define MARKED_GOLD_GEM2	1116
#define MARKED_GOLD_GEM3	1117

#define CLAIMED_LAND_EDGE_TL	1118
#define CLAIMED_LAND_EDGE_T		1119
#define CLAIMED_LAND_EDGE_TR	1120
#define CLAIMED_LAND_EDGE_L		1121
#define CLAIMED_LAND_EDGE_R		1122
#define CLAIMED_LAND_EDGE_BL	1123
#define CLAIMED_LAND_EDGE_B		1124
#define CLAIMED_LAND_EDGE_BR	1125

namespace game_utils
{
	namespace managers
	{
		CBlockManager::CBlockManager(): CManager(), globalDeformedMap(NULL)
		{
			srand(time(NULL));
		}

		CBlockManager::~CBlockManager()
		{		
		}

		bool CBlockManager::init()
		{				
			bool state = true;

			CLogger::setEntryStart();

			// create temp global deformed map
			state&=createGlobalDeformedMap();
			CLogger::setEntryEnd("\tGlobal deformed cube creation.");

			// and texture atlas coordinates
			state&=readTextureAtlasCoordinates();			

			// create an instance of room constructor
			roomConstructor = new CRoomConstructor();

			// read texture atlas info from settings
			CSettingsManager *sManager = game_utils::CV_GAME_MANAGER->getSettingsManager();			

			// read texture quality then proper options
			string textureQuality = sManager->getSetting_String(CV_SETTINGS_TEXTURE_QUALITY);

			textureAtlasWidth = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_WIDTH)+"_"+textureQuality);
			textureAtlasHeight = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_HEIGHT)+"_"+textureQuality);
			textureAtlasSubtileSize = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_SUBTILE_SIZE)+"_"+textureQuality);
			pixelWidth = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_PIXEL_WIDTH)+"_"+textureQuality);
			pixelHeight = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_PIXEL_HEIGHT)+"_"+textureQuality);

			return state;
		}

		GLint CBlockManager::getTexturePosInTextureAtlas(GLint terrainType, GLint owner)
		{
			return textureAtlasData[terrainType][owner];
		}

		bool CBlockManager::readTextureAtlasCoordinates()
		{
			// load texture coordinates from a file

			FILE *in=NULL;
			if (!(in=fopen((CV_RESOURCES_DIRECTORY+"\\"+CV_CONFIG_ATLAS_COORDS).c_str(),"rc")))
			{
				return false;
			}

			bool	done=false;
			char	buff[1024*60];
			GLint	bufferPos=0;
			size_t	fileSize=fread(buff,1,20000,in);

			fclose(in);

			GLint	linePos=0;
			char	line[200];

			while (!done)
			{
				if (buff[bufferPos]=='#')
				{
					while (buff[bufferPos]!='\n')
					{
						bufferPos++;
					}
					bufferPos++;
				}
				linePos=0;
				while (buff[bufferPos+linePos]!='\n')
				{
					line[linePos ]=buff[bufferPos+linePos];
					linePos++;
				}
				line[linePos++]='\0';

				char name[100];
				GLint owner=0;
				GLint atlasPos=0;
				GLint arrayPos=0;
				sscanf(line,"%s %d %d %d",name,&owner,&atlasPos,&arrayPos);

				textureAtlasData[arrayPos][owner]=atlasPos;

				bufferPos+=linePos;

				if ((size_t)bufferPos>=fileSize)
					done=true;
			}

			// fill-in templates

			////////////////////////////////////////////////////////////
			///////// ROCK and EARTH TEXTURE TEMPLATE  /////////////////
			////////////////////////////////////////////////////////////

			for (GLint i=0; i<3; i++)
			{
				rock_subtiles[i]=R_TL_N+1;
				rock_subtiles[i+3]=R_ML_N+1;
				rock_subtiles[i+6]=R_ML_N+1;
				rock_subtiles[i+9]=R_BL_N+1;

				earth_subtiles[i]=E_TL_N+1;
				earth_subtiles[i+3]=E_ML_N+1;
				earth_subtiles[i+6]=E_ML_N+1;
				earth_subtiles[i+9]=E_BL_N+1;		
			}

			////////////////////////////////////////////////////////////
			///////// NORMAL WALL PAINTING		   /////////////////////
			////////////////////////////////////////////////////////////

			normal_wall_2_edges[0]=WALL_TOP;		normal_wall_2_edges[1]=rand()%3+WALL_MIDDLE_TOP0;		normal_wall_2_edges[2]=WALL_TOP;
			normal_wall_2_edges[3]=WALL_DECORATION;	normal_wall_2_edges[4]=rand()%3+WALL_MIDDLE0;			normal_wall_2_edges[5]=WALL_DECORATION;
			normal_wall_2_edges[6]=WALL_EMPTY;		normal_wall_2_edges[7]=rand()%3+WALL_MIDDLE0;			normal_wall_2_edges[8]=WALL_EMPTY;
			normal_wall_2_edges[9]=WALL_BOTTOM;		normal_wall_2_edges[10]=rand()%3+WALL_MIDDLE_BOTTOM0;	normal_wall_2_edges[11]=WALL_BOTTOM;

			normal_wall_hang[0]=WALL_HANG_LEFT_P0;				normal_wall_hang[1]=WALL_HANG_MIDDLE_P0;			normal_wall_hang[2]=WALL_HANG_RIGHT_P0;
			normal_wall_hang[3]=rand()%3+WALL_MIDDLE0;			normal_wall_hang[4]=rand()%3+WALL_MIDDLE0;			normal_wall_hang[5]=rand()%3+WALL_MIDDLE0;
			normal_wall_hang[6]=rand()%3+WALL_MIDDLE0;			normal_wall_hang[7]=rand()%3+WALL_MIDDLE0;			normal_wall_hang[8]=rand()%3+WALL_MIDDLE0;
			normal_wall_hang[9]=rand()%3+WALL_MIDDLE_BOTTOM0;	normal_wall_hang[10]=rand()%3+WALL_MIDDLE_BOTTOM0;	normal_wall_hang[11]=rand()%3+WALL_MIDDLE_BOTTOM0;

			normal_wall_ending_left[0]=WALL_TOP;		normal_wall_ending_left[1]=rand()%3+WALL_MIDDLE_TOP0;		normal_wall_ending_left[2]=rand()%3+WALL_MIDDLE_TOP0;
			normal_wall_ending_left[3]=WALL_DECORATION;	normal_wall_ending_left[4]=rand()%3+WALL_MIDDLE0;			normal_wall_ending_left[5]=rand()%3+WALL_MIDDLE0;
			normal_wall_ending_left[6]=WALL_EMPTY;		normal_wall_ending_left[7]=rand()%3+WALL_MIDDLE0;			normal_wall_ending_left[8]=rand()%3+WALL_MIDDLE0;
			normal_wall_ending_left[9]=WALL_BOTTOM;		normal_wall_ending_left[10]=rand()%3+WALL_MIDDLE_BOTTOM0;	normal_wall_ending_left[11]=rand()%3+WALL_MIDDLE_BOTTOM0;

			normal_wall_ending_right[2]=WALL_TOP;		normal_wall_ending_right[1]=rand()%3+WALL_MIDDLE_TOP0;		normal_wall_ending_right[0]=rand()%3+WALL_MIDDLE_TOP0;
			normal_wall_ending_right[5]=WALL_DECORATION;normal_wall_ending_right[4]=rand()%3+WALL_MIDDLE0;			normal_wall_ending_right[3]=rand()%3+WALL_MIDDLE0;
			normal_wall_ending_right[8]=WALL_EMPTY;		normal_wall_ending_right[7]=rand()%3+WALL_MIDDLE0;			normal_wall_ending_right[6]=rand()%3+WALL_MIDDLE0;
			normal_wall_ending_right[11]=WALL_BOTTOM;	normal_wall_ending_right[10]=rand()%3+WALL_MIDDLE_BOTTOM0;	normal_wall_ending_right[9]=rand()%3+WALL_MIDDLE_BOTTOM0;

			normal_wall_no_edge[0]=normal_wall_no_edge[1]=normal_wall_no_edge[2]=rand()%3+WALL_MIDDLE_TOP0;
			normal_wall_no_edge[3]=normal_wall_no_edge[4]=normal_wall_no_edge[5]=rand()%3+WALL_MIDDLE0;
			normal_wall_no_edge[6]=normal_wall_no_edge[7]=normal_wall_no_edge[8]=rand()%3+WALL_MIDDLE0;
			normal_wall_no_edge[9]=normal_wall_no_edge[10]=normal_wall_no_edge[11]=rand()%3+WALL_MIDDLE_BOTTOM0;

			for (GLint i=0; i<12; i++)
			{
				normal_wall_painting0[i]=WALL_PAINTING0_0+i;
				normal_wall_painting1[i]=WALL_PAINTING1_0+i;
				normal_wall_painting2[i]=WALL_PAINTING2_0+i;
			}

			////////////////////////////////////////////////////////////
			///////// PRISON WALL PAINTING         /////////////////////
			////////////////////////////////////////////////////////////

			// prison middle
			for (GLint i=0; i<12; i++)
			{
				if (i<3)
				{
					room_wall_prison_middle[i]=rand()%3+WALL_MIDDLE_TOP0;
				}
				else if (i<9)
				{
					room_wall_prison_middle[i]=WALL_PAINTING_PRISON_0+i-3;
				}
				else
				{
					room_wall_prison_middle[i]=rand()%3+WALL_MIDDLE_BOTTOM0;
				}
			}

			// prison edge
			for (GLint i=0; i<12; i++)
			{
				room_wall_prison_edge[i]=normal_wall_hang[i];
				if (i>3 && i%3==1)
				{
					room_wall_prison_edge[i]=rand()%2+WALL_STONE_0;
				}
			}

			////////////////////////////////////////////////////////////
			///////// TREASURE ROOM WALL PAINTING  /////////////////////
			////////////////////////////////////////////////////////////

			// treasury middle
			for (GLint i=0; i<12; i++)
			{
				if (i<3)
				{
					room_wall_treasury_middle[i]=WALL_HANG_LEFT_P0+i;
				}
				else if (i<9)
				{
					room_wall_treasury_middle[i]=WALL_PAINTING_TREASURY_0+i-3;
				}
				else
				{
					room_wall_treasury_middle[i]=rand()%3+WALL_MIDDLE_BOTTOM0;
				}
			}

			// treasury edge
			for (GLint i=0; i<12; i++)
			{
				room_wall_treasury_edge[i]=normal_wall_hang[i];
			}

			////////////////////////////////////////////////////////////
			///////// LAIR WALL PAINTING		   /////////////////////
			////////////////////////////////////////////////////////////

			// lair middle
			room_wall_lair_middle[0]=WALL_PAINTING_LAIR_0;		room_wall_lair_middle[1]=WALL_PAINTING_LAIR_1;		room_wall_lair_middle[2]=WALL_PAINTING_LAIR_2;
			room_wall_lair_middle[3]=rand()%3+WALL_MIDDLE0;		room_wall_lair_middle[4]=WALL_PAINTING_LAIR_4;		room_wall_lair_middle[5]=rand()%3+WALL_MIDDLE0;
			room_wall_lair_middle[6]=rand()%3+WALL_MIDDLE0;		room_wall_lair_middle[7]=WALL_PAINTING_LAIR_7;		room_wall_lair_middle[8]=rand()%3+WALL_MIDDLE0;
			room_wall_lair_middle[9]=WALL_PAINTING_LAIR_9;		room_wall_lair_middle[10]=WALL_PAINTING_LAIR_10;	room_wall_lair_middle[11]=WALL_PAINTING_LAIR_11;

			// lair edge	
			for (GLint i=0; i<12; i++)
			{
				if (i<3)
				{
					room_wall_lair_edge[i]=WALL_HANG_LEFT_P0+i;
				}
				else
				{
					room_wall_lair_edge[i]=room_wall_lair_middle[i];
				}
			}

			////////////////////////////////////////////////////////////
			///////// HARCHERY WALL PAINTING	   /////////////////////
			////////////////////////////////////////////////////////////

			// hatchery middle
			for (GLint i=0; i<12; i++)
			{
				if (i<3)
				{
					room_wall_hatchery_middle[i]=WALL_HANG_LEFT_P0+i;
				}
				else
				{
					room_wall_hatchery_middle[i]=WALL_PAINTING_HATCHERY_3+i-3;
				}
			}

			// hatchery edge
			for (GLint i=0; i<12; i++)
			{
				room_wall_hatchery_edge[i]=room_wall_prison_edge[i];
			}

			////////////////////////////////////////////////////////////
			///////// TRAINING ROOM WALL PAINTING  /////////////////////
			////////////////////////////////////////////////////////////

			// training room edge
			for (GLint i=0; i<12; i++)
			{
				if (i%3==1)
				{
					room_wall_train_edge[i]=WALL_PAINTING_TRAIN_EDGE_1+i/3;
				}
				else
				{
					room_wall_train_edge[i]=normal_wall_no_edge[i];
				}
			}

			// training room middle
			for (GLint i=0; i<12; i++)
			{
				if (i>=3 && i<=8)
				{
					room_wall_train_middle[i]=WALL_PAINTING_TRAIN_MIDDLE_3+i-3;
				}
				else
				{
					room_wall_train_middle[i]=room_wall_train_edge[i];
				}
			}

			// training room left/right wall
			for (GLint i=0; i<12; i++)
			{
				if (i%3==0)
				{
					room_wall_train_left_edge[i]=WALL_PAINTING_TRAIN_EDGE_1+i/3;
				}
				else
				{
					room_wall_train_left_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==2)
				{
					room_wall_train_right_edge[i]=WALL_PAINTING_TRAIN_EDGE_1+i/3;
				}
				else
				{
					room_wall_train_right_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==0)
				{
					room_wall_train_edge_2[i]=room_wall_train_left_edge[i];
				}
				else if (i%3==2)
				{
					room_wall_train_edge_2[i]=room_wall_train_right_edge[i];
				}
				else
				{
					room_wall_train_edge_2[i]=normal_wall_no_edge[i];
				}
			}

			////////////////////////////////////////////////////////////
			///////// LIBRARY WALL PAINTING        /////////////////////
			////////////////////////////////////////////////////////////

			// library edge
			for (GLint i=0; i<12; i++)
			{
				if (i>=3 && i<=8)
				{
					room_wall_library_book_case[i]=WALL_PAINTING_LIBRARY_EDGE_3+i-3;
				}
				else
				{
					room_wall_library_book_case[i]=normal_wall_hang[i];
				}
			}

			// library left/right wall
			for (GLint i=0; i<12; i++)
			{
				if (i%3==0)
				{
					room_wall_library_left_edge[i]=WALL_PAINTING_LIBRARY_COLUMN_0+i/3;
				}
				else
				{
					room_wall_library_left_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==2)
				{
					room_wall_library_right_edge[i]=WALL_PAINTING_LIBRARY_COLUMN_0+i/3;
				}
				else
				{
					room_wall_library_right_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==0)
				{
					room_wall_library_edge_2[i]=room_wall_library_left_edge[i];
				}
				else if (i%3==2)
				{
					room_wall_library_edge_2[i]=room_wall_library_right_edge[i];
				}
				else
				{
					room_wall_library_edge_2[i]=normal_wall_no_edge[i];
				}
			}

			////////////////////////////////////////////////////////////
			///////// TORTURE CHAMBER WALL PAINTING  ///////////////////
			////////////////////////////////////////////////////////////

			// torture chamber middle
			for (GLint i=0; i<12; i++)
			{
				if (i<9)
				{
					room_wall_torture_middle[i]=WALL_PAINTING_TORTURE_0+i;
				}
				else 
				{
					room_wall_torture_middle[i]=normal_wall_no_edge[i];
				}
			}

			///////////////////////////////////////////////////////
			///////// BARRACKS WALL PAINTING  /////////////////////
			///////////////////////////////////////////////////////

			// barracks room edge
			for (GLint i=0; i<12; i++)
			{
				if (i>1 && i%3==1)
				{
					if (i==4 || i==10)
					{
						room_wall_barracks_edge[i]=WALL_PAINTING_BARRACKS_COLUMN_0;
					}
					else if (i==7)
					{
						room_wall_barracks_edge[i]=WALL_PAINTING_BARRACKS_COLUMN_9;
					}
				}
				else
				{
					room_wall_barracks_edge[i]=normal_wall_hang[i];
				}
			}

			// barracks room middle
			for (GLint i=0; i<12; i++)
			{
				if (i>=0 && i<=8)
				{
					room_wall_barracks_middle[i]=WALL_PAINTING_BARRACKS_0+i;
				}
				else
				{
					room_wall_barracks_middle[i]=normal_wall_no_edge[i];
				}
			}

			// barracks room left/right wall
			for (GLint i=0; i<12; i++)
			{
				if (i%3==0)
				{
					room_wall_barracks_left_edge[i]=WALL_PAINTING_BARRACKS_COLUMN_0+i/3;
				}
				else
				{
					room_wall_barracks_left_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==2)
				{
					room_wall_barracks_right_edge[i]=WALL_PAINTING_BARRACKS_COLUMN_0+i/3;
				}
				else
				{
					room_wall_barracks_right_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==0)
				{
					room_wall_barracks_edge_2[i]=room_wall_barracks_left_edge[i];
				}
				else if (i%3==2)
				{
					room_wall_barracks_edge_2[i]=room_wall_barracks_right_edge[i];
				}
				else
				{
					room_wall_barracks_edge_2[i]=normal_wall_no_edge[i];
				}
			}

			///////////////////////////////////////////////////////
			///////// TEMPLE WALL PAINTING  /////////////////////
			///////////////////////////////////////////////////////

			// temple room edge
			for (GLint i=0; i<12; i++)
			{
				if (i>1 && i%3==1)
				{
					if (i==4)
					{
						room_wall_temple_edge[i]=WALL_PAINTING_TEMPLE_COLUMN_4;
					}
					else if (i==7)
					{
						room_wall_temple_edge[i]=WALL_PAINTING_TEMPLE_COLUMN_1_7;
					}
					else if (i==10)
					{
						room_wall_temple_edge[i]=WALL_PAINTING_TEMPLE_9_10_11;
					}
				}
				else
				{
					room_wall_temple_edge[i]=normal_wall_hang[i];
				}
			}

			// temple room middle
			for (GLint i=0; i<12; i++)
			{
				if (i>=0 && i<=8)
				{
					room_wall_temple_middle[i]=WALL_PAINTING_TEMPLE_0+i;
				}
				else if (i==10)
				{
					room_wall_temple_middle[i]=WALL_PAINTING_TEMPLE_9_10_11;
				}
				else
				{
					room_wall_temple_middle[i]=normal_wall_no_edge[i];
				}
			}

			// temple room left/right wall
			for (GLint i=0; i<12; i++)
			{
				if (i%3==0)
				{
					if (i==0 || i==6)
					{
						room_wall_temple_left_edge[i]=WALL_PAINTING_TEMPLE_COLUMN_1_7;
					}
					else if (i==3)
					{
						room_wall_temple_left_edge[i]=WALL_PAINTING_TEMPLE_COLUMN_4;
					}
					else
					{
						room_wall_temple_left_edge[i]=WALL_PAINTING_TEMPLE_9_10_11;
					}
				}
				else
				{
					room_wall_temple_left_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==2)
				{
					if (i==2 || i==8)
					{
						room_wall_temple_right_edge[i]=WALL_PAINTING_TEMPLE_COLUMN_1_7;
					}
					else if (i==5)
					{
						room_wall_temple_right_edge[i]=WALL_PAINTING_TEMPLE_COLUMN_4;
					}
					else
					{
						room_wall_temple_right_edge[i]=WALL_PAINTING_TEMPLE_9_10_11;
					}
				}
				else
				{
					room_wall_temple_right_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==0)
				{
					room_wall_temple_edge_2[i]=room_wall_temple_left_edge[i];
				}
				else if (i%3==2)
				{
					room_wall_temple_edge_2[i]=room_wall_temple_right_edge[i];
				}
				else
				{
					room_wall_temple_edge_2[i]=normal_wall_no_edge[i];
				}
			}

			///////////////////////////////////////////////////////
			///////// GRAVEYARD WALL PAINTING  ////////////////////
			///////////////////////////////////////////////////////

			// graveyard room edge
			for (GLint i=0; i<12; i++)
			{
				room_wall_graveyard_middle[i]=normal_wall_no_edge[i];
				room_wall_graveyard_edge_2[i]=normal_wall_2_edges[i];
				room_wall_graveyard_left_edge[i]=normal_wall_ending_left[i];
				room_wall_graveyard_right_edge[i]=normal_wall_ending_right[i];
			}

			for (GLint i=9; i<12; i++)
			{
				room_wall_graveyard_middle[i]=WALL_PAINTING_GRAVEYARD_FENCE_0+rand()%2;
				if (i<11)
				{
					room_wall_graveyard_right_edge[i]=WALL_PAINTING_GRAVEYARD_FENCE_0+rand()%2;
				}

				if (i>9)
				{
					room_wall_graveyard_left_edge[i]=WALL_PAINTING_GRAVEYARD_FENCE_0+rand()%2;
				}
			}

			room_wall_graveyard_edge_2[10]=WALL_PAINTING_GRAVEYARD_FENCE_0+rand()%2;


			////////////////////////////////////////////////////////////
			///////// WORKSHOP WALL PAINTING        /////////////////////
			////////////////////////////////////////////////////////////

			// workshop edge
			for (GLint i=0; i<12; i++)
			{
				if (i>1 && i%3==1)
				{
					room_wall_workshop_edge[i]=WALL_PAINTING_WORKSHOP_COLUMN_0+i/3;
				}
				else
				{
					room_wall_workshop_edge[i]=normal_wall_hang[i];
				}
			}

			// workshop left/right wall
			for (GLint i=0; i<12; i++)
			{
				if (i%3==0)
				{
					room_wall_workshop_left_edge[i]=WALL_PAINTING_WORKSHOP_COLUMN_0+i/3;
				}
				else
				{
					room_wall_workshop_left_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==2)
				{
					room_wall_workshop_right_edge[i]=WALL_PAINTING_WORKSHOP_COLUMN_0+i/3;
				}
				else
				{
					room_wall_workshop_right_edge[i]=normal_wall_no_edge[i];
				}

				if (i%3==0)
				{
					room_wall_workshop_edge_2[i]=room_wall_workshop_left_edge[i];
				}
				else if (i%3==2)
				{
					room_wall_workshop_edge_2[i]=room_wall_workshop_right_edge[i];
				}
				else
				{
					room_wall_workshop_edge_2[i]=normal_wall_no_edge[i];
				}
			}

			//////////////////////////////////////////////////////////

			middle_wall_rooms[0]=CV_BLOCK_TYPE_PRISON_ID;
			middle_wall_rooms[1]=CV_BLOCK_TYPE_TREASURE_ROOM_ID;
			middle_wall_rooms[2]=CV_BLOCK_TYPE_LAIR_ID;
			middle_wall_rooms[3]=CV_BLOCK_TYPE_HATCHERY_ID;
			middle_wall_rooms[4]=CV_BLOCK_TYPE_TRAINING_ROOM_ID;
			middle_wall_rooms[5]=CV_BLOCK_TYPE_LIBRARY_ID;
			middle_wall_rooms[6]=CV_BLOCK_TYPE_TORTURE_CHAMBER_ID;
			middle_wall_rooms[7]=CV_BLOCK_TYPE_BARRACKS_ID;
			middle_wall_rooms[8]=CV_BLOCK_TYPE_TEMPLE_ID;
			middle_wall_rooms[9]=CV_BLOCK_TYPE_GRAVEYARD_ID;
			middle_wall_rooms[10]=CV_BLOCK_TYPE_WORKSHOP_ID;

			room_wall_edge[0]=room_wall_prison_edge;
			room_wall_edge[1]=room_wall_treasury_edge;
			room_wall_edge[2]=room_wall_lair_edge;
			room_wall_edge[3]=room_wall_hatchery_edge;
			room_wall_edge[4]=room_wall_train_edge;
			room_wall_edge[5]=room_wall_library_book_case;
			room_wall_edge[6]=room_wall_torture_middle;
			room_wall_edge[7]=room_wall_barracks_edge;
			room_wall_edge[8]=room_wall_temple_edge;
			room_wall_edge[9]=room_wall_graveyard_middle;
			room_wall_edge[10]=room_wall_workshop_edge;

			room_wall_middle[0]=room_wall_prison_middle;
			room_wall_middle[1]=room_wall_treasury_middle;
			room_wall_middle[2]=room_wall_lair_middle;
			room_wall_middle[3]=room_wall_hatchery_middle;
			room_wall_middle[4]=room_wall_train_middle;
			room_wall_middle[5]=NULL;
			room_wall_middle[6]=room_wall_torture_middle;
			room_wall_middle[7]=room_wall_barracks_middle;
			room_wall_middle[8]=room_wall_temple_middle;
			room_wall_middle[9]=room_wall_graveyard_middle;
			room_wall_middle[10]=NULL;

			////////////////////////////////////////////////////////////

			left_right_wall_rooms[0]=CV_BLOCK_TYPE_TRAINING_ROOM_ID;
			left_right_wall_rooms[1]=CV_BLOCK_TYPE_LIBRARY_ID;
			left_right_wall_rooms[2]=CV_BLOCK_TYPE_BARRACKS_ID;
			left_right_wall_rooms[3]=CV_BLOCK_TYPE_TEMPLE_ID;
			left_right_wall_rooms[4]=CV_BLOCK_TYPE_GRAVEYARD_ID;
			left_right_wall_rooms[5]=CV_BLOCK_TYPE_WORKSHOP_ID;

			room_wall_edge_2[0]=room_wall_train_edge_2;
			room_wall_edge_2[1]=room_wall_library_edge_2;
			room_wall_edge_2[2]=room_wall_barracks_edge_2;
			room_wall_edge_2[3]=room_wall_temple_edge_2;
			room_wall_edge_2[4]=room_wall_graveyard_edge_2;
			room_wall_edge_2[5]=room_wall_workshop_edge_2;

			room_wall_left_edge[0]=room_wall_train_left_edge;
			room_wall_left_edge[1]=room_wall_library_left_edge;
			room_wall_left_edge[2]=room_wall_barracks_left_edge;
			room_wall_left_edge[3]=room_wall_temple_left_edge;
			room_wall_left_edge[4]=room_wall_graveyard_left_edge;
			room_wall_left_edge[5]=room_wall_workshop_left_edge;

			room_wall_right_edge[0]=room_wall_train_right_edge;
			room_wall_right_edge[1]=room_wall_library_right_edge;
			room_wall_right_edge[2]=room_wall_barracks_right_edge;
			room_wall_right_edge[3]=room_wall_temple_right_edge;
			room_wall_right_edge[4]=room_wall_graveyard_right_edge;
			room_wall_right_edge[5]=room_wall_workshop_right_edge;

			return true;
		}		

		GLvoid CBlockManager::fillSubtiles_BOTTOM(CBlock *block)
		{
			GLint	terrainType = block->getType();
			GLint	owner = block->getOwner();

			GLint CLAIMED_LAND = CV_BLOCK_TYPE_CLAIMED_LAND_ID;

			std::swap(ltype,rtype); // hack because of previous engine bug!

			if (terrainType==CLAIMED_LAND)
			{
				// we set up the sub-tiles				
				BTM_S[SR_M].create(getTexturePosInTextureAtlas(CLAIMED_LAND,owner));

				if (ltype!=CLAIMED_LAND && utype!=CLAIMED_LAND)
				{
					BTM_S[SR_TL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_TL,0));
				}
				else if (ltype==CLAIMED_LAND && utype!=CLAIMED_LAND)
				{
					BTM_S[SR_TL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_T,0));
				}
				else if (ltype!=CLAIMED_LAND && utype==CLAIMED_LAND)
				{
					BTM_S[SR_TL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_L,0));
				}
				else
				{
					BTM_S[SR_TL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_TL,0));
				}
				
				BTM_S[SR_TM].create(getTexturePosInTextureAtlas((utype==CLAIMED_LAND?CLAIMED_LAND_TM:CLAIMED_LAND_EDGE_T),0));

				if (rtype!=CLAIMED_LAND && utype!=CLAIMED_LAND)
				{
					BTM_S[SR_TR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_TR,0));
				}
				else if (rtype==CLAIMED_LAND && utype!=CLAIMED_LAND)
				{
					BTM_S[SR_TR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_T,0));
				}
				else if (rtype!=CLAIMED_LAND && utype==CLAIMED_LAND)
				{
					BTM_S[SR_TR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_R,0));
				}
				else
				{
					BTM_S[SR_TR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_TR,0));
				}

				BTM_S[SR_ML].create(getTexturePosInTextureAtlas((ltype==CLAIMED_LAND?CLAIMED_LAND_ML:CLAIMED_LAND_EDGE_L),0));
				BTM_S[SR_MR].create(getTexturePosInTextureAtlas((rtype==CLAIMED_LAND?CLAIMED_LAND_MR:CLAIMED_LAND_EDGE_R),0));

				if (ltype!=CLAIMED_LAND && dtype!=CLAIMED_LAND)
				{
					BTM_S[SR_BL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_BL,0));
				}
				else if (ltype==CLAIMED_LAND && dtype!=CLAIMED_LAND)
				{
					BTM_S[SR_BL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_B,0));
				}
				else if (ltype!=CLAIMED_LAND && dtype==CLAIMED_LAND)
				{
					BTM_S[SR_BL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_L,0));
				}
				else
				{
					BTM_S[SR_BL].create(getTexturePosInTextureAtlas(CLAIMED_LAND_BL,0));
				}

				BTM_S[SR_BM].create(getTexturePosInTextureAtlas((dtype==CLAIMED_LAND?CLAIMED_LAND_BM:CLAIMED_LAND_EDGE_B),0));

				if (rtype!=CLAIMED_LAND && dtype!=CLAIMED_LAND)
				{
					BTM_S[SR_BR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_BR,0));
				}
				else if (rtype==CLAIMED_LAND && dtype!=CLAIMED_LAND)
				{
					BTM_S[SR_BR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_B,0));
				}
				else if (rtype!=CLAIMED_LAND && dtype==CLAIMED_LAND)
				{
					BTM_S[SR_BR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_EDGE_R,0));
				}
				else
				{
					BTM_S[SR_BR].create(getTexturePosInTextureAtlas(CLAIMED_LAND_BR,0));
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_UNCLAIMED_LAND_ID)
			{
				// assing a random unclaimed texture
				for (GLint i=0; i<9; i++)
				{
					GLint a=rand()%100;
					a=rand()%100;
					a=rand()%100;
					if (a<=80)
					{
						a=rand()%3;
						a+=UNCLAIMED_LAND0;
					}
					else
					{
						a=rand()%10;
						if (a<=6)
						{					
							a=UNCLAIMED_LAND3;
						}
						else
						{
							a=UNCLAIMED_LAND4;
						}
					}
					BTM_S[i].create(getTexturePosInTextureAtlas(a,0));
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_LAVA_ID)
			{		
				GLint lava_animation1[]=
				{
					getTexturePosInTextureAtlas(LAVA0,0),
					getTexturePosInTextureAtlas(LAVA1,0),
					getTexturePosInTextureAtlas(LAVA2,0),
					getTexturePosInTextureAtlas(LAVA3,0)
				};

				GLint lava_animation2[]=
				{
					getTexturePosInTextureAtlas(LAVA_0,0),
					getTexturePosInTextureAtlas(LAVA_1,0),
					getTexturePosInTextureAtlas(LAVA_2,0),
					getTexturePosInTextureAtlas(LAVA_3,0),
				};

				GLint lava_animation3[]=
				{
					getTexturePosInTextureAtlas(LAVA__0,0),
					getTexturePosInTextureAtlas(LAVA__1,0),
					getTexturePosInTextureAtlas(LAVA__2,0),
					getTexturePosInTextureAtlas(LAVA__3,0),
				};

				GLint lava_animation4[]=
				{
					getTexturePosInTextureAtlas(LAVA___0,0),
					getTexturePosInTextureAtlas(LAVA___1,0),
					getTexturePosInTextureAtlas(LAVA___2,0),
					getTexturePosInTextureAtlas(LAVA___3,0),
				};


				for (GLint i=0; i<9; i++)
				{
					switch (rand()%4)
					{
						case 0:
						{
							BTM_S[i].create(lava_animation1,LAVA_ANIM1_COUNT);
							break;
						}
						case 1:
						{
							BTM_S[i].create(lava_animation2,LAVA_ANIM2_COUNT);
							break;
						}
						case 2:
						{
							BTM_S[i].create(lava_animation3,LAVA_ANIM3_COUNT);
							break;
						}
						case 3:
						{
							BTM_S[i].create(lava_animation4,LAVA_ANIM4_COUNT);
							break;
						}
					}
					BTM_S[i].setAnimSpeed(LAVA_ANIM_SPEED);
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_WATER_ID)
			{
				GLint water_animation1[]=
				{
					getTexturePosInTextureAtlas(WATER0,0),
					getTexturePosInTextureAtlas(WATER1,0),
					getTexturePosInTextureAtlas(WATER2,0),
					getTexturePosInTextureAtlas(WATER3,0)
				};

				GLint water_animation2[]=
				{
					getTexturePosInTextureAtlas(WATER_0,0),
					getTexturePosInTextureAtlas(WATER_1,0),
					getTexturePosInTextureAtlas(WATER_2,0),
					getTexturePosInTextureAtlas(WATER_3,0),
				};

				for (GLint i=0; i<9; i++)
				{
					switch (rand()%2)
					{
						case 0:
						{
							BTM_S[i].create(water_animation1,1);//WATER_ANIM1_COUNT);
							break;
						}
						case 1:
						{
							BTM_S[i].create(water_animation2,1);//WATER_ANIM2_COUNT);
							break;
						}
					}
					BTM_S[i].setAnimSpeed(WATER_ANIM_SPEED);
				}
			}

			std::swap(ltype,rtype); // hack because of previous engine bug!
		}

		bool CBlockManager::setRoomWallLeftRightEdge(GLint ntype, GLint terrainType, GLint room_wall[], CSubtileData SUB_DATA[])
		{
			if (ntype==terrainType)
			{
				for (GLint i=0; i<12; i++)
				{
					SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall[i],0));
				}
				return true;
			}
			return false;
		}

		GLvoid CBlockManager::setAnimationWall(CSubtileData SUB_DATA[], GLint near_terrainType)
		{
			if (near_terrainType==CV_BLOCK_TYPE_LIBRARY_ID)
			{
				GLint library_animation3[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY0_3,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY1_3,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY2_3,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY3_3,0)
				};
				GLint library_animation4[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY0_4,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY1_4,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY2_4,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY3_4,0)
				};
				GLint library_animation5[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY0_5,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY1_5,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY2_5,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY3_5,0)
				};
				GLint library_animation6[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY0_6,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY1_6,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY2_6,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY3_6,0)
				};
				GLint library_animation7[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY0_7,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY1_7,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY2_7,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY3_7,0)
				};
				GLint library_animation8[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY0_8,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY1_8,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY2_8,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_LIBRARY3_8,0)
				};

				GLint *arr[6]={library_animation3,library_animation4,library_animation5,library_animation6,library_animation7,library_animation8};

				for (GLint i=0; i<12; i++)
				{
					if (i>=3 && i<=8)
					{
						SUB_DATA[i].create(arr[i-3],4,false,true);
						SUB_DATA[i].setAnimSpeed(LIBRARY_ANIM_SPEED);
					}
					else
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(normal_wall_hang[i],(i<3)?owner:0),4);
					}
				}
			}
			else if (near_terrainType==CV_BLOCK_TYPE_WORKSHOP_ID)
			{
				GLint workshop_animation3[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP0_3,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP1_3,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP2_3,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP3_3,0)
				};
				GLint workshop_animation4[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP0_4,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP1_4,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP2_4,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP3_4,0)
				};
				GLint workshop_animation5[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP0_5,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP1_5,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP2_5,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP3_5,0)
				};
				GLint workshop_animation6[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP0_6,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP1_6,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP2_6,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP3_6,0)
				};
				GLint workshop_animation7[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP0_7,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP1_7,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP2_7,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP3_7,0)
				};
				GLint workshop_animation8[]=
				{
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP0_8,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP1_8,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP2_8,0),
					getTexturePosInTextureAtlas(WALL_PAINTING_WORKSHOP3_8,0)
				};

				GLint *arr[6]={workshop_animation3,workshop_animation4,workshop_animation5,workshop_animation6,workshop_animation7,workshop_animation8};

				for (GLint i=0; i<12; i++)
				{
					if (i>=3 && i<=8)
					{
						SUB_DATA[i].create(arr[i-3],4,false,true);
						SUB_DATA[i].setAnimSpeed(WORKSHOP_ANIM_SPEED);
					}
					else
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(normal_wall_hang[i],(i<3)?owner:0),4);
					}
				}
			}
		}

		bool CBlockManager::setFrontRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], CSubtileData SUB_DATA[])
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (dtype==terrainType)
			{
				if (lManager->getBlockType(mapX+1,mapY+1)!=terrainType || lManager->getBlockType(mapX-1,mapY+1)!=terrainType)
				{
					for (GLint i=0; i<12; i++)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_edge[i],(i<3)?owner:0));
					}
				}
				else
				{
					if (terrainType==CV_BLOCK_TYPE_LIBRARY_ID || terrainType==CV_BLOCK_TYPE_WORKSHOP_ID)
					{
						setAnimationWall(SUB_DATA,dtype);
					}
					else
					{
						for (GLint i=0; i<12; i++)
						{
							if (terrainType==CV_BLOCK_TYPE_BARRACKS_ID)
							{
								/* 
									Barracks are special since owner is not shown from top 3 tiles
									but only from tile nr. 4.
								*/
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i==4)?owner:0));
							}
							else
							{
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i<3)?owner:0));							
							}
						}
					}
				}
				return true;
			}
			return false;
		}

		bool CBlockManager::setBackRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], CSubtileData SUB_DATA[])
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (utype==terrainType)
			{
				if (lManager->getBlockType(mapX+1,mapY-1)!=terrainType || lManager->getBlockType(mapX-1,mapY-1)!=terrainType)
				{
					for (GLint i=0; i<12; i++)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_edge[i],(i<3)?owner:0));
					}
				}
				else
				{
					if (terrainType==CV_BLOCK_TYPE_LIBRARY_ID || terrainType==CV_BLOCK_TYPE_WORKSHOP_ID)
					{
						setAnimationWall(SUB_DATA,utype);
					}
					else
					{
						for (GLint i=0; i<12; i++)
						{
							if (terrainType==CV_BLOCK_TYPE_BARRACKS_ID)
							{
								/* 
									Barracks are special since owner is not shown from top 3 tiles
									but only from tile nr. 4. No player has either no color or all 
									colors randomly changing.
								*/
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i==4)?owner:0));
							}
							else
							{
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i<3)?owner:0));
							}
						}
					}
				}
				return true;
			}
			return false;
		}

		bool CBlockManager::setLeftRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], CSubtileData SUB_DATA[])
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (ltype==terrainType)
			{
				if (lManager->getBlockType(mapX-1,mapY+1)!=terrainType || lManager->getBlockType(mapX-1,mapY-1)!=terrainType)
				{
					for (GLint i=0; i<12; i++)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_edge[i],(i<3)?owner:0));
					}
				}
				else
				{
					if (terrainType==CV_BLOCK_TYPE_LIBRARY_ID || terrainType==CV_BLOCK_TYPE_WORKSHOP_ID)
					{
						setAnimationWall(SUB_DATA,ltype);
					}
					else
					{
						for (GLint i=0; i<12; i++)
						{
							if (terrainType==CV_BLOCK_TYPE_BARRACKS_ID)
							{
								/* 
									Barracks are special since owner is not shown from top 3 tiles
									but only from tile nr. 4.
								*/
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i==4)?owner:0));
							}
							else
							{
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i<3)?owner:0));
							}
						}
					}
				}
				return true;
			}
			return false;
		}

		bool CBlockManager::setRightRoomWall(GLint terrainType, GLint room_wall_edge[], GLint room_wall_middle[], CSubtileData SUB_DATA[])
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (rtype==terrainType)
			{
				if (lManager->getBlockType(mapX+1,mapY+1)!=terrainType || lManager->getBlockType(mapX+1,mapY-1)!=terrainType)
				{
					for (GLint i=0; i<12; i++)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_edge[i],(i<3)?owner:0));
					}
				}
				else
				{
					if (terrainType==CV_BLOCK_TYPE_LIBRARY_ID || terrainType==CV_BLOCK_TYPE_WORKSHOP_ID)
					{
						setAnimationWall(SUB_DATA,rtype);
					}
					else
					{
						for (GLint i=0; i<12; i++)
						{
							if (terrainType==CV_BLOCK_TYPE_BARRACKS_ID)
							{
								/* 
									Barracks are special since owner is not shown from top 3 tiles
									but only from tile nr. 4.
								*/
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i==4)?owner:0));
							}
							else
							{
								SUB_DATA[i].create(getTexturePosInTextureAtlas(room_wall_middle[i],(i<3)?owner:0));
							}
						}
					}
				}
				return true;
			}
			return false;
		}

		GLvoid CBlockManager::setNormalWall(CSubtileData SUB_DATA[])
		{
			if ((mapY*85+mapX)%2==0)
			{
				for (GLint i=0; i<12; i++)
				{
					if (i<3)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(normal_wall_hang[i],owner));
					}
					else
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(normal_wall_hang[i],0));
					}
				}
			}
			else
			{
				GLint a=rand()%3;				

				if (a==0)
				{
					for (GLint i=0; i<12; i++)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(normal_wall_painting0[i],0));
					}
				}
				else if (a==1)
				{
					for (GLint i=0; i<12; i++)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(normal_wall_painting1[i],0));
					}
				}
				else
				{
					for (GLint i=0; i<12; i++)
					{
						SUB_DATA[i].create(getTexturePosInTextureAtlas(normal_wall_painting2[i],0));
					}
				}

			}
		}

		GLvoid CBlockManager::fillSubtiles_FRONT(CBlock *block)
		{
			GLint	terrainType = block->getType();
			GLint	owner = block->getOwner();

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (terrainType==CV_BLOCK_TYPE_WALL_ID)
			{
				// if the wall block is alone then we use the normal_block template for it
				if (!lManager->isSameTypeAndOwner(mapX-1,mapY,block) && !lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(dtype,left_right_wall_rooms[i],room_wall_edge_2[i],FNT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{							
							FNT_S[i].create(getTexturePosInTextureAtlas(normal_wall_2_edges[i],0));
						}
					}
				}				
				else if (lManager->isSameTypeAndOwner(mapX-1,mapY,block) && lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_ALL; i++)
					{
						if (setFrontRoomWall(middle_wall_rooms[i],room_wall_edge[i],room_wall_middle[i],FNT_S))
						{
							found=true;
							break;
						}
					}
					
					if (!found)
					{
						//if not, we create the normal wall decoration
						setNormalWall(FNT_S);
					}
				}
				else if (!lManager->isSameTypeAndOwner(mapX-1,mapY,block) && lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(dtype,left_right_wall_rooms[i],room_wall_left_edge[i],FNT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							FNT_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_left[i],0));
						}
					}
				}
				else if (lManager->isSameTypeAndOwner(mapX-1,mapY,block) && !lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(dtype,left_right_wall_rooms[i],room_wall_right_edge[i],FNT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							FNT_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_right[i],0));
						}
					}
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_LAVA_ID || terrainType==CV_BLOCK_TYPE_WATER_ID)
			{	
				//if (!dk_map->is_full_square(dk_map->get_block_type(mapY+1,mapX)))
				if (lManager->getBlock(mapX,mapY+1)->isLow())
				{
					for (GLint i=9; i<12; i++)
					{
						FNT_S[i].create(getTexturePosInTextureAtlas((terrainType==CV_BLOCK_TYPE_LAVA_ID?LAVA_CLAIMED:WATER_CLAIMED),0));
					}
				}
				else
				{
					if (lManager->getBlockType(mapX,mapY+1)==CV_BLOCK_TYPE_ROCK_ID)
					{
						for (GLint i=9; i<12; i++)
						{
							FNT_S[i].create(getTexturePosInTextureAtlas(R_BL_N,0));
						}
					}
					else 
					{
						GLint water_earth_animation[]=
						{
							getTexturePosInTextureAtlas(WATER_EARTH0,0),
							getTexturePosInTextureAtlas(WATER_EARTH1,0),
							getTexturePosInTextureAtlas(WATER_EARTH2,0),
							getTexturePosInTextureAtlas(WATER_EARTH3,0),
							getTexturePosInTextureAtlas(WATER_EARTH4,0),
							getTexturePosInTextureAtlas(WATER_EARTH5,0),
							getTexturePosInTextureAtlas(WATER_EARTH6,0),
							getTexturePosInTextureAtlas(WATER_EARTH7,0)
						};

						if (terrainType==CV_BLOCK_TYPE_LAVA_ID)
						{
							for (GLint i=9; i<12; i++)
							{
								FNT_S[i].create(getTexturePosInTextureAtlas(LAVA_EARTH,0));
							}
						}
						else
						{
							for (GLint i=9; i<12; i++)
							{
								FNT_S[i].create(water_earth_animation,8,true,true);
								FNT_S[i].setAnimSpeed(WATER_EARTH_ANIM_SPEED);
							}
						}
					}			
				}
			}
			else
			{
				if (terrainType==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						FNT_S[i].create(getTexturePosInTextureAtlas(earth_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_ROCK_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						FNT_S[i].create(getTexturePosInTextureAtlas(rock_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GEM_ID)
				{
					GLint gem_animation[]=
					{
						getTexturePosInTextureAtlas(GEM0,0),
						getTexturePosInTextureAtlas(GEM1,0)
					};

					for (GLint i=0; i<9; i++)
					{
						FNT_S[i].create(gem_animation,GEM_ANIM_COUNT);
						FNT_S[i].setAnimSpeed(GEM_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						FNT_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),2);
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation1[]=
					{
						getTexturePosInTextureAtlas(GOLD0,0),
						getTexturePosInTextureAtlas(GOLD1,0),
						getTexturePosInTextureAtlas(GOLD2,0),
						getTexturePosInTextureAtlas(GOLD3,0)
					};

					GLint gold_animation2[]=
					{
						getTexturePosInTextureAtlas(GOLD_0,0),
						getTexturePosInTextureAtlas(GOLD_1,0),
						getTexturePosInTextureAtlas(GOLD_2,0),
						getTexturePosInTextureAtlas(GOLD_3,0),
					};

					GLint gold_animation3[]=
					{
						getTexturePosInTextureAtlas(GOLD__0,0),
						getTexturePosInTextureAtlas(GOLD__1,0),
						getTexturePosInTextureAtlas(GOLD__2,0),
						getTexturePosInTextureAtlas(GOLD__3,0),
					};

					for (GLint i=0; i<9; i++)
					{
						if (rand()%5==0)
						{
							FNT_S[i].create(gold_animation1,GOLD_ANIM1_COUNT);
						}
						else
						{
							if (rand()%2==0)
							{
								FNT_S[i].create(gold_animation2,GOLD_ANIM2_COUNT);
							}
							else
							{
								FNT_S[i].create(gold_animation3,GOLD_ANIM3_COUNT);
							}
						}
						FNT_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						FNT_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),4);
					}
				}

				if (dtype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						FNT_S[i].create(getTexturePosInTextureAtlas(EARTH_AT_LAVA,0));
					}
				}

				if (dtype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation4[]=
					{
						getTexturePosInTextureAtlas(GOLD_LAVA0,0),
						getTexturePosInTextureAtlas(GOLD_LAVA1,0),
						getTexturePosInTextureAtlas(GOLD_LAVA2,0),
						getTexturePosInTextureAtlas(GOLD_LAVA3,0),
					};

					for (GLint i=0; i<12; i++)
					{
						FNT_S[i].create(gold_animation4,GOLD_ANIM4_COUNT);
						FNT_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}
				}
			}
		}

		GLvoid CBlockManager::fillSubtiles_BACK(CBlock *block)
		{
			GLint	terrainType = block->getType();
			GLint	owner = block->getOwner();

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (terrainType==CV_BLOCK_TYPE_WALL_ID)
			{
				// if the wall block is alone then we use the normal_block template for it
				if (!lManager->isSameTypeAndOwner(mapX-1,mapY,block) && !lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(utype,left_right_wall_rooms[i],room_wall_edge_2[i],BCK_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							BCK_S[i].create(getTexturePosInTextureAtlas(normal_wall_2_edges[i],0));
						}
					}
				}
				else if (lManager->isSameTypeAndOwner(mapX-1,mapY,block) && lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_ALL; i++)
					{
						if (setBackRoomWall(middle_wall_rooms[i],room_wall_edge[i],room_wall_middle[i],BCK_S))
						{
							found=true;
							break;
						}
					}
					
					if (!found)
					{
						//if not, we create the normal wall decoration
						setNormalWall(BCK_S);
					}
				}
				else if (!lManager->isSameTypeAndOwner(mapX-1,mapY,block) && lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(utype,left_right_wall_rooms[i],room_wall_left_edge[i],BCK_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							BCK_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_left[i],0));
						}
					}
				}
				else if (lManager->isSameTypeAndOwner(mapX-1,mapY,block) && !lManager->isSameTypeAndOwner(mapX+1,mapY,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(utype,left_right_wall_rooms[i],room_wall_right_edge[i],BCK_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							BCK_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_right[i],0));
						}
					}
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_LAVA_ID || terrainType==CV_BLOCK_TYPE_WATER_ID)
			{		
				//if (!dk_map->is_full_square(dk_map->get_block_type(mapY-1,mapX)))
				if (lManager->getBlock(mapX,mapY-1)->isLow())
				{
					for (GLint i=9; i<12; i++)
					{
						BCK_S[i].create(getTexturePosInTextureAtlas((terrainType==CV_BLOCK_TYPE_LAVA_ID?LAVA_CLAIMED:WATER_CLAIMED),0));
					}
				}
				else
				{
					if (lManager->getBlockType(mapX,mapY-1)==CV_BLOCK_TYPE_ROCK_ID)
					{
						for (GLint i=9; i<12; i++)
						{
							BCK_S[i].create(getTexturePosInTextureAtlas(R_BL_N,0));
						}
					}
					else 
					{
						GLint water_earth_animation[]=
						{
							getTexturePosInTextureAtlas(WATER_EARTH0,0),
							getTexturePosInTextureAtlas(WATER_EARTH1,0),
							getTexturePosInTextureAtlas(WATER_EARTH2,0),
							getTexturePosInTextureAtlas(WATER_EARTH3,0),
							getTexturePosInTextureAtlas(WATER_EARTH4,0),
							getTexturePosInTextureAtlas(WATER_EARTH5,0),
							getTexturePosInTextureAtlas(WATER_EARTH6,0),
							getTexturePosInTextureAtlas(WATER_EARTH7,0)
						};

						if (terrainType==CV_BLOCK_TYPE_LAVA_ID)
						{
							for (GLint i=9; i<12; i++)
							{
								BCK_S[i].create(getTexturePosInTextureAtlas(LAVA_EARTH,0));
							}
						}
						else
						{
							for (GLint i=9; i<12; i++)
							{
								BCK_S[i].create(water_earth_animation,8,true,true);
								BCK_S[i].setAnimSpeed(WATER_EARTH_ANIM_SPEED);
							}
						}
					}			
				}
			}
			else
			{
				if (terrainType==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						BCK_S[i].create(getTexturePosInTextureAtlas(earth_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_ROCK_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						BCK_S[i].create(getTexturePosInTextureAtlas(rock_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GEM_ID)
				{
					GLint gem_animation[]=
					{
						getTexturePosInTextureAtlas(GEM0,0),
						getTexturePosInTextureAtlas(GEM1,0)
					};
					for (GLint i=0; i<9; i++)
					{
						BCK_S[i].create(gem_animation,GEM_ANIM_COUNT);
						BCK_S[i].setAnimSpeed(GEM_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						BCK_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),2);
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation1[]=
					{
						getTexturePosInTextureAtlas(GOLD0,0),
						getTexturePosInTextureAtlas(GOLD1,0),
						getTexturePosInTextureAtlas(GOLD2,0),
						getTexturePosInTextureAtlas(GOLD3,0)
					};

					GLint gold_animation2[]=
					{
						getTexturePosInTextureAtlas(GOLD_0,0),
						getTexturePosInTextureAtlas(GOLD_1,0),
						getTexturePosInTextureAtlas(GOLD_2,0),
						getTexturePosInTextureAtlas(GOLD_3,0),
					};

					GLint gold_animation3[]=
					{
						getTexturePosInTextureAtlas(GOLD__0,0),
						getTexturePosInTextureAtlas(GOLD__1,0),
						getTexturePosInTextureAtlas(GOLD__2,0),
						getTexturePosInTextureAtlas(GOLD__3,0),
					};

					for (GLint i=0; i<9; i++)
					{
						if (rand()%5==0)
						{
							BCK_S[i].create(gold_animation1,GOLD_ANIM1_COUNT);
						}
						else
						{
							if (rand()%2==0)
							{
								BCK_S[i].create(gold_animation2,GOLD_ANIM2_COUNT);
							}
							else
							{
								BCK_S[i].create(gold_animation3,GOLD_ANIM3_COUNT);
							}
						}
						BCK_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						BCK_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),4);
					}
				}

				if (utype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						BCK_S[i].create(getTexturePosInTextureAtlas(EARTH_AT_LAVA,0));
					}
				}

				if (utype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation4[]=
					{
						getTexturePosInTextureAtlas(GOLD_LAVA0,0),
						getTexturePosInTextureAtlas(GOLD_LAVA1,0),
						getTexturePosInTextureAtlas(GOLD_LAVA2,0),
						getTexturePosInTextureAtlas(GOLD_LAVA3,0),
					};

					for (GLint i=0; i<12; i++)
					{
						BCK_S[i].create(gold_animation4,GOLD_ANIM4_COUNT);
						BCK_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}
				}
			}
		}

		GLvoid CBlockManager::fillSubtiles_LEFT(CBlock *block)
		{
			GLint	terrainType = block->getType();
			GLint	owner = block->getOwner();

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (terrainType==CV_BLOCK_TYPE_WALL_ID)
			{
				// if the wall block is alone then we use the normal_block template for it
				if (!lManager->isSameTypeAndOwner(mapX,mapY-1,block) && !lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(ltype,left_right_wall_rooms[i],room_wall_edge_2[i],LFT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							LFT_S[i].create(getTexturePosInTextureAtlas(normal_wall_2_edges[i],0));
						}
					}
				}
				else if (lManager->isSameTypeAndOwner(mapX,mapY-1,block) && lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_ALL; i++)
					{
						if (setLeftRoomWall(middle_wall_rooms[i],room_wall_edge[i],room_wall_middle[i],LFT_S))
						{
							found=true;
							break;
						}
					}
					
					if (!found)
					{
						//if not, we create the normal wall decoration
						setNormalWall(LFT_S);
					}
				}
				else if (!lManager->isSameTypeAndOwner(mapX,mapY-1,block) && lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(ltype,left_right_wall_rooms[i],room_wall_left_edge[i],LFT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							LFT_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_left[i],0));
						}
					}
				}
				else if (lManager->isSameTypeAndOwner(mapX,mapY-1,block) && !lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(ltype,left_right_wall_rooms[i],room_wall_right_edge[i],LFT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							LFT_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_right[i],0));
						}
					}
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_LAVA_ID || terrainType==CV_BLOCK_TYPE_WATER_ID)
			{		
				//if (!dk_map->is_full_square(dk_map->get_block_type(mapY,mapX-1)))
				if (lManager->getBlock(mapX-1,mapY)->isLow())
				{
					for (GLint i=9; i<12; i++)
					{
						LFT_S[i].create(getTexturePosInTextureAtlas((terrainType==CV_BLOCK_TYPE_LAVA_ID?LAVA_CLAIMED:WATER_CLAIMED),0));
					}
				}
				else
				{
					if (lManager->getBlockType(mapX-1,mapY)==CV_BLOCK_TYPE_ROCK_ID)
					{
						for (GLint i=9; i<12; i++)
						{
							LFT_S[i].create(getTexturePosInTextureAtlas(R_BL_N,0));
						}
					}
					else 
					{
						GLint water_earth_animation[]=
						{
							getTexturePosInTextureAtlas(WATER_EARTH0,0),
							getTexturePosInTextureAtlas(WATER_EARTH1,0),
							getTexturePosInTextureAtlas(WATER_EARTH2,0),
							getTexturePosInTextureAtlas(WATER_EARTH3,0),
							getTexturePosInTextureAtlas(WATER_EARTH4,0),
							getTexturePosInTextureAtlas(WATER_EARTH5,0),
							getTexturePosInTextureAtlas(WATER_EARTH6,0),
							getTexturePosInTextureAtlas(WATER_EARTH7,0)
						};

						if (terrainType==CV_BLOCK_TYPE_LAVA_ID)
						{
							for (GLint i=9; i<12; i++)
							{
								LFT_S[i].create(getTexturePosInTextureAtlas(LAVA_EARTH,0));
							}
						}
						else
						{
							for (GLint i=9; i<12; i++)
							{
								LFT_S[i].create(water_earth_animation,8,true,true);
								LFT_S[i].setAnimSpeed(WATER_EARTH_ANIM_SPEED);
							}
						}
					}			
				}
			}
			else
			{
				if (terrainType==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						LFT_S[i].create(getTexturePosInTextureAtlas(earth_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_ROCK_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						LFT_S[i].create(getTexturePosInTextureAtlas(rock_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GEM_ID)
				{
					GLint gem_animation[]=
					{
						getTexturePosInTextureAtlas(GEM0,0),
						getTexturePosInTextureAtlas(GEM1,0)
					};
					for (GLint i=0; i<9; i++)
					{
						LFT_S[i].create(gem_animation,GEM_ANIM_COUNT);
						LFT_S[i].setAnimSpeed(GEM_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						LFT_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),2);
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation1[]=
					{
						getTexturePosInTextureAtlas(GOLD0,0),
						getTexturePosInTextureAtlas(GOLD1,0),
						getTexturePosInTextureAtlas(GOLD2,0),
						getTexturePosInTextureAtlas(GOLD3,0)
					};

					GLint gold_animation2[]=
					{
						getTexturePosInTextureAtlas(GOLD_0,0),
						getTexturePosInTextureAtlas(GOLD_1,0),
						getTexturePosInTextureAtlas(GOLD_2,0),
						getTexturePosInTextureAtlas(GOLD_3,0),
					};

					GLint gold_animation3[]=
					{
						getTexturePosInTextureAtlas(GOLD__0,0),
						getTexturePosInTextureAtlas(GOLD__1,0),
						getTexturePosInTextureAtlas(GOLD__2,0),
						getTexturePosInTextureAtlas(GOLD__3,0),
					};

					for (GLint i=0; i<9; i++)
					{
						if (rand()%5==0)
						{
							LFT_S[i].create(gold_animation1,GOLD_ANIM1_COUNT);
						}
						else
						{
							if (rand()%2==0)
							{
								LFT_S[i].create(gold_animation2,GOLD_ANIM2_COUNT);
							}
							else
							{
								LFT_S[i].create(gold_animation3,GOLD_ANIM3_COUNT);
							}
						}
						LFT_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						LFT_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),4);
					}
				}

				if (ltype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						LFT_S[i].create(getTexturePosInTextureAtlas(EARTH_AT_LAVA,0));
					}
				}

				if (ltype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation4[]=
					{
						getTexturePosInTextureAtlas(GOLD_LAVA0,0),
						getTexturePosInTextureAtlas(GOLD_LAVA1,0),
						getTexturePosInTextureAtlas(GOLD_LAVA2,0),
						getTexturePosInTextureAtlas(GOLD_LAVA3,0),
					};

					for (GLint i=0; i<12; i++)
					{
						LFT_S[i].create(gold_animation4,GOLD_ANIM4_COUNT);
						LFT_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}
				}
			}
		}

		GLvoid CBlockManager::fillSubtiles_RIGHT(CBlock *block)
		{
			GLint	terrainType = block->getType();
			GLint	owner = block->getOwner();

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (terrainType==CV_BLOCK_TYPE_WALL_ID)
			{
				// if the wall block is alone then we use the normal_block template for it
				if (!lManager->isSameTypeAndOwner(mapX,mapY-1,block) && !lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(rtype,left_right_wall_rooms[i],room_wall_edge_2[i],RGT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							RGT_S[i].create(getTexturePosInTextureAtlas(normal_wall_2_edges[i],0));
						}
					}
				}
				else if (lManager->isSameTypeAndOwner(mapX,mapY-1,block) && lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;

					for (GLint i=0; i<MAX_ROOM_ALL; i++)
					{
						if (setRightRoomWall(middle_wall_rooms[i],room_wall_edge[i],room_wall_middle[i],RGT_S))
						{
							found=true;
							break;
						}
					}
					
					if (!found)
					{
						//if not, we create the normal wall decoration
						setNormalWall(RGT_S);
					}
				}
				else if (!lManager->isSameTypeAndOwner(mapX,mapY-1,block) && lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(rtype,left_right_wall_rooms[i],room_wall_left_edge[i],RGT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							RGT_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_left[i],0));
						}
					}
				}
				else if (lManager->isSameTypeAndOwner(mapX,mapY-1,block) && !lManager->isSameTypeAndOwner(mapX,mapY+1,block))
				{
					bool found=false;
					for (GLint i=0; i<MAX_ROOM_NOT_ALL; i++)
					{
						if (setRoomWallLeftRightEdge(rtype,left_right_wall_rooms[i],room_wall_right_edge[i],RGT_S))
						{
							found=true;
							break;
						}
					}

					if (!found)
					{
						for (GLint i=0; i<12; i++)
						{
							RGT_S[i].create(getTexturePosInTextureAtlas(normal_wall_ending_right[i],0));
						}
					}
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_LAVA_ID || terrainType==CV_BLOCK_TYPE_WATER_ID)
			{		
				//if (!dk_map->is_full_square(dk_map->get_block_type(mapY,mapX+1)))
				if (lManager->getBlock(mapX+1,mapY)->isLow())
				{
					for (GLint i=9; i<12; i++)
					{
						RGT_S[i].create(getTexturePosInTextureAtlas((terrainType==CV_BLOCK_TYPE_LAVA_ID?LAVA_CLAIMED:WATER_CLAIMED),0));
					}
				}
				else
				{
					if (lManager->getBlockType(mapX+1,mapY)==CV_BLOCK_TYPE_ROCK_ID)
					{
						for (GLint i=9; i<12; i++)
						{
							RGT_S[i].create(getTexturePosInTextureAtlas(R_BL_N,0));
						}
					}
					else 
					{
						GLint water_earth_animation[]=
						{
							getTexturePosInTextureAtlas(WATER_EARTH0,0),
							getTexturePosInTextureAtlas(WATER_EARTH1,0),
							getTexturePosInTextureAtlas(WATER_EARTH2,0),
							getTexturePosInTextureAtlas(WATER_EARTH3,0),
							getTexturePosInTextureAtlas(WATER_EARTH4,0),
							getTexturePosInTextureAtlas(WATER_EARTH5,0),
							getTexturePosInTextureAtlas(WATER_EARTH6,0),
							getTexturePosInTextureAtlas(WATER_EARTH7,0)
						};

						if (terrainType==CV_BLOCK_TYPE_LAVA_ID)
						{
							for (GLint i=9; i<12; i++)
							{
								RGT_S[i].create(getTexturePosInTextureAtlas(LAVA_EARTH,0));
							}
						}
						else
						{
							for (GLint i=9; i<12; i++)
							{
								RGT_S[i].create(water_earth_animation,8,true,true);
								RGT_S[i].setAnimSpeed(WATER_EARTH_ANIM_SPEED);
							}
						}
					}			
				}
			}
			else
			{
				if (terrainType==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						RGT_S[i].create(getTexturePosInTextureAtlas(earth_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_ROCK_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						RGT_S[i].create(getTexturePosInTextureAtlas(rock_subtiles[i],0));
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GEM_ID)
				{
					GLint gem_animation[]=
					{
						getTexturePosInTextureAtlas(GEM0,0),
						getTexturePosInTextureAtlas(GEM1,0)
					};
					for (GLint i=0; i<12; i++)
					{
						RGT_S[i].create(gem_animation,GEM_ANIM_COUNT);
						RGT_S[i].setAnimSpeed(GEM_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						RGT_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),2);
					}
				}
				else if (terrainType==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation1[]=
					{
						getTexturePosInTextureAtlas(GOLD0,0),
						getTexturePosInTextureAtlas(GOLD1,0),
						getTexturePosInTextureAtlas(GOLD2,0),
						getTexturePosInTextureAtlas(GOLD3,0)
					};

					GLint gold_animation2[]=
					{
						getTexturePosInTextureAtlas(GOLD_0,0),
						getTexturePosInTextureAtlas(GOLD_1,0),
						getTexturePosInTextureAtlas(GOLD_2,0),
						getTexturePosInTextureAtlas(GOLD_3,0),
					};

					GLint gold_animation3[]=
					{
						getTexturePosInTextureAtlas(GOLD__0,0),
						getTexturePosInTextureAtlas(GOLD__1,0),
						getTexturePosInTextureAtlas(GOLD__2,0),
						getTexturePosInTextureAtlas(GOLD__3,0),
					};

					for (GLint i=0; i<12; i++)
					{
						if (rand()%5==0)
						{
							RGT_S[i].create(gold_animation1,GOLD_ANIM1_COUNT);
						}
						else
						{
							if (rand()%2==0)
							{
								RGT_S[i].create(gold_animation2,GOLD_ANIM2_COUNT);
							}
							else
							{
								RGT_S[i].create(gold_animation3,GOLD_ANIM3_COUNT);
							}
						}
						RGT_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}

					for (GLint i=9; i<12; i++)
					{
						RGT_S[i].create(getTexturePosInTextureAtlas(GEM_GOLD_LOW,0),4);
					}
				}

				if (rtype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_EARTH_ID)
				{
					for (GLint i=0; i<12; i++)
					{
						RGT_S[i].create(getTexturePosInTextureAtlas(EARTH_AT_LAVA,0));
					}
				}

				if (rtype==CV_BLOCK_TYPE_LAVA_ID && type==CV_BLOCK_TYPE_GOLD_ID)
				{
					GLint gold_animation4[]=
					{
						getTexturePosInTextureAtlas(GOLD_LAVA0,0),
						getTexturePosInTextureAtlas(GOLD_LAVA1,0),
						getTexturePosInTextureAtlas(GOLD_LAVA2,0),
						getTexturePosInTextureAtlas(GOLD_LAVA3,0),
					};

					for (GLint i=0; i<12; i++)
					{
						RGT_S[i].create(gold_animation4,GOLD_ANIM4_COUNT);
						RGT_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
					}
				}
			}
		}

		GLvoid CBlockManager::setTopWallCorner(CSubtileData SUB_DATA[], GLint pos)
		{
			if (pos==0 || pos==1 || pos==2)
			{
				if (utype==CV_BLOCK_TYPE_TEMPLE_ID)
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_PAINTING_TEMPLE_COLUMN_1_7,0));
				}
				else
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_CORNER,0));
				}
			}
			else if (pos==6 || pos==7 || pos==8)
			{
				if (dtype==CV_BLOCK_TYPE_TEMPLE_ID)
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_PAINTING_TEMPLE_COLUMN_1_7,0));
				}
				else
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_CORNER,0));
				}
			}
			else if (pos==0 || pos==3 || pos==6)
			{
				if (ltype==CV_BLOCK_TYPE_TEMPLE_ID)
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_PAINTING_TEMPLE_COLUMN_1_7,0));
				}
				else
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_CORNER,0));
				}
			}
			else if (pos==2 || pos==5 ||pos==8)
			{
				if (rtype==CV_BLOCK_TYPE_TEMPLE_ID)
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_PAINTING_TEMPLE_COLUMN_1_7,0));
				}
				else
				{
					SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_CORNER,0));
				}
			}
			else
			{
				SUB_DATA[pos].create(getTexturePosInTextureAtlas(WALL_CORNER,0));
			}
		}

		GLvoid CBlockManager::fillSubtiles_TOP(CBlock *block)
		{

			GLint	terrainType = block->getType();
			GLint	owner = block->getOwner();

			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			if (terrainType==CV_BLOCK_TYPE_WALL_ID)
			{
				GLint subtiles[8]; // max 8 subtiles can be edge ones
				byte set[]={0,0,0,0,1,0,0,0,0};

				// get the edge subtiles and correct if necesary
				GLint count = block->getEdgeSubtiles(subtiles);
			
				for (GLint i=0; i<count; i++)
				{
					// the corner ones
					if (subtiles[i]==0 || subtiles[i]==2 || subtiles[i]==6 || subtiles[i]==8)
					{
						if (subtiles[i]==0)
						{
							if (lManager->isSameTypeAndOwner(mapX,mapY-1,block) || lManager->isSameTypeAndOwner(mapX-1,mapY,block))
							{
								TOP_S[subtiles[i]].create(getTexturePosInTextureAtlas(WALL_EDGE,0));
							}
							else
							{
								setTopWallCorner(TOP_S,subtiles[i]);
							}
						}
						else if (subtiles[i]==2)
						{
							if (lManager->isSameTypeAndOwner(mapX,mapY-1,block) || lManager->isSameTypeAndOwner(mapX+1,mapY,block))
							{
								TOP_S[subtiles[i]].create(getTexturePosInTextureAtlas(WALL_EDGE,0));
							}
							else
							{
								setTopWallCorner(TOP_S,subtiles[i]);
							}
						}
						else if (subtiles[i]==6)
						{
							if (lManager->isSameTypeAndOwner(mapX,mapY+1,block) || lManager->isSameTypeAndOwner(mapX-1,mapY,block))
							{
								TOP_S[subtiles[i]].create(getTexturePosInTextureAtlas(WALL_EDGE,0));
							}
							else
							{
								setTopWallCorner(TOP_S,subtiles[i]);
							}
						}
						else if (subtiles[i]==8)
						{
							if (lManager->isSameTypeAndOwner(mapX,mapY+1,block) || lManager->isSameTypeAndOwner(mapX+1,mapY,block))
							{
								TOP_S[subtiles[i]].create(getTexturePosInTextureAtlas(WALL_EDGE,0));
							}
							else
							{
								setTopWallCorner(TOP_S,subtiles[i]);
							}
						}
						else
						{
							setTopWallCorner(TOP_S,subtiles[i]);
						}
						set[subtiles[i]]=1;
					}
					else if (subtiles[i]==1 || subtiles[i]==3 || subtiles[i]==5 || subtiles[i]==7)
					{
						TOP_S[subtiles[i]].create(getTexturePosInTextureAtlas(WALL_EDGE,0));
						//set_top_wall_corner(TOP_S,subtiles[i]);
						set[subtiles[i]]=1;
					}			
				}
				// the center one goes manualy
				TOP_S[4].create(getTexturePosInTextureAtlas(WALL_CENTER,owner));

				for (GLint i=0; i<9; i++)
				{
					if (set[i]==0)
					{
						// if the wall is not "alone" we show hidden areas as earth
						TOP_S[i].create(getTexturePosInTextureAtlas(WALL_CENTER,5));
					}
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_EARTH_ID || terrainType==CV_BLOCK_TYPE_ROCK_ID)
			{
				for (GLint i=0; i<9; i++)
				{
					TOP_S[i].create(getTexturePosInTextureAtlas(terrainType==CV_BLOCK_TYPE_EARTH_ID?EARTH_M:ROCK_M,0));
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_GEM_ID)
			{
				GLint gem_animation[]=
				{
					getTexturePosInTextureAtlas(GEM0,0),
					getTexturePosInTextureAtlas(GEM1,0)
				};
				for (GLint i=0; i<9; i++)
				{
					TOP_S[i].create(gem_animation,GEM_ANIM_COUNT);
					TOP_S[i].setAnimSpeed(GEM_ANIM_SPEED);
				}
			}
			else if (terrainType==CV_BLOCK_TYPE_GOLD_ID)
			{
				GLint gold_animation1[]=
				{
					getTexturePosInTextureAtlas(GOLD0,0),
					getTexturePosInTextureAtlas(GOLD1,0),
					getTexturePosInTextureAtlas(GOLD2,0),
					getTexturePosInTextureAtlas(GOLD3,0)
				};

				GLint gold_animation2[]=
				{
					getTexturePosInTextureAtlas(GOLD_0,0),
					getTexturePosInTextureAtlas(GOLD_1,0),
					getTexturePosInTextureAtlas(GOLD_2,0),
					getTexturePosInTextureAtlas(GOLD_3,0),
				};

				GLint gold_animation3[]=
				{
					getTexturePosInTextureAtlas(GOLD__0,0),
					getTexturePosInTextureAtlas(GOLD__1,0),
					getTexturePosInTextureAtlas(GOLD__2,0),
					getTexturePosInTextureAtlas(GOLD__3,0),
				};

				for (GLint i=0; i<9; i++)
				{
					if (rand()%5==0)
					{
						TOP_S[i].create(gold_animation1,GOLD_ANIM1_COUNT);
					}
					else
					{
						if (rand()%2==0)
						{
							TOP_S[i].create(gold_animation2,GOLD_ANIM2_COUNT);
						}
						else
						{
							TOP_S[i].create(gold_animation3,GOLD_ANIM3_COUNT);
						}
					}
					TOP_S[i].setAnimSpeed(GOLD_ANIM_SPEED);
				}
			}
		}

		GLvoid CBlockManager::fillSubtiles(game_objects::CBlock *block)
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			mapX = block->getLogicalPosition()[0];
			mapY = block->getLogicalPosition()[1];
			owner = block->getOwner();

			type=lManager->getBlockType(mapX,mapY);
			ltype=lManager->getBlockType(mapX-1,mapY);
			rtype=lManager->getBlockType(mapX+1,mapY);
			utype=lManager->getBlockType(mapX,mapY-1);
			ultype=lManager->getBlockType(mapX-1,mapY-1);
			urtype=lManager->getBlockType(mapX+1,mapY-1);
			dtype=lManager->getBlockType(mapX,mapY+1);
			drtype=lManager->getBlockType(mapX+1,mapY+1);

			/* 
				1. fill-in the *_S structure.
				2. read texture coordinates from *_S into block->textureCoordinates
					Important:	If this tile supports texture animation then 
								we have to do step 2 every animation update.
			*/
			
			fillSubtiles_FRONT(block);
			fillSubtiles_BACK(block);
			fillSubtiles_LEFT(block);
			fillSubtiles_RIGHT(block);
			fillSubtiles_TOP(block);

			bool isRoom = block->isRoom();

			if (isRoom)
			{						
				/* 
					Room type gets handled differently. Floor textures and other room 
					tile geometries get generated in RoomConstructor.
				*/	
				roomConstructor->createRoomData(block);
			}
			else
			{
				fillSubtiles_BOTTOM(block);					
			}

			// a fix for torch plate
			if (block->hasTorch())
			{
				GLint terrainType = block->getType();
				for (GLint i=0; i<12; i++)
				{
					if (terrainType==CV_BLOCK_TYPE_WALL_ID)
					{
						FNT_S[i].create(getTexturePosInTextureAtlas(normal_wall_no_edge[i],0));
						BCK_S[i].create(getTexturePosInTextureAtlas(normal_wall_no_edge[i],0));
						LFT_S[i].create(getTexturePosInTextureAtlas(normal_wall_no_edge[i],0));
						RGT_S[i].create(getTexturePosInTextureAtlas(normal_wall_no_edge[i],0));
					}

					if (i==4)
					{
						if (dtype!=CV_BLOCK_TYPE_LAVA_ID)
						{
							FNT_S[4].create(getTexturePosInTextureAtlas(terrainType==CV_BLOCK_TYPE_WALL_ID?WALL_TORCH_PLATE:EARTH_TORCH_PLATE,0));
						}

						if (utype!=CV_BLOCK_TYPE_LAVA_ID)
						{
							BCK_S[4].create(getTexturePosInTextureAtlas(terrainType==CV_BLOCK_TYPE_WALL_ID?WALL_TORCH_PLATE:EARTH_TORCH_PLATE,0));
						}

						if (rtype!=CV_BLOCK_TYPE_LAVA_ID)
						{
							LFT_S[4].create(getTexturePosInTextureAtlas(terrainType==CV_BLOCK_TYPE_WALL_ID?WALL_TORCH_PLATE:EARTH_TORCH_PLATE,0));		
						}

						if (ltype!=CV_BLOCK_TYPE_LAVA_ID)
						{
							RGT_S[4].create(getTexturePosInTextureAtlas(terrainType==CV_BLOCK_TYPE_WALL_ID?WALL_TORCH_PLATE:EARTH_TORCH_PLATE,0));
						}
					}
				}
			}

			if (!isRoom)
			{
				updateTextureCoordinates(block,CBlock::BFS_BOTTOM);
			}

			updateTextureCoordinates(block,CBlock::BFS_WATER_LAVA);
			updateTextureCoordinates(block,CBlock::BFS_FRONT);
			updateTextureCoordinates(block,CBlock::BFS_BACK);
			updateTextureCoordinates(block,CBlock::BFS_LEFT);
			updateTextureCoordinates(block,CBlock::BFS_RIGHT);
			updateTextureCoordinates(block,CBlock::BFS_TOP);
			
		}

		GLvoid CBlockManager::updateTextureCoordinates(CBlock *block, GLint face)
		{
			GLfloat *texCoords = block->getTextureCoordinates()[face];

			GLint	pos = 0;
			GLfloat subtileWidth = (GLfloat)textureAtlasSubtileSize/(GLfloat)textureAtlasWidth-BTM_S[0].getPixelWidth()*2.0f;
			GLfloat subtileHeight = -(GLfloat)textureAtlasSubtileSize/(GLfloat)textureAtlasHeight+BTM_S[0].getPixelHeight()*2.0f;

			vector2f	texCoord,
						texCoord_;

			if (face==CBlock::BFS_BOTTOM || face==CBlock::BFS_WATER_LAVA)
			{
				block->setNumberOfTextureFrames((CBlock::BLOCK_FACE_SELECTOR)face,BTM_S[4].getAnimCount());
				for (GLint x=2; x>=0; x--)
				{
					for (GLint y=0; y<3; y++)
					{
						for (GLint f=0; f<BTM_S[4].getAnimCount(); f++)
						{
							if (f>=4)
							{
								break;
							}
							GLint tPos = pos;

							BTM_S[3*y+x].setAnimPos(f);

							texCoords = block->getTextureCoordinates()[face+8*f];

							texCoord = BTM_S[3*y+x].getTextureAtlasPosition();

							texCoord_ = texCoord+vector2f(subtileWidth,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(0,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord;
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							pos=tPos;
						}
						pos+=8;
					}					
				}
			}
			else if (face==CBlock::BFS_TOP)
			{	
				block->setNumberOfTextureFrames((CBlock::BLOCK_FACE_SELECTOR)face,TOP_S[4].getAnimCount());
				for (GLint x=0; x<3; x++)
				{
					for (GLint y=0; y<3; y++)
					{						
						for (GLint f=0; f<TOP_S[4].getAnimCount(); f++)
						{
							if (f>=4)
							{
								break;
							}
							GLint tPos = pos;

							TOP_S[3*y+x].setAnimPos(f);

							texCoords = block->getTextureCoordinates()[face+8*f];

							texCoord = TOP_S[3*y+x].getTextureAtlasPosition();										

							texCoord_ = texCoord+vector2f(subtileWidth,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(0,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord;
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							pos=tPos;
						}						
						pos+=8;
					}
				}
			}
			else if (face==CBlock::BFS_FRONT)
			{
				block->setNumberOfTextureFrames((CBlock::BLOCK_FACE_SELECTOR)face,FNT_S[4].getAnimCount());
				for (GLint x=3; x>=0; x--)
				{
					for (GLint y=0; y<3; y++)
					{
						for (GLint f=0; f<FNT_S[4].getAnimCount(); f++)
						{
							if (f>=4)
							{
								break;
							}
							GLint tPos = pos;

							FNT_S[3*x+y].setAnimPos(f);

							texCoords = block->getTextureCoordinates()[face+8*f];

							texCoord = FNT_S[3*x+y].getTextureAtlasPosition();										

							texCoord_ = texCoord+vector2f(0,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(0,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							pos=tPos;
						}						
						pos+=8;
					}
				}
			}
			else if (face==CBlock::BFS_BACK)
			{
				block->setNumberOfTextureFrames((CBlock::BLOCK_FACE_SELECTOR)face,BCK_S[4].getAnimCount());
				for (GLint x=3; x>=0; x--)
				{
					for (GLint y=0; y<3; y++)
					{
						for (GLint f=0; f<BCK_S[4].getAnimCount(); f++)
						{
							if (f>=4)
							{
								break;
							}
							GLint tPos = pos;

							BCK_S[3*x+y].setAnimPos(f);

							texCoords = block->getTextureCoordinates()[face+8*f];
			
							texCoord = BCK_S[3*x+y].getTextureAtlasPosition();																																		

							texCoord_ = texCoord+vector2f(0,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(0,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							pos=tPos;
						}						
						pos+=8;
					}
				}
			}
			else if (face==CBlock::BFS_LEFT)
			{
				block->setNumberOfTextureFrames((CBlock::BLOCK_FACE_SELECTOR)face,LFT_S[4].getAnimCount());
				for (GLint x=3; x>=0; x--)
				{
					for (GLint y=0; y<3; y++)
					{
						for (GLint f=0; f<LFT_S[4].getAnimCount(); f++)
						{
							if (f>=4)
							{
								break;
							}
							GLint tPos = pos;

							LFT_S[3*x+y].setAnimPos(f);

							texCoords = block->getTextureCoordinates()[face+8*f];
							texCoord = LFT_S[3*x+y].getTextureAtlasPosition();																																		

							texCoord_ = texCoord+vector2f(0,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(0,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							pos=tPos;
						}						
						pos+=8;
					}
				}
			}
			else if (face==CBlock::BFS_RIGHT)
			{
				block->setNumberOfTextureFrames((CBlock::BLOCK_FACE_SELECTOR)face,RGT_S[4].getAnimCount());
				for (GLint x=3; x>=0; x--)
				{
					for (GLint y=0; y<3; y++)
					{
						for (GLint f=0; f<RGT_S[4].getAnimCount(); f++)
						{
							if (f>=4)
							{
								break;
							}
							GLint tPos = pos;

							RGT_S[3*x+y].setAnimPos(f);

							texCoords = block->getTextureCoordinates()[face+8*f];
							texCoord = RGT_S[3*x+y].getTextureAtlasPosition();																																		

							texCoord_ = texCoord+vector2f(0,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,subtileHeight);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(subtileWidth,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							texCoord_ = texCoord+vector2f(0,0);
							memcpy(texCoords+pos,&texCoord_[0],sizeof(GLfloat)*2);
							pos+=2;

							pos=tPos;
						}						
						pos+=8;
					}
				}
			}
		}

		bool CBlockManager::createGlobalDeformedMap()
		{
			GLfloat step_xz	= CV_BLOCK_WIDTH/(GLfloat)(CV_BLOCK_RESOLUTION_XZ-1),
					step_y	= CV_BLOCK_HEIGHT/(GLfloat)(CV_BLOCK_RESOLUTION_Y-1);

			globalDeformedMap = new vector3f**[CV_DEFORMED_CUBE_DEPTH];

			for (GLint z=0; z<CV_DEFORMED_CUBE_DEPTH; z++)
			{
				globalDeformedMap[z] = new vector3f*[CV_DEFORMED_CUBE_HEIGHT];
				for (GLint y=0; y<CV_DEFORMED_CUBE_HEIGHT; y++)
				{
					globalDeformedMap[z][y] = new vector3f[CV_DEFORMED_CUBE_WIDTH];

					for (GLint x=0; x<CV_DEFORMED_CUBE_WIDTH; x++)
					{
						GLfloat py = y<=5||y>6?(GLfloat)y*step_y:3.0f*step_y+(GLfloat)y*step_y*0.5f;						

						//globalDeformedMap[z][y][x] = calculatePerlin3D(vector3f((GLfloat)x*step_xz,py,(GLfloat)z*step_xz));
						globalDeformedMap[z][y][x] = vector3f((GLfloat)x*step_xz,(GLfloat)y*step_y,(GLfloat)z*step_xz);

						/*if (y>5)
						{
							globalDeformedMap[z][y][x][1]+=(GLfloat)(rand()%100-50)/1000.0f;
						}*/
					}
				}
			}

			// should make some error checking when NEW-ing
			return true;
		}

		bool CBlockManager::update()
		{
			return true;
		}

		bool CBlockManager::shutdown()
		{
			if (globalDeformedMap)
			{
				for (GLint z=0; z<CV_DEFORMED_CUBE_DEPTH; z++)
				{					
					for (GLint y=0; y<CV_DEFORMED_CUBE_HEIGHT; y++)
					{
						delete [] globalDeformedMap[z][y];
					}
					delete [] globalDeformedMap[z];
				}
				delete [] globalDeformedMap;

				globalDeformedMap = NULL;
			}

			delete roomConstructor;

			return true;
		}

		vector3f CBlockManager::calculatePerlin3D(vector3f vec)
		{
			GLfloat details = 8.0f;	// bigger the value more detailed the terrain becomes
			GLfloat scale = 25.0f;	// bigger the scale more deformation gets applied
			GLfloat noise = (GLfloat)PerlinNoise3D(vec[0]*details,vec[1]*details,vec[2]*details,2.0,2.0,1)/scale;
			vec+=vector3f(noise,noise,noise);
			return vec;
		}

		GLvoid CBlockManager::setupGFXAndCeiling(CBlock *block)
		{
			fillSubtiles(block);

			vector2i lPos = block->getLogicalPosition();

			// now lets setup ceiling
			if (block->isLow())
			{
				// no need to call calculateBlockCeilingHeight since it has alread been called in LevelManager
				vector3f ceilingPatch[4][4];
				calculateCeiling(lPos,ceilingPatch);
				
				// write vertics into blocks vertex buffers
				GLfloat		**vertices = block->getVertices();
				GLfloat		**texCoords = block->getTextureCoordinates();

				GLint	vPos = 0,
						tPos = 0;

				GLint face = CBlock::BFS_CEILING;				

				GLfloat pxW = (1.0f/(GLfloat)textureAtlasWidth)*pixelWidth;
				GLfloat pxH = (1.0f/(GLfloat)textureAtlasHeight)*pixelHeight;				

				GLfloat subTexW = (GLfloat)textureAtlasSubtileSize/(GLfloat)textureAtlasWidth - pxW*2.0f;
				GLfloat subTexH = (GLfloat)textureAtlasSubtileSize/(GLfloat)textureAtlasHeight - pxH*2.0f;

				GLint textureAtlasPos = 207; // hardcoded, put in config?

				GLint texturesPerRow = (GLint)textureAtlasWidth/(GLint)textureAtlasSubtileSize;

				GLfloat ceilingX = (GLfloat)((textureAtlasPos%texturesPerRow)*textureAtlasSubtileSize)/textureAtlasWidth+pxW;
				GLfloat ceilingY = (GLfloat)(1.0f-(textureAtlasPos/texturesPerRow)*textureAtlasSubtileSize)/textureAtlasHeight-pxH;
		
				for (int y=0; y<CV_BLOCK_RESOLUTION_XZ-1; y++)
				{
					for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
					{	
						CConversions::vec3fToFloat_p(ceilingPatch[y][x],vertices[face]+vPos);
						CConversions::vec2fToFloat_p(vector2f(ceilingX,ceilingY),texCoords[face]+tPos);
						tPos+=2;
						vPos+=3;

						CConversions::vec3fToFloat_p(ceilingPatch[y][x+1],vertices[face]+vPos);
						CConversions::vec2fToFloat_p(vector2f(ceilingX+subTexW,ceilingY),texCoords[face]+tPos);
						tPos+=2;
						vPos+=3;

						CConversions::vec3fToFloat_p(ceilingPatch[y+1][x+1],vertices[face]+vPos);
						CConversions::vec2fToFloat_p(vector2f(ceilingX+subTexW,ceilingY-subTexH),texCoords[face]+tPos);
						tPos+=2;
						vPos+=3;

						CConversions::vec3fToFloat_p(ceilingPatch[y+1][x],vertices[face]+vPos);
						CConversions::vec2fToFloat_p(vector2f(ceilingX,ceilingY-subTexH),texCoords[face]+tPos);
						tPos+=2;
						vPos+=3;
					}
				}
			}
		}

		GLvoid CBlockManager::reloadSubLevelVertices(CBlock *block)
		{
			vector2i	logicalPosition = block->getLogicalPosition();
			GLint		type = block->getType();
			GLfloat		**vertices = block->getVertices();
			GLfloat		**texCoords = block->getTextureCoordinates();			

			GLint gDEfCx = logicalPosition[0] * (CV_BLOCK_RESOLUTION_XZ-1);
			GLint gDEfCz = logicalPosition[1] * (CV_BLOCK_RESOLUTION_XZ-1);

			GLint level = (type == CV_BLOCK_TYPE_LAVA_ID || type == CV_BLOCK_TYPE_WATER_ID)?1:0;

			GLint face = CBlock::BFS_WATER_LAVA;
			GLint vPos = 0;
			for (int y=0; y<CV_BLOCK_RESOLUTION_XZ-1; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{					 
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][0][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][0][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][0][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][0][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;
				}
			}
		}

		GLvoid CBlockManager::generateBlockGeometry(CBlock *block)
		{
			setupGFXAndCeiling(block);

			/*
				top:
						D--C
						|  |
						A--B

				bottom:
						H--G
						|  |
						E--F				
			*/	

			vector2i	logicalPosition = block->getLogicalPosition();
			GLint		type = block->getType();
			GLfloat		**vertices = block->getVertices();
			GLfloat		**texCoords = block->getTextureCoordinates();			

			GLint gDEfCx = logicalPosition[0] * (CV_BLOCK_RESOLUTION_XZ-1);
			GLint gDEfCz = logicalPosition[1] * (CV_BLOCK_RESOLUTION_XZ-1);

			GLint level = (type == CV_BLOCK_TYPE_LAVA_ID || type == CV_BLOCK_TYPE_WATER_ID)?1:0;

			GLint face = CBlock::BFS_FRONT;
			GLint vPos = 0;
			for (int y=1-level; y<CV_BLOCK_RESOLUTION_Y-level; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{		
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+(CV_BLOCK_RESOLUTION_XZ-1)][y][gDEfCx+x],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+(CV_BLOCK_RESOLUTION_XZ-1)][y][gDEfCx+x+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+(CV_BLOCK_RESOLUTION_XZ-1)][y+1][gDEfCx+x+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+(CV_BLOCK_RESOLUTION_XZ-1)][y+1][gDEfCx+x],vertices[face]+vPos);
					vPos+=3;
				}
			}

			face = CBlock::BFS_BACK;
			vPos = 0;
			for (int y=1-level; y<CV_BLOCK_RESOLUTION_Y-level; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{	
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz][y][gDEfCx+x],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz][y][gDEfCx+x+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz][y+1][gDEfCx+x+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz][y+1][gDEfCx+x],vertices[face]+vPos);
					vPos+=3;
				}
			}

			face = CBlock::BFS_LEFT;
			vPos = 0;
			for (int y=1-level; y<CV_BLOCK_RESOLUTION_Y-level; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{					 
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][y][gDEfCx],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][y][gDEfCx],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][y+1][gDEfCx],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][y+1][gDEfCx],vertices[face]+vPos);
					vPos+=3;
				}
			}

			face = CBlock::BFS_RIGHT;
			vPos = 0;
			for (int y=1-level; y<CV_BLOCK_RESOLUTION_Y-level; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{					 
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][y][gDEfCx+(CV_BLOCK_RESOLUTION_XZ-1)],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][y][gDEfCx+(CV_BLOCK_RESOLUTION_XZ-1)],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][y+1][gDEfCx+(CV_BLOCK_RESOLUTION_XZ-1)],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][y+1][gDEfCx+(CV_BLOCK_RESOLUTION_XZ-1)],vertices[face]+vPos);
					vPos+=3;
				}
			}

			face = CBlock::BFS_TOP;
			vPos = 0;
			for (int y=0; y<CV_BLOCK_RESOLUTION_XZ-1; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{	
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][CV_BLOCK_RESOLUTION_Y-level][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][CV_BLOCK_RESOLUTION_Y-level][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][CV_BLOCK_RESOLUTION_Y-level][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][CV_BLOCK_RESOLUTION_Y-level][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;
				}
			}
			
			face = CBlock::BFS_BOTTOM;
			vPos = 0;
			for (int y=0; y<CV_BLOCK_RESOLUTION_XZ-1; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{	
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][1][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][1][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][1][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][1][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;
				}
			}

			face = CBlock::BFS_WATER_LAVA;
			vPos = 0;
			for (int y=0; y<CV_BLOCK_RESOLUTION_XZ-1; y++)
			{
				for (int x=0; x<CV_BLOCK_RESOLUTION_XZ-1; x++)
				{					 
					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][0][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][0][gDEfCx+y],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x+1][0][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;

					CConversions::vec3fToFloat_p(globalDeformedMap[gDEfCz+x][0][gDEfCx+y+1],vertices[face]+vPos);
					vPos+=3;
				}
			}
		}			

		#define SET_CSR(i,state) csr[i]=((csr[i]==CSR_NOT_EDGE)?state:csr[i]);

		GLvoid CBlockManager::getEdgeCeilingSubPatches(CEILING_SUBPATCH_RELATION csr[], vector2i lPos)
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			GLint dx = lPos[0];
			GLint dy = lPos[1]; 

			GLint height=lManager->getBlock(dx,dy)->getCeilingHeight();
			GLint lheight=lManager->getBlock(dx-1,dy)->getCeilingHeight();
			GLint rheight=lManager->getBlock(dx+1,dy)->getCeilingHeight();
			GLint theight=lManager->getBlock(dx,dy-1)->getCeilingHeight();
			GLint bheight=lManager->getBlock(dx,dy+1)->getCeilingHeight();
			GLint rbheight=lManager->getBlock(dx+1,dy+1)->getCeilingHeight();
			GLint lbheight=lManager->getBlock(dx-1,dy+1)->getCeilingHeight();
			GLint rtheight=lManager->getBlock(dx+1,dy-1)->getCeilingHeight();
			GLint ltheight=lManager->getBlock(dx-1,dy-1)->getCeilingHeight();

			for (GLint i=0; i<9; i++)
			{
				csr[i]=CSR_NOT_EDGE;
			}

			if (height>lheight)
			{
				SET_CSR(0,CSR_LOWER)
				SET_CSR(3,CSR_LOWER)
				SET_CSR(6,CSR_LOWER)
			}
			else if (height<lheight)
			{
				SET_CSR(0,CSR_HIGHER)
				SET_CSR(3,CSR_HIGHER)
				SET_CSR(6,CSR_HIGHER)
			}

			if (height>rheight)
			{
				SET_CSR(2,CSR_LOWER)
				SET_CSR(5,CSR_LOWER)
				SET_CSR(8,CSR_LOWER)
			}
			else if (height<rheight)
			{
				SET_CSR(2,CSR_HIGHER)
				SET_CSR(5,CSR_HIGHER)
				SET_CSR(8,CSR_HIGHER)
			}

			if (height>theight)
			{
				SET_CSR(0,CSR_LOWER)
				SET_CSR(1,CSR_LOWER)
				SET_CSR(2,CSR_LOWER)
			}
			else if (height<theight)
			{
				SET_CSR(0,CSR_HIGHER)
				SET_CSR(1,CSR_HIGHER)
				SET_CSR(2,CSR_HIGHER)
			}

			if (height>bheight)
			{
				SET_CSR(6,CSR_LOWER)
				SET_CSR(7,CSR_LOWER)
				SET_CSR(8,CSR_LOWER)
			}
			else if (height<bheight)
			{
				SET_CSR(6,CSR_HIGHER)
				SET_CSR(7,CSR_HIGHER)
				SET_CSR(8,CSR_HIGHER)
			}

			if (height>ltheight)
			{
				SET_CSR(0,CSR_LOWER)
			}
			else if (height<ltheight)
			{
				SET_CSR(0,CSR_HIGHER)
			}

			if (height>rtheight)
			{
				SET_CSR(2,CSR_LOWER)
			}
			else if (height<rtheight)
			{
				SET_CSR(2,CSR_HIGHER)
			}

			if (height>lbheight)
			{
				SET_CSR(6,CSR_LOWER)
			}
			else if (height<lbheight)
			{
				SET_CSR(6,CSR_HIGHER)
			}

			if (height>rbheight)
			{
				SET_CSR(8,CSR_LOWER)
			}
			else if (height<rbheight)
			{
				SET_CSR(8,CSR_HIGHER)
			}
		}

		bool CBlockManager::isMaxHeightEdgeCeilingPatch(bool edges[], vector2i lPos)
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			GLint dx = lPos[0];
			GLint dy = lPos[1]; 

			GLint height=lManager->getBlock(lPos)->getCeilingHeight();

			if (height==MAX_CEILING_HEIGHT)
			{
				GLint lheight=lManager->getBlock(dx-1,dy)->getCeilingHeight();
				GLint rheight=lManager->getBlock(dx+1,dy)->getCeilingHeight();
				GLint theight=lManager->getBlock(dx,dy-1)->getCeilingHeight();
				GLint bheight=lManager->getBlock(dx,dy+1)->getCeilingHeight();
				GLint rbheight=lManager->getBlock(dx+1,dy+1)->getCeilingHeight();
				GLint lbheight=lManager->getBlock(dx-1,dy+1)->getCeilingHeight();
				GLint rtheight=lManager->getBlock(dx+1,dy-1)->getCeilingHeight();
				GLint ltheight=lManager->getBlock(dx-1,dy-1)->getCeilingHeight();

				if (height>theight)
				{
					edges[0]=edges[1]=edges[2]=true;
				}

				if (height>bheight)
				{
					edges[6]=edges[7]=edges[8]=true;
				}

				if (height>lheight)
				{
					edges[0]=edges[3]=edges[6]=true;
				}

				if (height>rheight)
				{
					edges[2]=edges[5]=edges[8]=true;
				}

				if (height>ltheight)
				{
					edges[0]=true;
				}

				if (height>rtheight)
				{
					edges[2]=true;
				}

				if (height>lbheight)
				{
					edges[6]=true;
				}

				if (height>rbheight)
				{
					edges[8]=true;
				}

				return true;
			}
			else
			{
				return false;
			}
		}

		vector3f CBlockManager::getGlobalDeformedMapPoint(GLint cx, GLint cy, GLint dx, GLint dy, GLint dz)
		{
			return globalDeformedMap[cy*3+dz][dy][cx*3+dx];
		}

		GLvoid CBlockManager::setGlobalDeformedMapPoint(GLint cx, GLint cy, GLint dx, GLint dy, GLint dz, GLfloat ny)
		{
			globalDeformedMap[cy*3+dz][dy][cx*3+dx][1]=ny;
		}

		#define LOW_3(z,x) getGlobalDeformedMapPoint(dx,dy,x,height*3+5-3,z);
		#define LOW_2(z,x) getGlobalDeformedMapPoint(dx,dy,x,height*3+5-2,z);
		#define LOW_1(z,x) getGlobalDeformedMapPoint(dx,dy,x,height*3+5-1,z);

		GLvoid CBlockManager::calculateCeiling(cml::vector2i lPos, vector3f ceilingPatch[4][4])
		{
			CLevelManager *lManager = CV_GAME_MANAGER->getLevelManager();

			GLint dx = lPos[0];
			GLint dy = lPos[1]; 

			GLint height=lManager->getBlock(dx,dy)->getCeilingHeight();
			GLint lheight=lManager->getBlock(dx-1,dy)->getCeilingHeight();
			GLint rheight=lManager->getBlock(dx+1,dy)->getCeilingHeight();
			GLint theight=lManager->getBlock(dx,dy-1)->getCeilingHeight();
			GLint bheight=lManager->getBlock(dx,dy+1)->getCeilingHeight();
			GLint rbheight=lManager->getBlock(dx+1,dy+1)->getCeilingHeight();
			GLint lbheight=lManager->getBlock(dx-1,dy+1)->getCeilingHeight();
			GLint rtheight=lManager->getBlock(dx+1,dy-1)->getCeilingHeight();
			GLint ltheight=lManager->getBlock(dx-1,dy-1)->getCeilingHeight();

			for (GLint x=0; x<4; x++)
			{
				for (GLint z=0; z<4; z++)
				{
					ceilingPatch[z][x]=getGlobalDeformedMapPoint(dx,dy,x,height*3+5,z);
				}
			}

			CEILING_SUBPATCH_RELATION csr[9];
			getEdgeCeilingSubPatches(csr,lPos);
			
			if (csr[0]==CSR_LOWER)
			{
				ceilingPatch[0][0]=LOW_3(0,0)
				ceilingPatch[1][1]=LOW_2(1,1)

				if (csr[1]==CSR_LOWER)
				{
					ceilingPatch[0][1]=LOW_3(0,1)
				}
				else
				{
					ceilingPatch[0][1]=LOW_2(0,1)
				}

				if (csr[3]==CSR_LOWER)
				{
					ceilingPatch[1][0]=LOW_3(1,0)
				}
				else
				{
					ceilingPatch[1][0]=LOW_2(1,0)
				}
			}
			else if (csr[0]==CSR_HIGHER)
			{
				ceilingPatch[1][1]=LOW_1(1,1)

				if (csr[1]!=CSR_HIGHER)
				{
					ceilingPatch[0][1]=LOW_1(0,1)
				}

				if (csr[3]!=CSR_HIGHER)
				{
					ceilingPatch[1][0]=LOW_1(1,0)
				}
			}

			if (csr[6]==CSR_LOWER)
			{
				ceilingPatch[3][0]=LOW_3(3,0)
				ceilingPatch[2][1]=LOW_2(2,1)

				if (csr[3]==CSR_LOWER)
				{
					ceilingPatch[2][0]=LOW_3(2,0)
				}
				else
				{
					ceilingPatch[2][0]=LOW_2(2,0)
				}

				if (csr[7]==CSR_LOWER)
				{
					ceilingPatch[3][1]=LOW_3(3,1)
				}
				else
				{
					ceilingPatch[3][1]=LOW_2(3,1)
				}
			}
			else if (csr[6]==CSR_HIGHER)
			{
				ceilingPatch[2][1]=LOW_1(2,1)

				if (csr[3]!=CSR_HIGHER)
				{
					ceilingPatch[2][0]=LOW_1(2,0)
				}

				if (csr[7]!=CSR_HIGHER)
				{
					ceilingPatch[3][1]=LOW_1(3,1)
				}
			}

			if (csr[2]==CSR_LOWER)
			{
				ceilingPatch[0][3]=LOW_3(0,3)
				ceilingPatch[1][2]=LOW_2(1,2)

				if (csr[1]==CSR_LOWER)
				{
					ceilingPatch[0][2]=LOW_3(0,2)
				}
				else
				{
					ceilingPatch[0][2]=LOW_2(0,2)
				}

				if (csr[5]==CSR_LOWER)
				{
					ceilingPatch[1][3]=LOW_3(1,3)
				}
				else
				{
					ceilingPatch[1][3]=LOW_2(1,3)
				}
			}
			else if (csr[2]==CSR_HIGHER)
			{
				ceilingPatch[1][2]=LOW_1(1,2)
			
				if (csr[1]!=CSR_HIGHER)
				{
					ceilingPatch[0][2]=LOW_1(0,2)
				}

				if (csr[5]!=CSR_HIGHER)
				{
					ceilingPatch[1][3]=LOW_1(1,3)
				}
			}

			if (csr[8]==CSR_LOWER)
			{
				ceilingPatch[3][3]=LOW_3(3,3)
				ceilingPatch[2][2]=LOW_2(2,2)

				if (csr[7]==CSR_LOWER)
				{
					ceilingPatch[3][2]=LOW_3(3,2)
				}
				else
				{
					ceilingPatch[3][2]=LOW_2(3,2)
				}

				if (csr[5]==CSR_LOWER)
				{
					ceilingPatch[2][3]=LOW_3(2,3)
				}
				else
				{
					ceilingPatch[2][3]=LOW_2(2,3)
				}
			}
			else if (csr[8]==CSR_HIGHER)
			{
				ceilingPatch[2][2]=LOW_1(2,2)

				if (csr[5]!=CSR_HIGHER)
				{
					ceilingPatch[2][3]=LOW_1(2,3)
				}

				if (csr[7]!=CSR_HIGHER)
				{
					ceilingPatch[3][2]=LOW_1(3,2)
				}
			}

			// fix the top ceiling patch problem!
			bool edges[]={false,false,false,false,false,false,false,false,false};
			bool is_edge=isMaxHeightEdgeCeilingPatch(edges,lPos);
			if (is_edge)
			{

				if (edges[1])
				{
					ceilingPatch[2][2]=LOW_1(2,2)
					ceilingPatch[2][1]=LOW_1(2,1)
				}

				if (edges[7])
				{
					ceilingPatch[1][2]=LOW_1(1,2)
					ceilingPatch[1][1]=LOW_1(1,1)
				}

				if (edges[3])
				{
					ceilingPatch[1][2]=LOW_1(1,2)
					ceilingPatch[2][2]=LOW_1(2,2)
				}

				if (edges[5])
				{
					ceilingPatch[1][1]=LOW_1(1,1)
					ceilingPatch[2][1]=LOW_1(2,1)
				}
			}

			/*if (is_door(dk_map[y][x].type)) tmp
			{
				ceilingPatch[1][1]=LOW_3(1,1)
				ceilingPatch[2][1]=LOW_3(2,1)
				ceilingPatch[1][2]=LOW_3(1,2)
				ceilingPatch[2][2]=LOW_3(2,2)
			}*/
		}
	};
};