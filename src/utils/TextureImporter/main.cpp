#include <iostream>
#include <fstream>
#include "TextureLoader.h"

using namespace std;
using namespace loaders;

#define MAX_ENT 512
#define MAX_TEX 256

int main()
{
	/* 
		Read texture config file (*.tfc). The file has the following structure:

		atlas_file_name
		new_atlas_file_name
		atlas_width
		atlas_height
		subtile_size(n^2)
		image_0_file_name atlas_subtile_index texture_subtile_index texture_index
		image_1_file_name atlas_subtile_index texture_subtile_index texture_index
		.
		.
		.
		image_n_file_name atlas_subtile_index texture_subtile_index
	*/

	ifstream listFile;

	listFile.open("data/import.tfc");

	CImage cimg;

	CImage atlasTex;
	CImage otherTex[MAX_TEX];

	int atlas_width = 0,
		atlas_height = 0,
		subtile_size = 0,
		count = -1,
		l_texs = 0;

	char	atlas_name[MAX_ENT],
			new_atlas_name[MAX_PATH];

	int atlas_index[MAX_ENT],
		texture_index[MAX_ENT],
		used_textures[MAX_ENT];

	bool loaded[MAX_ENT];

	for (int i=0; i<MAX_ENT; i++)
	{
		used_textures[i] = -1;
		loaded[i] = false;
	}

	// read textures
	while (listFile.good())
	{
		char line[MAX_ENT];
		listFile.getline(line,MAX_ENT,'\n');

		if (count==-1)
		{
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
		}
		else
		{
			char name[64];
			sscanf(line,"%s %d %d %d",&name,&atlas_index[count],&texture_index[count],&used_textures[count]);

			if (!loaded[used_textures[count]])
			{
				loaded[l_texs] = true;
				otherTex[l_texs++].Load(name);
			}
		}
		count++;
	}	

	// now we have all data, let's create the atlas
	for (int i=0; i<count; i++)
	{
		// calc x,y of atlas
		int a_col, a_x, a_y;
		a_col = atlasTex.GetWidth()/subtile_size;
		a_x = (atlas_index[i] % a_col)*subtile_size;
		a_y = (atlas_index[i] / a_col)*subtile_size;

		// calc x,y of texture
		int t_col, t_x, t_y;
		t_col = otherTex[used_textures[i]].GetWidth()/subtile_size;
		t_x = (texture_index[i] % t_col)*subtile_size;
		t_y = (texture_index[i] / t_col)*subtile_size;

		otherTex[used_textures[i]].BitBlt(atlasTex.GetDC(),a_x,a_y,subtile_size,subtile_size,t_x,t_y);
		atlasTex.ReleaseDC();

		/*for (int y=0; y<subtile_size; y++)
		{
			for (int x=0; x<subtile_size; x++)
			{
				atlasTex.SetPixel(a_x+x,a_y+y,otherTex[used_textures[i]].GetPixel(t_x+x,t_y+y));
			}
		}*/
		cout << "Finished " << i+1 << " of " << count << endl;
	}

	cout << "Saving atlas..." << endl;
	atlasTex.Save(new_atlas_name);
	//atlasTex.ReleaseDC();

	return 0;
}