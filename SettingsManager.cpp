#include "commons.h"
#include "SettingsManager.h"
#include "Conversions.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace cml;
using namespace utils;

namespace game_utils
{
	namespace managers
	{
		CSettingsManager::CSettingsManager(): CManager()
		{
		}

		CSettingsManager::~CSettingsManager()
		{
		}

		bool CSettingsManager::init(string configFile)
		{
			ifstream iFile;
			iFile.open((CV_RESOURCES_DIRECTORY+configFile).c_str());

			if (!iFile)
			{
				return false;
			}

			settings.clear();

			string line="";
			char name[64];

			while (!iFile.eof())
			{
				getline (iFile,line);

				if (line.length()!=0 && line.at(0)!='#')
				{
					sscanf(line.c_str(),"%s",name);

					string val = sStringUtils::trim(line.substr(strlen(name)+1));

					settings[string(name)] = val;
				}
			}	

			iFile.close();

			return true;
		}

		bool CSettingsManager::init()
		{
			return init(CV_CONFIG_SETTINGS);
		}

		bool CSettingsManager::update()
		{
			return true;
		}

		bool CSettingsManager::shutdown()
		{
			return true;
		}

		GLint CSettingsManager::getSetting_Int(string settingName)
		{
			return CConversions::strToInt(getSetting_String(settingName));
		}

		GLfloat CSettingsManager::getSetting_Float(string settingName)
		{
			return CConversions::strToFloat(getSetting_String(settingName));
		}

		string CSettingsManager::getSetting_String(string settingName)
		{
			map<std::string,std::string>::iterator iter = settings.find(settingName);
			return (iter!=settings.end()?iter->second:"-1");
		}

		vector2f CSettingsManager::getSetting_vector2f(string settingName)
		{
			return CConversions::strToVec2f(getSetting_String(settingName));
		}

		vector3f CSettingsManager::getSetting_vector3f(string settingName)
		{
			return CConversions::strToVec3f(getSetting_String(settingName));
		}

		map<string,string> *CSettingsManager::getSettings()
		{
			return &settings;
		}
	}
};