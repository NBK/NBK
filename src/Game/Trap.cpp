#include "commons.h"
#include "Trap.h"

using namespace std;
using namespace cml;
using namespace loaders;
using namespace game_utils;

namespace game_objects
{
	namespace block_objects
	{
		CTrap::CTrap(string name, vector3f position, CTrap::TRAP_TYPE trapType)
			: CBlockObject(name,position,NULL,0.0f)
		{
			this->trapType = trapType;

			// used when selling block objects
			setClassName("TRAP");

			/* 
				Depending on the trap type we set the proper inner light bulb.
				Boulder trap is excluded since it's unique.
			*/

			setModel(CV_GAME_MANAGER->getResourceManager()->getModel(name));

			vector3f colors[] =
			{
				vector3f(0.3f,0.1f,0.3f),
				vector3f(0.5f,0.6f,0.3f),
				vector3f(0.1f,0.2f,0.7f),
				vector3f(0.8f,0.0f,0.0f),
				vector3f(0.6f,0.0f,0.3f),
			};

			if (trapType!=TT_BOULDER)
			{
				m_effectName = "EFFECTS_TRAP_INNER";
				setEffect(CV_GAME_MANAGER->getResourceManager()->getEffect(m_effectName));
				
				// set the effect collor according to the trap type
				if (m_effect)
				{
					m_effect->getEmittter(0)->setColor(colors[trapType-TT_BOULDER-1]);
				}
			}
		}

		CTrap::~CTrap()
		{

		}
	};
};