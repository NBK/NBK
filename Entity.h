#ifndef ENTITY_H
#define ENTITY_H

#include "system.h"
#include <GL/gl.h>
#include <cml/cml.h>
#include "boundingBox.h"

namespace game_objects
{
	class CEntity
	{
	public:
		CEntity();
		CEntity(CEntity &entity);
		~CEntity();

		virtual GLvoid setPosition(cml::vector3f position);
		virtual GLvoid setRotation(cml::vector3f rotation);
		virtual GLvoid setActive(bool active);

		cml::vector3f	getPosition();
		GLfloat			*getPositionP();
		cml::vector3f	getRotation();
		bool			isActive();

		geometry::sBoundingBox *getBoundingBox();

		GLvoid moveTo();
		GLvoid moveBack();
		GLvoid rotateTo();
		GLvoid rotateBack();

	protected:

		cml::vector3f	position;
		cml::vector3f	rotation;
		bool			active;

		/*
			An entity can use this bbox how ever it wants.
		*/
		geometry::sBoundingBox boundingBox;

		/*
			Every entity has a unique ID. So we can use this ID to sort them,
			group them, identify them...
		*/

		GLint ID;
		static GLint IDCount;
	};
};

#endif // ENTITY_H