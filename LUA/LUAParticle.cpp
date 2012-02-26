extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include "../commons.h"
#include "../system.h"
#include <GL/gl.h>
#include <math.h>

#include "../utils.h"
#include "LUAParticle.h"
#include "LUAEmitter.h"
#include "lua_common.h"

using namespace cml;
using namespace control;
using namespace game_utils;

#define NOSPAWN if (current_spawn_delay>0.0f) return;

namespace LUA_effects
{
	CLUAParticle *G_currentTempParticle;

	CLUAParticle::CLUAParticle()
	{
		texture_x=0.0f;
		texture_y=0.0f;
		texture_w=1.0f;
		texture_h=1.0f;
		animation_speed=0.0f;
		animation_step=0.0f;
		current_frame=0;
		max_frames=1;
		count_x=1;

		set_start_position(NULL_VEC);
		set_move_vector(NULL_VEC);
		set_color(vector3f(1.0f,1.0f,1.0f));

		set_alpha_properties(1.0f,0.0f);
		set_life_properties(1.0f,0.0f);
		set_current_life(1.0f);
		set_size_properties(1.0f,0.0f);

		set_gravity(NULL_VEC);

		set_reborn(true);
		dead=false;
		set_billboard(true);
		reset_alpha_on_reborn(true);
		alpha_down=true;

		set_texture(0);

		parent=NULL;
		connect_movement_with_parent=false;

		start_spawn_delay=current_spawn_delay=0.0f;
	}

	CLUAParticle::~CLUAParticle()
	{
	}

