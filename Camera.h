#ifndef SIMPLE_CAMERA_H
#define SIMPLE_CAMERA_H

#include "system.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cml/cml.h>
#include "DeltaTime.h"
#include <string>
#include "Entity.h"

namespace control
{
	class CCamera: public game_objects::CEntity
	{
	public:
		CCamera(void);
		~CCamera(void);

		GLvoid transformView();

		//GLvoid setPosition(cml::vector3f position);
		GLvoid move(cml::vector3f vec);
		GLvoid moveForward(GLfloat a);
		GLvoid strafeLeft(GLfloat a);

		GLvoid rotateX(GLfloat a);
		GLvoid rotateY(GLfloat a);
		GLvoid rotateZ(GLfloat a);
		GLvoid setPosScale(GLfloat posScale);

		GLfloat getRotateX();
		GLfloat getRotateY();
		GLfloat getRotateZ();

		//cml::vector3f getPosition();
		cml::vector3f getTargetPoint();
		cml::vector3f getDirection();
		cml::vector3f getUpVector();

		GLvoid rotateAroundTargetOnY(cml::vector3f target, GLfloat angle);
		GLvoid rotateAroundTargetOnX(cml::vector3f target, GLfloat angle);

		GLvoid pushPosition();
		GLvoid popPosition();

		GLvoid setFollowPath(std::string pathFile);

		GLvoid update(CDeltaTime *deltaTime);

		std::vector<cml::vector3f> *getPathPoints();

	private:
		cml::vector3f	direction,
						up,
						tempPosition;

		GLint normAll;

		cml::vector3f rotateAroundArbitrary(const cml::vector3f &v, cml::vector3f &axis, GLfloat angle);

		GLfloat posScale,
				tmp_angle_x,
				tmp_angle_y,
				tmp_angle_z;

		std::vector<cml::vector3f> path;
		std::vector<cml::vector3f>::iterator pathPos;

		GLvoid followPath(CDeltaTime *deltaTime);
	};
};

#endif // SIMPLE_CAMERA_H