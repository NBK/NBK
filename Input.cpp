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
		lmbd=rmbd=false;
	}

	CInput::~CInput()
	{
	}

	vector2i CInput::getMousePos()
	{
		POINT p;
		GetCursorPos(&p);
		return vector2i(p.x,p.y);
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

	GLvoid CInput::registerListener(CInputListener *listener)
	{
		registeredListeners.push_back(listener);
	}
};