#include <iostream>
#include <fstream>
#include "TextureLoader.h"

using namespace std;
using namespace loaders;


int main()
{
	CImage atlasTex;

	atlasTex.Load("../../data/resources/textures/atlas4_COLOR_HI.bmp");

	int splits = atlasTex.GetHeight() / 1024;

	printf("Extracting %d tiles.\n",splits+1);

	int remaps[] = {0,2,4,6,8,5,3,7,1};

	for (int i=0; i<=splits; i++)
	{
		CImage tile;

		tile.Create(1024,1024,32,0);

		HDC hdc = tile.GetDC();

		RECT destRect;

		destRect.left = destRect.top = 0;
		destRect.right = 1024;
		destRect.bottom = 1024/(i==splits?2:1);

		POINT srcPt;
		srcPt.x = 0;
		srcPt.y = i*1024;

		atlasTex.BitBlt(hdc,destRect,srcPt);

		tile.ReleaseDC();

		char buff[32];
		sprintf(buff,"tile%d.bmp",remaps[i]);
		printf("%s\n",buff);

		tile.Save(buff);
	}
	printf("Finished!\n");

	return 0;
}