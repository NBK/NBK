#ifndef MANAGER_H
#define MANAGER_H

#include "system.h"
#include <GL/gl.h>
#include <string>
#include <cml/cml.h>

namespace game_utils
{
	namespace managers
	{
		class CManager
		{
		public:
			CManager();
			~CManager();

			virtual bool init() = 0;
			virtual bool update() = 0;
			virtual bool shutdown() = 0;
		};
	};
};

#endif // MANAGER_H