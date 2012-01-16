#include "settings.h"
#include "commons.h"
#include "ControlManager.h"
#include "commons.h"
#include <cml/cml.h>

using namespace control;
using namespace geometry;
using namespace cml;
using namespace std;

#define GOTO	"GOTO"

namespace game_utils
{
	namespace managers
	{
		CControlManager::CControlManager(): CManager()
		{
			FPS = false;
		}

		CControlManager::~CControlManager()
		{
		}

		bool CControlManager::init()
		{
			viewFrustum.initialize(CV_SETTINGS_FOV,(GLfloat)CV_SETTINGS_WINDOW_WIDTH/(GLfloat)CV_SETTINGS_WINDOW_HEIGHT,0.01f,10000.0f);

			//camera.setPosition(vector3f(CV_BLOCK_WIDTH*42.0f,CV_CAMERA_INITIAL_HEIGHT,CV_BLOCK_DEPTH*42.0f));
			camera.setPosition(vector3f(CV_BLOCK_WIDTH*4.0f,CV_CAMERA_INITIAL_HEIGHT,CV_BLOCK_DEPTH*4.0f));

			CSettingsManager *sManager = CV_GAME_MANAGER->getSettingsManager();

			camRotYSpeed = sManager->getSetting_Float(CV_SETTINGS_CAMERA_ROTATE_Y_SPEED);
			camZoomSpeed = sManager->getSetting_Float(CV_SETTINGS_CAMERA_ZOOM_SPEED);
			camMoveSpeed = sManager->getSetting_Float(CV_SETTINGS_CAMERA_MOVE_SPEED);
			camMLookSpeed = sManager->getSetting_Float(CV_SETTINGS_CAMERA_MLOOK_SPEED);
			camInitXRot = sManager->getSetting_Float(CV_SETTINGS_CAMERA_INITIAL_X_ROTATION);

			camera.rotateX(camInitXRot);

			// register camera as a possible collision entity with terrain
			CV_GAME_MANAGER->getCollisionManager()->registerEntity_T(&camera);

			// register to console
			CV_GAME_MANAGER->getConsole()->registerClass(this,"CONTROL MANAGER");
			CV_GAME_MANAGER->getConsole()->addParam(GOTO,"(x y) Jumps to a block on x,y location. x and y must be (>=0 && <84)");

			return true;
		}

