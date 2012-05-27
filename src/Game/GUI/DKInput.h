#ifndef DKINPUT_H
#define DKINPUT_H

#include "DKcommons.h"
#include "../SDLUtils.h"

class CDKInput  
{
public:
	CDKInput();
	~CDKInput();
	GLint get_screen_x();
	GLint get_screen_y();
#ifdef WIN32
	GLvoid update(UINT message, WPARAM wParam, LPARAM lParam);
#else
	GLvoid update(SDL_Event event);
#endif

	bool is_left_down();
	bool is_right_down();
	bool is_up_down();
	bool is_down_down();
	bool is_esc_down();
	bool is_space_down();
	bool is_enter_down();
	bool is_shift_down();
	bool is_tab_down();
	bool is_control_down();
	bool is_plus_down();
	bool is_minus_down();
	bool is_F1_down();
	bool is_F2_down();
	bool is_F3_down();
	bool is_F4_down();
	bool is_F5_down();
	bool is_F6_down();
	bool is_F7_down();
	bool is_F8_down();
	bool is_F9_down();
	bool is_F10_down();
	bool is_F11_down();
	bool is_F12_down();

	bool is_key_down(int key);
	GLvoid set_z_depth(GLfloat z_depth);

	/*
	11,044x...10z
	x.... nz

	x=11,044*nz/10

	==> world_screen_width=fabs(1,1044*z_depth);

	==> world_screen_height=fabs(0,8283*z_depth);

	*/

	GLfloat get_world_x();
	GLfloat get_world_y();

	GLvoid set_mouse_pos(GLint xpos, GLint ypos);

	bool is_lmouse_down();
	bool is_rmouse_down();

private:
#ifdef WIN32
	bool keys[256];
#else
	bool keys[SDLK_LAST];
#endif
	bool lmouse_down,rmouse_down;
	GLfloat z_depth,wsw,wsh,w_2,h_2, cos_45;
};

#endif // DKINPUT_H
