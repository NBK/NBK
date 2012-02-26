#include "system.h"
#include <GL/gl.h>
#include "Input.h"

using namespace cml;
using namespace control;

namespace control
{
	CInput::CInput()
	{
#ifdef WIN32
		ZeroMemory(keys,sizeof(bool)*256);
#else
		ZeroMemory(keys,sizeof(bool)*SDLK_LAST);
#endif
		lmbd=rmbd=mmbd=false;
		mscroll=0;
	}

	CInput::~CInput()
	{
	}

	vector2i CInput::getMousePos()
	{
		return vector2i(xPos,yPos);
	}

	vector2i CInput::getMouseMove()
	{
		return vector2i(xMove,yMove);
	}

	int CInput::getMouseMoveX()
	{
		return xMove;
	}

	int CInput::getMouseMoveY()
	{
		return yMove;
	}

	bool CInput::update()
	{
#ifdef WIN32
		//update mouse positions
		POINT p;
		GetCursorPos(&p);

		xMove = p.x-xPos;
		yMove = p.y-yPos;

		xPos = p.x;
		yPos = p.y;
#else
		GLint x, y;
		SDL_GetMouseState(&x, &y);

		xMove = x-xPos;
		yMove = y-yPos;

		xPos = x;
		yPos = y;
#endif
		return true;
	}

#ifdef WIN32
	GLvoid CInput::update(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
			case WM_KEYDOWN:
			{
				keys[wParam]=true;

				// update registered listeners
				for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
				{
					(*rlIter)->onKeyDown(wParam);
				}

				break;
			}

			case WM_KEYUP:
			{
				keys[wParam]=false;

				// update registered listeners
				for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
				{
					(*rlIter)->onKeyUp(wParam);
				}

				break;
			}

			case WM_LBUTTONDOWN:
			{
				lmbd=true;

				// update registered listeners
				for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
				{
					(*rlIter)->onMouseClicked(0);
				}
				break;
			}

			case WM_LBUTTONUP:
			{
				lmbd=false;
				break;
			}

			case WM_RBUTTONDOWN:
			{
				rmbd=true;

				// update registered listeners
				for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
				{
					(*rlIter)->onMouseClicked(1);
				}

				break;
			}

			case WM_RBUTTONUP:
			{
				rmbd=false;
				break;
			}

			case WM_MBUTTONDOWN:
			{
				mmbd=true;

				// update registered listeners
				for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
				{
					(*rlIter)->onMouseClicked(3);
				}

				break;
			}

			case WM_MBUTTONUP:
			{
				mmbd=false;
				break;
			}

			//update distance scrolled
			case WM_MOUSEWHEEL:
			{
				mscroll += GET_WHEEL_DELTA_WPARAM(wParam);
				break;
			}
		}
	}
#else
	GLvoid CInput::update(SDL_Event event)
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
			{
				keys[event.key.keysym.sym]=true;

				// update registered listeners
				for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
				{
					(*rlIter)->onKeyDown(event.key.keysym.sym);
				}

				break;
			}

			case SDL_KEYUP:
			{
				keys[event.key.keysym.sym]=false;

				// update registered listeners
				for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
				{
					(*rlIter)->onKeyUp(event.key.keysym.sym);
				}

				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
						lmbd=true;

						// update registered listeners
						for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
						{
							(*rlIter)->onMouseClicked(0);
						}
						break;
					}
					case SDL_BUTTON_RIGHT:
					{
						rmbd=true;

						// update registered listeners
						for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
						{
							(*rlIter)->onMouseClicked(1);
						}

						break;
					}
					case SDL_BUTTON_MIDDLE:
					{
						mmbd=true;

						// update registered listeners
						for (rlIter=registeredListeners.begin(); rlIter!=registeredListeners.end(); rlIter++)
						{
							(*rlIter)->onMouseClicked(3);
						}

						break;
					}
					case SDL_BUTTON_WHEELUP:
					{
						mscroll ++;
						break;
					}
					case SDL_BUTTON_WHEELDOWN:
					{
						mscroll --;
						break;
					}
				}
				break;
			}

			case SDL_MOUSEBUTTONUP:
			{
				switch (event.button.button)
				{
					case SDL_BUTTON_LEFT:
					{
						lmbd=false;
						break;
					}
					case SDL_BUTTON_RIGHT:
					{
						rmbd=false;
						break;
					}
					case SDL_BUTTON_MIDDLE:
					{
						mmbd=false;
						break;
					}
				}
				break;
			}
		}
	}
#endif

	bool CInput::isKeyDown(int key)
	{
		return keys[key];
	}

	GLvoid CInput::setMousePos(GLint xpos, GLint ypos)
	{
#ifdef WIN32
		SetCursorPos(xpos,ypos);
#else
		SDL_WarpMouse(xpos,ypos);
#endif
	}

	bool CInput::isLeftMouseDown()
	{
		return lmbd;
	}

	bool CInput::isRightMouseDown()
	{
		return rmbd;
	}

	bool CInput::isMiddleMouseDown()
	{
		return mmbd;
	}

	int CInput::checkScroll()
	{
		int scroll = mscroll;
		mscroll=0;
		return scroll;
	}

	GLvoid CInput::registerListener(CInputListener *listener)
	{
		registeredListeners.push_back(listener);
	}
};