		bool CControlManager::update()
		{	
			input.update();

			vector3f camPos = camera.getPosition();

			GLfloat delta = CV_GAME_MANAGER->getDeltaTime();
			
			//zoom with scroll wheel
			float scroll = -(float)input.checkScroll()/120.0f;
			camera.move(vector3f(0.0f,camZoomSpeed*delta*scroll,0.0f));					

			//clip camera pos
			if (camera.getPosition()[1]>=CV_CAMERA_MAX_HEIGHT)
			{
				camPos[1] = CV_CAMERA_MAX_HEIGHT;
				camera.setPosition(camPos);
			}
			else if (camera.getPosition()[1]<CV_CAMERA_MIN_HEIGHT)
			{
				camPos[1] = CV_CAMERA_MIN_HEIGHT;
				camera.setPosition(camPos);
			}

			if (input.isMiddleMouseDown())
			{
				//if the middle mouse button is down, we can rotate the view with the mouse
				camera.moveForward(!FPS?CV_BLOCK_DEPTH*(camPos[1]+2.0f):0.0f);
				camera.rotateY(camRotYSpeed*delta*input.getMouseMoveX());
				camera.rotateX(camRotYSpeed*delta*input.getMouseMoveY());
				camera.moveForward(!FPS?-CV_BLOCK_DEPTH*(camPos[1]+2.0f):0.0f);
			}else
			{
				//move camera when mouse is near edges of screen
#ifdef WIN32
				POINT pt;
				GetCursorPos(&pt);
				RECT rect;
				GetWindowRect(CV_WINDOW_HANDLE,&rect);
#else
				// TODO Tequila: This is only correct in fullscreen and input grab mode
				MOUSEPOINT pt;
				RECT rect;
				SDL_GetMouseState(&pt.x, &pt.y);
				rect.left = 0;
				rect.right = CV_SETTINGS_WINDOW_WIDTH;
				rect.top = 0;
				rect.bottom = CV_SETTINGS_WINDOW_HEIGHT;
#endif
				int border=20;

				if(pt.x<rect.left+border)
					camera.strafeLeft(camMoveSpeed*delta);
				else if(pt.x>rect.right-border)
					camera.strafeLeft(-camMoveSpeed*delta);

				GLfloat tmpRotX = camera.getRotateX();
				camera.rotateX(!FPS?-tmpRotX:0.0f);
				
				if(pt.y<rect.top+border)
					camera.moveForward(camMoveSpeed*delta);
				else if(pt.y>rect.bottom-border)
					camera.moveForward(-camMoveSpeed*delta);

				camera.rotateX(!FPS?tmpRotX:0.0f);

				if (input.isKeyDown(VK_CONTROL))
				{
					// handle rotation and zoom-in/out

					bool rotate = (input.isKeyDown(VK_LEFT) || input.isKeyDown(VK_RIGHT));
					bool zoom	= (input.isKeyDown(VK_UP) || input.isKeyDown(VK_DOWN)) && !FPS; // no zoom in FPS

					if (rotate)
					{
						camera.moveForward(!FPS?CV_BLOCK_DEPTH*(camPos[1]+2.0f):0.0f);
						camera.rotateY(camRotYSpeed*delta*(input.isKeyDown(VK_LEFT)?-1.0f:1.0f));
						camera.moveForward(!FPS?-CV_BLOCK_DEPTH*(camPos[1]+2.0f):0.0f);
					}
					else if (zoom)
					{
						camera.move(vector3f(0.0f,camZoomSpeed*delta*(input.isKeyDown(VK_UP)?-1.0f:1.0f),0.0f));					

						if (camera.getPosition()[1]>=CV_CAMERA_MAX_HEIGHT)
						{
							camPos[1] = CV_CAMERA_MAX_HEIGHT;
							camera.setPosition(camPos);
						}
						else if (camera.getPosition()[1]<CV_CAMERA_MIN_HEIGHT)
						{
							camPos[1] = CV_CAMERA_MIN_HEIGHT;
							camera.setPosition(camPos);
						}
					}
				}
				else
				{
					// handle standard movement

					bool l_r = input.isKeyDown(VK_LEFT) || input.isKeyDown(VK_RIGHT);
					bool u_d = input.isKeyDown(VK_UP) || input.isKeyDown(VK_DOWN);

					if (l_r)
					{
						camera.strafeLeft(camMoveSpeed*delta*(input.isKeyDown(VK_SHIFT)?10.0f:1.0f)*(input.isKeyDown(VK_LEFT)?1.0f:-1.0f));
					}
				
					if (u_d)
					{
						GLfloat tmpRotX = camera.getRotateX();
						camera.rotateX(!FPS?-tmpRotX:0.0f);
						camera.moveForward(camMoveSpeed*delta*(input.isKeyDown(VK_SHIFT)?10.0f:1.0f)*(input.isKeyDown(VK_UP)?1.0f:-1.0f));
						camera.rotateX(!FPS?tmpRotX:0.0f);
					}

					// if we are in the FPS mode we have mouse look
					if (FPS)
					{
#ifdef WIN32
						POINT pt;
						GetCursorPos(&pt);
#else
						MOUSEPOINT pt;
						SDL_GetMouseState(&pt.x, &pt.y);
#endif
						camera.rotateY((GLfloat)(pt.x-CV_SETTINGS_WINDOW_WIDTH_HALF)*camMLookSpeed*2.0f);
						camera.rotateX((GLfloat)(pt.y-CV_SETTINGS_WINDOW_HEIGHT_HALF)*camMLookSpeed*2.0f);
#ifdef WIN32
						SetCursorPos(CV_SETTINGS_WINDOW_WIDTH_HALF,CV_SETTINGS_WINDOW_HEIGHT_HALF);
#else
						SDL_WarpMouse(CV_SETTINGS_WINDOW_WIDTH_HALF, CV_SETTINGS_WINDOW_HEIGHT_HALF);
#endif
					}

					// insert resets the keyboard
					if (input.isKeyDown(VK_INSERT))
					{
						camPos[1] = CV_CAMERA_INITIAL_HEIGHT;
						camera.setPosition(camPos);
					}

					// TEMP: jump into FPS mode
					if (input.isKeyDown(VK_F1) && !FPS)
					{
						GLint centerX = (GLint)(camera.getPosition()[0]/CV_BLOCK_WIDTH);
						GLint centerY = (GLint)(camera.getPosition()[2]/CV_BLOCK_DEPTH);
						if (CV_GAME_MANAGER->getLevelManager()->getBlock(centerX,centerY)->isLow())
						{
							FPS=true;
							camera.rotateX(-camInitXRot*delta);
							camPos[1]=0.2f;
							camera.setPosition(camPos);

							// position the mouse to screen center
#ifdef WIN32
							SetCursorPos(CV_SETTINGS_WINDOW_WIDTH_HALF,CV_SETTINGS_WINDOW_HEIGHT_HALF);
#else
							SDL_WarpMouse(CV_SETTINGS_WINDOW_WIDTH_HALF, CV_SETTINGS_WINDOW_HEIGHT_HALF);
#endif
						}
					}
					else if (input.isKeyDown(VK_F2) && FPS)
					{
						FPS=false;
						camera.rotateX(camInitXRot);
						camPos[1]=CV_CAMERA_INITIAL_HEIGHT;
						camera.setPosition(camPos);
					}

					// temp adjust X angle
					if (input.isKeyDown(VK_HOME))
					{
						camera.rotateX(camRotYSpeed*delta);
					}
					else if (input.isKeyDown(VK_END))
					{
						camera.rotateX(-camRotYSpeed*delta);
					}
				}
			}

			// collision detection only when in FPS mode
			camera.setActive(FPS);

			// now update the frustum with new camera params
			viewFrustum.update(&camera);

			return true;
		}

		bool CControlManager::shutdown()
		{
			return true;
		}

		CCamera *CControlManager::getCamera()
		{
			return &camera;
		}

		CFrustum *CControlManager::getViewFrustum()
		{
			return &viewFrustum;
		}

		CInput *CControlManager::getInput()
		{
			return &input;
		}

		bool CControlManager::isFPS()
		{
			return FPS;
		}

		string CControlManager::onAction(string keyword, string params)
		{	
			string checkResult = "";
			std::vector<string> tParams;
			
			if (keyword==GOTO)
			{
				if (!CConsoleListener::checkParams(params,2,checkResult,tParams))
				{					
					return checkResult;
				}	

				GLint x = CConversions::strToInt(tParams[0]);
				GLint y = CConversions::strToInt(tParams[1]);

				if (x<0 || y<0 || x>=(GLint)CV_LEVEL_MAP_SIZE || y>=(GLint)CV_LEVEL_MAP_SIZE)
				{
					return "Error: Coordinates out of this level!";
				}

				vector3f newCamPos = vector3f(	(GLfloat)x*CV_BLOCK_WIDTH,
												camera.getPosition()[1],
												(GLfloat)y*CV_BLOCK_DEPTH);

				camera.setPosition(newCamPos);

				return "Camera position set to "+CConversions::vec3fToStr(newCamPos);
			}

			return "<>";
		}
	};
};
