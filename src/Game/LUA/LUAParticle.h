#ifndef LUA_PARTICLE_H
#define LUA_PARTICLE_H

#include <cml/cml.h>
#include <string>

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

namespace LUA_effects
{
	class CLUAParticle
	{
	public:
		CLUAParticle();	
		virtual ~CLUAParticle();

		virtual GLvoid init(GLint index, GLint count);
		virtual GLvoid draw();
		virtual GLvoid update();
		virtual GLvoid reset();
		virtual bool is_dead();

		GLvoid set_start_position(cml::vector3f start_position);
		GLvoid set_start_position_only(cml::vector3f start_position);
		GLvoid update_position(cml::vector3f dPos);
		GLvoid update_move_vector(cml::vector3f dPos);

		GLvoid set_move_vector(cml::vector3f move_vector);
		GLvoid set_current_move_vector(cml::vector3f move_vector);
		GLvoid set_color(cml::vector3f color);

		GLvoid set_alpha_properties(GLfloat start_alpha, GLfloat alpha_mod);
		GLvoid set_alpha_mod(GLfloat alpha_mod);
		GLfloat get_start_alpha();

		GLvoid set_life_properties(GLfloat start_life, GLfloat life_mod);		
		GLvoid set_current_life(GLfloat life);
		GLfloat get_start_life();
		GLfloat get_life_mod();

		GLvoid set_size_properties(GLfloat start_size, GLfloat size_mod);
		GLvoid set_size_mod(GLfloat size_mod);
		GLfloat get_start_size();

		GLvoid set_spawn_properties(GLfloat start_spawn_delay);
		GLfloat get_current_spawn_delay();

		// a simple static texture representing our particle
		GLvoid set_texture(GLuint texture);

		// non simple texture consisting of series of steps for simulating texture animation
		GLvoid set_texture(GLuint texture, GLint count_x, GLint count_y, GLfloat speed);

		GLvoid set_gravity(cml::vector3f gravity);

		GLvoid set_reborn(bool reborn);
		GLvoid reset_alpha_on_reborn(bool reset_alpha);
		GLvoid set_dead(bool dead);		
		GLvoid set_billboard(bool billboard);

		cml::vector3f get_gravity();
		cml::vector3f get_current_position();
		cml::vector3f get_start_position();
		cml::vector3f get_move();
		cml::vector3f get_current_move();

		GLvoid setLUAFile(std::string initLUAFile);

		GLvoid setEffectDirectory(std::string effectDirectory);
		std::string getEffectDirectory();

		GLvoid set_parent(GLvoid *parent);

		GLvoid set_connect_movement_to_parent(bool connect);

	protected:
		cml::vector3f	start_position,
						current_position,						

						start_move_vector,
						current_move_vector,

						color,
						gravity;

		GLfloat start_alpha,
				current_alpha,
				alpha_mod,

				start_life,
				current_life,
				start_current_life,
				life_mod,

				start_size,
				current_size,
				size_mod,

				start_spawn_delay,
				current_spawn_delay;

		GLuint texture;

		bool	reborn, 
				dead, 
				billboard,
				reset_alpha,
				alpha_down,
				connect_movement_with_parent,
				first_reset;

		std::string initLUAFile,
					effectDirectory;

		lua_State	*LUA_init;


		// variables for texture animation
		GLint	current_frame,
				max_frames,
				count_x,
				count_y;

		GLfloat animation_speed,
				animation_step,
				texture_x,
				texture_y,
				texture_w,
				texture_h;

		// pointer to parent emitter
		GLvoid	*parent;
	};
};

#endif // LUA_PARTICLE_H