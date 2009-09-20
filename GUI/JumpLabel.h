#ifndef JUMP_LABEL_H
#define JUMP_LABEL_H

#include "MoneyLabel.h"

class CJumpLabel
{
public:
	CJumpLabel(GLfloat x, GLfloat y, GLint value);
	~CJumpLabel();

	GLvoid draw();

	/* when the label has "traveled" enough this becomes true so we can kill it */
	bool is_dead();

private:

	DK_GUI::CMoneyLabel label;

	bool dead, move_up;

	GLfloat yp,sy;
	GLfloat move_speed;

};

#endif // JUMP_LABEL_H