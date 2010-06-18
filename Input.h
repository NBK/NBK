#ifndef INPUT_H
#define INPUT_H

#include <cml/cml.h>
#include "InputListener.h"

namespace control
{
	class CInput  
	{
	public:
		CInput();
		~CInput();	

		GLvoid update(UINT message, WPARAM wParam, LPARAM lParam);
		bool update();
	
		cml::vector2i getMousePos();
		cml::vector2i getMouseMove();

		bool isKeyDown(char key);		
		bool isLeftMouseDown();
		bool isRightMouseDown();
		bool isMiddleMouseDown();
		int checkScroll();
		int getMouseMoveX();
		int getMouseMoveY();

		GLvoid setMousePos(GLint xpos, GLint ypos);

		GLvoid registerListener(control::CInputListener *listener);

	private:
		bool keys[256];
		bool lmbd,rmbd,mmbd;
		short mscroll;
		int xPos, yPos;
		int xMove, yMove;
		
		std::vector<control::CInputListener*>			registeredListeners;
		std::vector<control::CInputListener*>::iterator rlIter;
	};
};

#endif // INPUT_H
