#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include "Manager.h"
#include "Frustum.h"
#include "Camera.h"
#include "Input.h"
#include "ConsoleListener.h"
#include <string>

/*
	Teakes control of camera system, keyboard controls, mouse input, ...
*/

namespace game_utils
{
	namespace managers
	{
		class CControlManager: public CManager, public control::CConsoleListener
		{
		public:
			CControlManager();
			~CControlManager();

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

			control::CCamera	*getCamera();
			geometry::CFrustum	*getViewFrustum();
			control::CInput		*getInput();

			bool				isFPS();

		private:

			geometry::CFrustum	viewFrustum;
			control::CCamera	camera;
			control::CInput		input;

			/*
				If we are in the FPS mode this is true.
			*/
			bool				FPS;

			GLfloat		camRotYSpeed,
						camZoomSpeed,
						camMoveSpeed,
						camMLookSpeed,
						camInitXRot;
			
		};
	};
};

#endif // CONTROL_MANAGER_H