	// init
	int PARTICLE_SET_START_POSITION(lua_State *L)
	{
		G_currentTempParticle->set_start_position(G_currentTempParticle->get_start_position()+vector3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3)));
		return 0;
	}

	int PARTICLE_SET_MOVE_VECTOR(lua_State *L)
	{
		G_currentTempParticle->set_move_vector(vector3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3)));
		return 0;
	}

	int PARTICLE_SET_LIFE_PROPERTIES(lua_State *L)
	{
		G_currentTempParticle->set_life_properties((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));
		G_currentTempParticle->set_current_life((GLfloat)lua_tonumber(L,3));
		return 0;
	}

	int PARTICLE_SET_REBORN(lua_State *L)
	{
		G_currentTempParticle->set_reborn((GLint)lua_tonumber(L,1)==1);
		return 0;
	}

	int PARTICLE_SET_ALPHA_PROPERTIES(lua_State *L)
	{
		GLfloat connectedWithLife = (GLfloat)lua_tonumber(L,3);
		G_currentTempParticle->set_alpha_properties((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));

		if (connectedWithLife<0.0f)
		{
			GLfloat life_mod = G_currentTempParticle->get_life_mod();
			if (life_mod!=0.0f)
			{
				G_currentTempParticle->set_alpha_mod(G_currentTempParticle->get_start_alpha()/G_currentTempParticle->get_start_life());
			}
		}

		return 0;
	}

	int PARTICLE_SET_SIZE_PROPERTIES(lua_State *L)
	{
		GLfloat connectedWithLife = (GLfloat)lua_tonumber(L,3);
		G_currentTempParticle->set_size_properties((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2));

		if (connectedWithLife<0.0f)
		{
			GLfloat life_mod = G_currentTempParticle->get_life_mod();
			if (life_mod!=0.0f)
			{
				G_currentTempParticle->set_size_mod(G_currentTempParticle->get_start_size()/G_currentTempParticle->get_start_life());
			}
		}

		return 0;
	}

	int PARTICLE_SET_COLOR(lua_State *L)
	{
		G_currentTempParticle->set_color(vector3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3)));
		return 0;
	}

	int PARTICLE_SET_GRAVITY(lua_State *L)
	{
		G_currentTempParticle->set_gravity(vector3f((GLfloat)lua_tonumber(L,1),(GLfloat)lua_tonumber(L,2),(GLfloat)lua_tonumber(L,3)));
		return 0;
	}

	int PARTICLE_SET_RESET_ALPHA(lua_State *L)
	{
		G_currentTempParticle->reset_alpha_on_reborn((GLint)lua_tonumber(L,1)==1);
		return 0;
	}

	int PARTICLE_SET_CONNECT_MOVEMENT_WITH_PARENT(lua_State *L)
	{
		G_currentTempParticle->set_connect_movement_to_parent((GLint)lua_tonumber(L,1)==1);
		return 0;
	}

	int PARTICLE_SET_SPAWN_PROPERTIES(lua_State *L)
	{
		G_currentTempParticle->set_spawn_properties((GLfloat)lua_tonumber(L,1));
		return 0;
	}

	GLvoid CLUAParticle::init(GLint index, GLint count)
	{
		// load initial values from LUA file
		G_currentTempParticle=this;

		first_reset=true;

		// for particle init
		LUA_init = lua_open();
		luaL_openlibs(LUA_init);
		lua_register(LUA_init,"PARTICLE_SET_START_POSITION",PARTICLE_SET_START_POSITION);
		lua_register(LUA_init,"PARTICLE_SET_MOVE_VECTOR",PARTICLE_SET_MOVE_VECTOR);
		lua_register(LUA_init,"PARTICLE_SET_LIFE_PROPERTIES",PARTICLE_SET_LIFE_PROPERTIES);
		lua_register(LUA_init,"PARTICLE_SET_REBORN",PARTICLE_SET_REBORN);
		lua_register(LUA_init,"PARTICLE_SET_ALPHA_PROPERTIES",PARTICLE_SET_ALPHA_PROPERTIES);
		lua_register(LUA_init,"PARTICLE_SET_COLOR",PARTICLE_SET_COLOR);
		lua_register(LUA_init,"PARTICLE_SET_GRAVITY",PARTICLE_SET_GRAVITY);
		lua_register(LUA_init,"PARTICLE_SET_SIZE_PROPERTIES",PARTICLE_SET_SIZE_PROPERTIES);
		lua_register(LUA_init,"PARTICLE_SET_RESET_ALPHA",PARTICLE_SET_RESET_ALPHA);
		lua_register(LUA_init,"PARTICLE_SET_CONNECT_MOVEMENT_WITH_PARENT",PARTICLE_SET_CONNECT_MOVEMENT_WITH_PARENT);
		lua_register(LUA_init,"PARTICLE_SET_SPAWN_PROPERTIES",PARTICLE_SET_SPAWN_PROPERTIES);

		string fullPath = G_startDir+PATH_SEP+initLUAFile;

		__luaL_dofile(LUA_init, fullPath.c_str());
		lua_getglobal(LUA_init,"onParticleInit");
		lua_pushnumber(LUA_init,index);
		lua_pushnumber(LUA_init,count);
		lua_call(LUA_init,2,0);
		lua_close(LUA_init);
	}

