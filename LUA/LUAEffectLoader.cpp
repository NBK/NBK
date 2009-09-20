#include "LUAEffectLoader.h"

using namespace std;

namespace LUA_effects
{
	CLUAEffectLoader::CLUAEffectLoader()
	{
	}

	CLUAEffectLoader::~CLUAEffectLoader()
	{
	}

	CLUAEffect *CLUAEffectLoader::loadFromFile(string fileName)
	{
		FILE *f = fopen(fileName.c_str(),"rb");

		if (!f)
		{
			return NULL;
		}

		GLuint emittersCount=0;

		fread(&emittersCount,sizeof(GLuint),1,f);

		EMITTER_DATA eData;

		CLUAEffect *newLUAEffect = new CLUAEffect();

		for (GLuint i=0; i<emittersCount; i++)
		{
			fread(&eData,sizeof(EMITTER_DATA),1,f);

			newLUAEffect->addEmitter();

			string effDir = fileName.substr(0,fileName.find_last_of('/'));

			newLUAEffect->getEmittter(i)->setInitLUAFile(effDir,string(eData.initFile));
			newLUAEffect->getEmittter(i)->setUpdateLUAFile(effDir,string(eData.updateFile));
			
			//newLUAEffect->getEmittter(i)->setInitLUAFile(string(eData.effectDirectory),string(eData.initFile));
			//newLUAEffect->getEmittter(i)->setUpdateLUAFile(string(eData.effectDirectory),string(eData.updateFile));
		}

		fclose(f);

		return newLUAEffect;
	}
};