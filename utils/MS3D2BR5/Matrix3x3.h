#ifndef MATRIX3x3_H
#define MATRIX3x3_H

#include "VectorSimple.h"

class CMatrix3x3
{
public:
	CMatrix3x3();
	~CMatrix3x3();

	GLvoid create_x_rotation_matrix(GLfloat deg_angle);
	GLvoid create_y_rotation_matrix(GLfloat deg_angle);
	GLvoid create_z_rotation_matrix(GLfloat deg_angle);

	CVector operator*(CVector v);

	inline GLfloat deg_to_rad(GLfloat deg);

private:
	GLfloat m[3][3];	
};

#endif // MATRIX3x3_H