#define GETX(i) (i%count_x)
#define GETY(i) (i/count_x)

	GLvoid CLUAParticle::draw()
	{
		NOSPAWN

		vector3f pos = current_position;

		if (connect_movement_with_parent)
		{
			pos+=((CLUAEmitter*)parent)->get_current_position();
		}

		glTranslatef(pos[0],pos[1],pos[2]);

		CCamera *G_camera = CV_GAME_MANAGER->getControlManager()->getCamera();

		if (billboard)
		{
			glRotatef(-G_camera->getRotateZ(),0.0f,0.0f,1.0f);
			glRotatef(-G_camera->getRotateY(),0.0f,1.0f,0.0f);
			glRotatef(-G_camera->getRotateX(),1.0f,0.0f,0.0f);
		}

		GLfloat alpha = current_alpha;

		if (((CLUAEmitter*)parent)->doAlphaConnection())
		{
			alpha = min(alpha,((CLUAEmitter*)parent)->current_alpha);
		}

		glColor4f(color[0],color[1],color[2],alpha);
		GLfloat half_size = current_size/2.0f;

		texture_x = (GLfloat)GETX(current_frame)*texture_w;
		texture_y = (GLfloat)GETY(current_frame)*texture_h;

		glBindTexture(GL_TEXTURE_2D,texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(texture_x,1.0f-texture_y);
			glVertex3f(-half_size,half_size,0.0f);

			glTexCoord2f(texture_x+texture_w,1.0f-texture_y);
			glVertex3f(half_size,half_size,0.0f);

			glTexCoord2f(texture_x+texture_w,1.0f-texture_y-texture_h);
			glVertex3f(half_size,-half_size,0.0f);

			glTexCoord2f(texture_x,1.0f-texture_y-texture_h);
			glVertex3f(-half_size,-half_size,0.0f);
		}
		glEnd();

		if (billboard)
		{
			glRotatef(G_camera->getRotateX(),1.0f,0.0f,0.0f);
			glRotatef(G_camera->getRotateY(),0.0f,1.0f,0.0f);
			glRotatef(G_camera->getRotateZ(),0.0f,0.0f,1.0f);
		}

		glTranslatef(-pos[0],-pos[1],-pos[2]);
	}

	GLvoid CLUAParticle::update()
	{
		GLfloat delta = CV_GAME_MANAGER->getDeltaTime();

		// if there's a spawn delay present we need to decrement it
		if (current_spawn_delay>0.0f)
		{
			current_spawn_delay-=delta*0.001f;

			if (current_spawn_delay<0.0f)
			{
				current_spawn_delay=0.0f;
			}
		}

		NOSPAWN

		// update anim if needed
		if (max_frames!=1)
		{
			animation_step+=animation_speed*delta;

			if (animation_step>=100.0f)
			{
				animation_step=0.0f;
				current_frame++;

				if (current_frame>=max_frames)
				{
					current_frame=0;
				}
			}
		}

		current_position+=current_move_vector*delta;

		current_move_vector+=gravity*delta;

		if (alpha_down)
		{
			current_alpha-=alpha_mod*delta;
		}
		else
		{
			current_alpha+=alpha_mod*delta;
		}

		current_size-=size_mod*delta;
		if (current_size<0.0f)
		{
			current_size=0.0f;
		}

		current_life-=life_mod*delta;
		if (current_life<0.0f)
		{
			current_life=0.0f;
		}

		if ( (current_life<=0.0f) || (alpha_down && current_alpha<0.0f) || (!alpha_down && current_alpha>1.0f) )
		{
			dead=true;
		}

		if (dead && reborn)
		{
			reset();
			dead=false;
		}
	}

	GLvoid CLUAParticle::reset()
	{
		current_position=start_position;
		current_move_vector=start_move_vector;
		current_spawn_delay=start_spawn_delay;
		if (reset_alpha)
		{
			current_alpha=start_alpha;
		}
		else
		{
			alpha_down=!alpha_down;
		}

		if (parent && (((CLUAEmitter*)parent)->doTimeOut() || first_reset))
		{
			first_reset=false;
			current_life=start_current_life;
		}
		else
		{
			current_life=start_life;
		}
		current_size=start_size;
		current_frame=0;
		animation_step=0.0f;
		dead=false;
	}

	GLvoid CLUAParticle::set_start_position(vector3f start_position)
	{
		this->start_position=start_position;
		current_position=start_position;
	}

	GLvoid CLUAParticle::set_start_position_only(vector3f start_position)
	{
		this->start_position=start_position;
	}

	GLvoid CLUAParticle::update_position(vector3f dPos)
	{
		current_position+=dPos;
	}

	GLvoid CLUAParticle::update_move_vector(vector3f dPos)
	{
		current_move_vector+=dPos;
	}

	GLvoid CLUAParticle::set_move_vector(vector3f move_vector)
	{
		current_move_vector=move_vector;
		start_move_vector=move_vector;
	}

	GLvoid CLUAParticle::set_current_move_vector(vector3f move_vector)
	{
		current_move_vector=move_vector;
	}

	GLvoid CLUAParticle::set_color(vector3f color)
	{
		this->color=color;
	}

	GLvoid CLUAParticle::set_alpha_properties(GLfloat start_alpha, GLfloat alpha_mod)
	{
		this->start_alpha=start_alpha;
		this->current_alpha=start_alpha;
		this->alpha_mod=alpha_mod;
	}

	GLvoid CLUAParticle::set_life_properties(GLfloat start_life, GLfloat life_mod)
	{
		this->start_life=start_life;
		this->current_life=start_life;
		this->life_mod=life_mod;

		if  (animation_speed<0.0f)
		{
			// animation is connected with life
			animation_speed=((GLfloat)(max_frames*100)*life_mod)/start_life;
		}
	}

	GLvoid CLUAParticle::set_current_life(GLfloat life)
	{
		this->current_life=life;
		this->start_current_life=life;
	}

	GLvoid CLUAParticle::set_size_properties(GLfloat start_size, GLfloat size_mod)
	{
		this->start_size=start_size;
		this->current_size=start_size;
		this->size_mod=size_mod;
	}

	GLvoid CLUAParticle::set_gravity(vector3f gravity)
	{
		this->gravity=gravity;
	}

	GLvoid CLUAParticle::set_reborn(bool reborn)
	{
		this->reborn=reborn;
	}

	GLvoid CLUAParticle::set_dead(bool dead)
	{
		this->dead=dead;
	}

	bool CLUAParticle::is_dead()
	{
		return dead;
	}

	GLvoid CLUAParticle::set_billboard(bool billboard)
	{
		this->billboard=billboard;
	}

	GLvoid CLUAParticle::reset_alpha_on_reborn(bool reset_alpha)
	{
		this->reset_alpha=reset_alpha;
	}

	vector3f CLUAParticle::get_gravity()
	{
		return gravity;
	}

	vector3f CLUAParticle::get_current_position()
	{
		return current_position;
	}

	vector3f CLUAParticle::get_start_position()
	{
		return start_position;
	}

	vector3f CLUAParticle::get_move()
	{
		return start_move_vector;
	}

	vector3f CLUAParticle::get_current_move()
	{
		return current_move_vector;
	}

	GLvoid CLUAParticle::setLUAFile(string initLUAFile)
	{
		this->initLUAFile=initLUAFile;

		this->initLUAFile=effectDirectory+PATH_SEP+this->initLUAFile;
	}

	GLvoid CLUAParticle::set_alpha_mod(GLfloat alpha_mod)
	{
		this->alpha_mod=alpha_mod;
	}

	GLvoid CLUAParticle::set_size_mod(GLfloat size_mod)
	{
		this->size_mod=size_mod;
	}

	GLfloat CLUAParticle::get_start_size()
	{
		return start_size;
	}

	GLfloat CLUAParticle::get_start_alpha()
	{
		return start_alpha;
	}

	GLfloat CLUAParticle::get_start_life()
	{
		return start_life;
	}

	GLfloat CLUAParticle::get_life_mod()
	{
		return life_mod;
	}

	GLvoid CLUAParticle::setEffectDirectory(string effectDirectory)
	{
		this->effectDirectory=effectDirectory;
	}

	string CLUAParticle::getEffectDirectory()
	{
		return effectDirectory;
	}

	GLvoid CLUAParticle::set_texture(GLuint texture)
	{
		this->texture=texture;
	}

	GLvoid CLUAParticle::set_texture(GLuint texture, GLint count_x, GLint count_y, GLfloat speed)
	{
		this->texture=texture;
		this->count_x=count_x;
		this->count_y=count_y;
		this->max_frames=count_x*count_y;
		this->animation_speed=speed;
		this->texture_w=1.0f/(GLfloat)count_x;
		this->texture_h=1.0f/(GLfloat)count_y;
	}

	GLvoid CLUAParticle::set_parent(GLvoid *parent)
	{
		this->parent=parent;
	}

	GLvoid CLUAParticle::set_connect_movement_to_parent(bool connect)
	{
		this->connect_movement_with_parent=connect;
	}

	GLvoid CLUAParticle::set_spawn_properties(GLfloat start_spawn_delay)
	{
		this->start_spawn_delay=start_spawn_delay;
		this->current_spawn_delay=start_spawn_delay;
	}

	GLfloat CLUAParticle::get_current_spawn_delay()
	{
		return current_spawn_delay;
	}
};