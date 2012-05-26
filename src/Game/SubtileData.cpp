#include "commons.h"
#include "SubtileData.h"
#include "settings.h"
#include "SettingsManager.h"

using namespace std;
using namespace cml;
using namespace game_utils::managers;

namespace game_utils
{
	bool CSubtileData::settingsRead = false;

	GLfloat CSubtileData::textureAtlasWidth = 1.0f;
	GLfloat CSubtileData::textureAtlasHeight = 1.0f;
	GLfloat CSubtileData::textureAtlasSubtileSize = 1000.0f;
	GLfloat CSubtileData::textureAtlasPixelWidth = 1000000.0f;
	GLfloat CSubtileData::textureAtlasPixelHeight = 100000.0f;

	CSubtileData::CSubtileData(): animPos(0), animCount(1), animSpeed(0.0f), animChange(1.0f), reset(false), inited(false)
	{			
	}

	CSubtileData::~CSubtileData()
	{
	}

	GLvoid CSubtileData::init()
	{
		if (!settingsRead)
		{
			// read texture atlas info from settings
			CSettingsManager *sManager = game_utils::CV_GAME_MANAGER->getSettingsManager();			

			// read texture quality then proper options
			string textureQuality = sManager->getSetting_String(CV_SETTINGS_TEXTURE_QUALITY);

			textureAtlasWidth = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_WIDTH)+"_"+textureQuality);
			textureAtlasHeight = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_HEIGHT)+"_"+textureQuality);
			textureAtlasSubtileSize = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_SUBTILE_SIZE)+"_"+textureQuality);
			textureAtlasPixelWidth = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_PIXEL_WIDTH)+"_"+textureQuality);
			textureAtlasPixelHeight = sManager->getSetting_Float(string(CV_SETTINGS_TEXTURE_ATLAS_PIXEL_HEIGHT)+"_"+textureQuality);
			settingsRead = true;
		}

		if (!inited)
		{
			init_(textureAtlasWidth,textureAtlasHeight,textureAtlasSubtileSize,textureAtlasPixelWidth,textureAtlasPixelHeight);
		}
	}

	GLvoid CSubtileData::init_(GLfloat texAtlW, GLfloat texAtlH, GLfloat texAtlSubSize, GLfloat pixelWidth, GLfloat pixelHeight)
	{
		this->texAtlW=(GLint)texAtlW;
		this->texAtlH=(GLint)texAtlH;
		this->texAtlSubSize=(GLint)texAtlSubSize;

		texturesPerRow = this->texAtlW/this->texAtlSubSize;

		this->pixelWidth =  (1.0f/texAtlW)*pixelWidth;
		this->pixelHeight = (1.0f/texAtlH)*pixelHeight;

		animData[0].textureAtlasX=0.0f;
		animData[0].textureAtlasY=0.0f;

		inited=true;
	}

	GLvoid CSubtileData::create(GLint textureAtlasPos, GLint repeat)
	{	
		init();

		GLint pixelPosX=(textureAtlasPos%texturesPerRow)*texAtlSubSize;
		GLint pixelPosY=(textureAtlasPos/texturesPerRow)*texAtlSubSize;					

		for (GLint i=0; i<repeat; i++)
		{
			animData[i].textureAtlasX=(GLfloat)pixelPosX/(GLfloat)texAtlW+pixelWidth;
			animData[i].textureAtlasY=1.0f-(GLfloat)pixelPosY/(GLfloat)texAtlH-pixelHeight;
		}

		animPos=0;
		animCount=repeat;
		animSpeed=0.0f;
		animChange=1.0f;
		reset=false;
	}

	GLvoid CSubtileData::create(GLint textureAtlasPos[], GLint count, bool randomStart, bool reset)
	{
		init();

		for (GLint i=0; i<count; i++)
		{
			GLint pixelPosX=(textureAtlasPos[i]%texturesPerRow)*texAtlSubSize;
			GLint pixelPosY=(textureAtlasPos[i]/texturesPerRow)*texAtlSubSize;

			animData[i].textureAtlasX=(GLfloat)pixelPosX/(GLfloat)texAtlW+pixelWidth;
			animData[i].textureAtlasY=1.0f-(GLfloat)pixelPosY/(GLfloat)texAtlH-pixelHeight;
		}
		
		animPos=(randomStart)?rand()%count:0;
		animCount=count;
		animSpeed=0.0f;
		animChange=1.0f;
		this->reset=reset;
	}

	GLvoid CSubtileData::resetAnimation()
	{
		animPos=0;
	}

	GLvoid CSubtileData::updateAnimation(GLfloat delta)
	{
		animChange-=animSpeed*delta;

		if (animChange<=0.0f)
		{
			if (!reset)
			{
				if (up)
				{
					animPos++;
				}
				else
				{
					animPos--;
				}

				if (animPos<0)
				{
					animPos=0;
					up=!up;
				}
			}
			else
			{
				animPos++;
			}

			if (animPos>=animCount)
			{
				if (!reset)
				{
					animPos=animCount-1;
					up=!up;
				}
				else
				{
					animPos=0;
				}
			}

			animChange=1.0f;
		}	
	}

	vector2f CSubtileData::getTextureAtlasPosition()
	{
		return vector2f(animData[animPos].textureAtlasX,animData[animPos].textureAtlasY);
	}

	GLvoid CSubtileData::setAnimSpeed(GLfloat animSpeed)
	{
		this->animSpeed=animSpeed;
	}

	GLfloat CSubtileData::getPixelWidth()
	{
		init();
		return pixelWidth;
	}

	GLfloat CSubtileData::getPixelHeight()
	{
		init();
		return pixelHeight;
	}

	GLint CSubtileData::getAnimCount()
	{
		init();
		return animCount;
	}

	GLvoid CSubtileData::setAnimPos(GLint animPos)
	{
		init();
		this->animPos = animPos;
	}

	vector2f CSubtileData::getTextureAtlasSize()
	{
		return vector2f(textureAtlasWidth,textureAtlasHeight);
	}

	GLfloat CSubtileData::getTextureAtlasSubtileSize()
	{
		return textureAtlasSubtileSize;
	}
};