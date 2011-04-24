#include "../commons.h"
#include "DKcommons.h"
#include "JumpLabel.h"
#include <math.h>

using namespace GUI;
using namespace DK_GUI;
using namespace game_utils;

CJumpLabel::CJumpLabel(GLfloat x, GLfloat y, GLint value)
{
	yp=y/(GLfloat)CV_SETTINGS_WINDOW_HEIGHT;
	sy=yp;
	
	label.set_extent(CAbstractGUIItem::EXTENT(x/((GLfloat)CV_SETTINGS_WINDOW_HEIGHT/0.75f),yp,0.0f,0.05f,0.05f));
	label.init(CV_SETTINGS_WINDOW_WIDTH,CV_SETTINGS_WINDOW_HEIGHT);
	label.set_value(value);
	label.set_check_updated_value(false);

	dead=false;
	
	move_up=true;
	move_speed=0.001f;
}

CJumpLabel::~CJumpLabel()
{
}

GLvoid CJumpLabel::draw(GLfloat deltaTime)
{
	if (dead)
	{
		return;
	}

	label.draw();

	label.set_y_pos(yp);

	if (move_up)
	{
		yp-=move_speed*deltaTime;

		if (fabs(yp-sy)>0.2f)
		{
			move_up=false;
		}
	}
	else
	{
		yp+=move_speed*deltaTime;

		if (fabs(yp-sy)<=0.01f)
		{
			dead=true;
		}
	}
}

bool CJumpLabel::is_dead()
{
	return dead;
}