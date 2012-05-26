#include <iostream>
#include <fstream>
#include "TextureLoader.h"
#include <vector>

using namespace std;
using namespace loaders;

#define MAX_ENT 512

struct subAtlasData
{
	string name;

	int owner,
		new_atlas_pos,	// newly calculated value
		in_game_pos;

};

vector<subAtlasData> mapSubtiles;
vector<subAtlasData>::iterator msIter;

void readSubtilePosFile(char *file)
{
	ifstream atlas;

	atlas.open(file,ios_base::in);

	char	line[256],
			sub_name[128];

	int owner,
		atlas_pos,	// this one needs recalculating
		in_game_pos;

	while (atlas.good())
	{
		atlas.getline(line,256,'\n');

		if (line[0]=='#')
		{
			continue;
		}

		// sample input line: CLAIMED_LAND 0 272 11

		sscanf(line,"%s %d %d %d",sub_name,&owner,&atlas_pos,&in_game_pos);

		subAtlasData sData;

		sData.name = string(sub_name);
		sData.owner = owner;
		sData.new_atlas_pos = atlas_pos;
		sData.in_game_pos = in_game_pos;

		mapSubtiles.push_back(sData);
	}

	atlas.close();
}

void writeSubtilePosFile(char *file)
{
	ofstream atlas;

	atlas.open(file,ios_base::out);

	char line[256];

	for (msIter = mapSubtiles.begin(); msIter != mapSubtiles.end(); msIter++)
	{
		subAtlasData sData = *msIter;

		int count = sprintf(line,"%s %d %d %d\n",sData.name.c_str(),sData.owner,sData.new_atlas_pos,sData.in_game_pos);

		atlas.write(line,count);
	}
	

	atlas.close();
}

int main()
{
	/* 
		Read texture config file (*.tfc). The file has the following structure:

		atlas_file_name
		new_atlas_file_name
		atlas_width
		atlas_height
		subtile_size(n^2)
		max_new_width
		max_new_height
	*/

	ifstream listFile;

	listFile.open("data/import.tfc");

	CImage atlasTex;

	int atlas_width = 0,
		atlas_height = 0,
		subtile_size = 0,
		max_new_width = 0,
		max_new_height = 0;

	char	subtile_database_name[MAX_ENT],	
			subtile_database_name_new[MAX_ENT],	
			atlas_name[MAX_ENT],
			new_atlas_name[MAX_PATH];

	// read texture atlas info
	char line[MAX_ENT];

	listFile.getline(line,MAX_ENT,'\n');
	strcpy(subtile_database_name,line);

	listFile.getline(line,MAX_ENT,'\n');
	strcpy(subtile_database_name_new,line);

	listFile.getline(line,MAX_ENT,'\n');
	strcpy(atlas_name,line);
	HRESULT atl = atlasTex.Load(line);

	listFile.getline(line,MAX_PATH,'\n');
	strcpy(new_atlas_name,line);

	listFile.getline(line,MAX_ENT,'\n');
	atlas_width = atoi(line);

	listFile.getline(line,MAX_ENT,'\n');
	atlas_height = atoi(line);

	listFile.getline(line,MAX_ENT,'\n');
	subtile_size = atoi(line);

	listFile.getline(line,MAX_ENT,'\n');
	max_new_width = atoi(line);

	listFile.getline(line,MAX_ENT,'\n');
	max_new_height = atoi(line);

	// restructure atlas

	// 1. calculate if it is even possible to restructure atlas to fit into new max dimensions

	int all_subtiles = (atlas_width/subtile_size)*(atlas_height/subtile_size);

	int subtiles_in_width = max_new_width/subtile_size;

	int remainig_subtiles_height = all_subtiles/subtiles_in_width;

	if (remainig_subtiles_height*subtile_size>max_new_height)
	{
		cout << "Limitations are too severe!" << endl;
		return -1;
	}

	cout << "Transformation possible!" << endl;

	// 2. do the actual transformation in 2 steps: - image transform, - subtile positions file transform

	// load subtile_database_name
	//readSubtilePosFile(subtile_database_name);

	CImage newAtlas;

	newAtlas.Create(max_new_width,max_new_height,24);

	long oldCount = 0;

	int oldPerWidth = atlas_width/subtile_size;
	int oldPerHeight = atlas_height/subtile_size;

	cout << "Transforming..." << endl;
	for (int y=0; y<remainig_subtiles_height; y++)
	{
		for (int x=0; x<subtiles_in_width; x++)
		{
			int xPixelPosNew = x*subtile_size;
			int yPixelPosNew = y*subtile_size;

			int xPixelPosOld = (oldCount%oldPerWidth)*subtile_size;
			int yPixelPosOld = (oldCount/oldPerWidth)*subtile_size;

			int oldSubtilePos = yPixelPosOld/subtile_size*oldPerWidth+xPixelPosOld/subtile_size;
			int newSubtilePos = yPixelPosNew/subtile_size*subtiles_in_width+xPixelPosNew/subtile_size;

			oldCount++;

			atlasTex.BitBlt(newAtlas.GetDC(),xPixelPosNew,yPixelPosNew,subtile_size,subtile_size,xPixelPosOld,yPixelPosOld);
			newAtlas.ReleaseDC();
		}
	}

	cout << "Saving new atlas..." << endl;
	newAtlas.Save(new_atlas_name);

	// save subtile info
	//writeSubtilePosFile(subtile_database_name_new);

	cout << "Done!" << endl;

	return 0;
}