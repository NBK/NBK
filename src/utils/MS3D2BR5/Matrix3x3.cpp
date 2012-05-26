#include <windows.h>
#include <gl\gl.h>
#include <math.h>

#include "Matrix3x3.h"

CMatrix3x3::CMatrix3x3()
{
}

CMatrix3x3::~CMatrix3x3()
{
}

GLvoid CMatrix3x3::create_x_rotation_matrix(GLfloat deg_angle)
{
	GLfloat rad_angle=deg_to_rad(deg_angle);

	m[0][0]=cos(rad_angle);
	m[0][1]=0.0f;
	m[0][2]=-sin(rad_angle);

	m[1][0]=0.0f;
	m[1][1]=1.0f;
	m[1][2]=0.0f;

	m[2][0]=sin(rad_angle);
	m[2][1]=0.0f;
	m[2][2]=cos(rad_angle);
}

GLvoid CMatrix3x3::create_y_rotation_matrix(GLfloat deg_angle)
{
	GLfloat rad_angle=deg_to_rad(deg_angle);

	m[0][0]=cos(rad_angle);
	m[0][1]=0.0f;
	m[0][2]=-sin(rad_angle);

	m[1][0]=0.0f;
	m[1][1]=1.0f;
	m[1][2]=0.0f;

	m[2][0]=sin(rad_angle);
	m[2][1]=0.0f;
	m[2][2]=cos(rad_angle);
}

GLvoid CMatrix3x3::create_z_rotation_matrix(GLfloat deg_angle)
{
	GLfloat rad_angle=deg_to_rad(deg_angle);

	m[0][0]=cos(rad_angle);
	m[0][1]=0.0f;
	m[0][2]=-sin(rad_angle);

	m[1][0]=0.0f;
	m[1][1]=1.0f;
	m[1][2]=0.0f;

	m[2][0]=sin(rad_angle);
	m[2][1]=0.0f;
	m[2][2]=cos(rad_angle);
}

inline GLfloat CMatrix3x3::deg_to_rad(GLfloat deg)
{
	return deg*0.017453293f;
}

CVector CMatrix3x3::operator*(CVector v)
{
	return CVector(m[0][0]*v.get_x()+m[0][1]*v.get_y()+m[0][2]*v.get_z(),
				   m[1][0]*v.get_x()+m[1][1]*v.get_y()+m[1][2]*v.get_z(),
				   m[2][0]*v.get_x()+m[2][1]*v.get_y()+m[2][2]*v.get_z());
}