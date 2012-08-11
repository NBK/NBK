#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include "Manager.h"
#include <string>
#include <map>
#include <cml/cml.h>

/*
	This class contains game settings that must be read from 
	settings.conf or similar file in config directory.
*/

namespace game_utils
{
	namespace managers
	{
		class CSettingsManager: public CManager
		{
		public:

			CSettingsManager();
			virtual ~CSettingsManager();

			/*
				Description:
				Reads game settings using config reader. If no file 
				is set, the default config/settings.conf will be used.				
			*/
			virtual bool init();			
			virtual bool update();
			virtual bool shutdown();

			/* 
				If we need to read some file other then settings.conf and this file
				is of same structure as other settings.conf we can create a local 
				copy of CSettingsManager to read data from that file.
			*/
			bool init(std::string configFile);

			GLint			getSetting_Int(std::string settingName);
			GLfloat			getSetting_Float(std::string settingName);
			std::string		getSetting_String(std::string settingName);
			cml::vector2f	getSetting_vector2f(std::string settingName);
			cml::vector3f	getSetting_vector3f(std::string settingName);

			// returns a map of settings so we alone can reprocess it
			std::map<std::string,std::string> *getSettings();

		private:

			std::map<std::string,std::string> settings;
		};
	};
};

#endif // GAME_SETTINGS_H