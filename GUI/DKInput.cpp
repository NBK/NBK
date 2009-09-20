#include "../commons.h"
#include <windows.h>
#include <gl\gl.h>
#include "DKInput.h"
#include <math.h>

using namespace game_utils;

CDKInput::CDKInput()
{
	for (GLuint i=0; i<256; i++)
		keys[i]=false;

	z_depth=wsw=w_2=h_2=0.0f;

	cos_45=0.70710678118f;

	lmouse_down=rmouse_down=false;
}

CDKInput::~CDKInput()
{

}

GLint CDKInput::get_screen_x()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(CV_WINDOW_HANDLE,&p);
	return p.x;
}

GLint CDKInput::get_screen_y()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(CV_WINDOW_HANDLE,&p);
	return p.y;
}

GLvoid CDKInput::set_z_depth(GLfloat z_depth)
{
	this->z_depth=z_depth;
	wsw=fabs(1.1044f*z_depth);
	wsh=fabs(0.8283f*z_depth);
	w_2=1024/2;
	h_2=768/2;
}

GLfloat CDKInput::get_world_x()
{
	GLfloat mx=(GLfloat)get_screen_x();

	if (mx<=w_2)
	{
		return -(wsw/2-((wsw/2)*mx)/w_2);
	}
	else
	{
		return ((wsw/2)*(mx-w_2))/w_2;
	}
}

GLfloat CDKInput::get_world_y()
{
	GLfloat my=(GLfloat)get_screen_y();

	if (my<=h_2)
	{
		return wsh/2-((wsh/2)*my)/h_2;
	}
	else
	{
		return -((wsh/2)*(my-h_2))/h_2;
	}
}

GLvoid CDKInput::update(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_KEYDOWN:
		{
			keys[wParam]=true;
			break;											// Jump Back
		}

		case WM_KEYUP:
		{
			keys[wParam]=false;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			lmouse_down=true;
			break;
		}

		case WM_LBUTTONUP:
		{
			lmouse_down=false;
			break;
		}

		case WM_RBUTTONDOWN:
		{
			rmouse_down=true;
			break;
		}

		case WM_RBUTTONUP:
		{
			rmouse_down=false;
			break;
		}
	}
}

bool CDKInput::is_left_down() 
{ 
	return keys[VK_LEFT]; 
}

bool CDKInput::is_right_down() 
{ 
	return keys[VK_RIGHT]; 
}

bool CDKInput::is_up_down() 
{ 
	return keys[VK_UP]; 
}

bool CDKInput::is_down_down() 
{ 
	return keys[VK_DOWN]; 
}

bool CDKInput::is_esc_down()  
{ 
	return keys[VK_ESCAPE]; 
}

bool CDKInput::is_space_down() 
{ 
	return keys[VK_SPACE]; 
}

bool CDKInput::is_enter_down()
{
	return keys[VK_RETURN];
}

bool CDKInput::is_shift_down()
{
	return keys[VK_SHIFT];
}

bool CDKInput::is_tab_down()
{
	return keys[VK_TAB];
}

bool CDKInput::is_control_down()
{
	return keys[VK_CONTROL];
}

bool CDKInput::is_key_down(char key)
{
	return keys[key];
}

bool CDKInput::is_plus_down()
{
	return keys[VK_ADD];
}

bool CDKInput::is_minus_down()
{
	return keys[VK_SUBTRACT];
}

bool CDKInput::is_F1_down()
{
	return keys[VK_F1];
}

bool CDKInput::is_F2_down()
{
	return keys[VK_F2];
}

bool CDKInput::is_F3_down()
{
	return keys[VK_F3];
}

bool CDKInput::is_F4_down()
{
	return keys[VK_F4];
}

bool CDKInput::is_F5_down()
{
	return keys[VK_F5];
}

bool CDKInput::is_F6_down()
{
	return keys[VK_F6];
}

bool CDKInput::is_F7_down()
{
	return keys[VK_F7];
}

bool CDKInput::is_F8_down()
{
	return keys[VK_F8];
}

bool CDKInput::is_F9_down()
{
	return keys[VK_F9];
}

bool CDKInput::is_F10_down()
{
	return keys[VK_F10];
}

bool CDKInput::is_F11_down()
{
	return keys[VK_F11];
}

bool CDKInput::is_F12_down()
{
	return keys[VK_F12];
}

GLvoid CDKInput::set_mouse_pos(GLint xpos, GLint ypos)
{
	SetCursorPos(xpos,ypos);
}

bool CDKInput::is_lmouse_down()
{
	return lmouse_down;
}

bool CDKInput::is_rmouse_down()
{
	return rmouse_down;
}