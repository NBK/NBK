#include "Entity.h"

using namespace cml;
using namespace geometry;

namespace game_objects
{
	GLint CEntity::IDCount = 0;

	CEntity::CEntity(): position(vector3f(0.0f,0.0f,0.0f)), active(false)
	{
		ID = IDCount++;
	}

	CEntity::CEntity(CEntity &entity)
	{
		this->position=entity.position;
		this->active=entity.active;

		//this->ID=ID;
		ID = IDCount++;
	}

	CEntity::~CEntity()
	{
	}

	GLvoid CEntity::setPosition(vector3f position)
	{
		this->position=position;
	}

	GLvoid CEntity::setActive(bool active)
	{
		this->active=active;
	}

	vector3f CEntity::getPosition()
	{
		return position;
	}

	GLfloat	*CEntity::getPositionP()
	{
		return &position[0];
	}

	bool CEntity::isActive()
	{
		return active;
	}

	sBoundingBox *CEntity::getBoundingBox()
	{
		return &boundingBox;
	}

	GLvoid CEntity::moveTo()
	{
		glTranslatef(position[0],position[1],position[2]);
	}

	GLvoid CEntity::moveBack()
	{
		glTranslatef(-position[0],-position[1],-position[2]);
	}
};