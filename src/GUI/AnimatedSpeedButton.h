#ifndef ANIMATED_SPEED_BUTTON_H
#define ANIMATED_SPEED_BUTTON_H

#include "SpeedButton.h"

namespace DK_GUI
{
	class CAnimatedSpeedButton: public CSpeedButton
	{
	public:
		CAnimatedSpeedButton(CDKTextureList *game_textures, GLint name, GLint texture_count, const char **texture_names);
		~CAnimatedSpeedButton();

		virtual GLvoid on_mouse_up();
		virtual GLvoid on_mouse_down();
		virtual GLvoid draw();	

	private:
		bool on, first;


		GLuint *textures;
		GLint	texture_count,
				tex_pos;

		GLfloat	tex_change,
				tex_change_speed;
	};
}

#endif // ANIMATED_SPEED_BUTTON_H