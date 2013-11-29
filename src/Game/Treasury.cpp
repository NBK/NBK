#include "commons.h"
#include "Treasury.h"

using namespace cml;
using namespace game_utils;

namespace game_objects
{
	namespace rooms
	{
		CTreasury::CTreasury()
			: CRoom()
		{
		}

		CTreasury::~CTreasury()
		{

		}

		GLvoid CTreasury::update()
		{
			CV_GAME_MANAGER->getConsole()->writeLine("I AM A TREASURY UPDATING");
		}

	}
}