#include <windows.h>
#include <gl\gl.h>
#include "Input.h"

using namespace cml;
using namespace control;

namespace control
{
	CInput::CInput()
	{
		ZeroMemory(keys,sizeof(bool)*256);
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
		//update mouse positions
		POINT p;
		GetCursorPos(&p);

		xMove = p.x-xPos;
		yMove = p.y-yPos;

		xPos = p.x;
		yPos = p.y;
		return true;
	}

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

	bool CInput::isKeyDown(char key)
	{
		return keys[key];
	}

	GLvoid CInput::setMousePos(GLint xpos, GLint ypos)
	{
		SetCursorPos(xpos,ypos);
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