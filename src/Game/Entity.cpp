#include "Entity.h"

using namespace cml;
using namespace geometry;

namespace game_objects
{
	GLint CEntity::IDCount = 0;

	CEntity::CEntity(): position(vector3f(0.0f,0.0f,0.0f)), 
						rotation(vector3f(0.0f,0.0f,0.0f)), 
						active(false)
	{
		ID = IDCount++;
	}

	CEntity::CEntity(CEntity &entity)
	{
		this->position=entity.position;
		this->rotation=entity.rotation;
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

	GLvoid CEntity::setRotation(vector3f rotation)
	{
		this->rotation=rotation;
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

	vector3f CEntity::getRotation()
	{
		return rotation;
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

	GLvoid CEntity::rotateTo()
	{
		glRotatef(rotation[0],1.0f,0.0f,0.0f);
		glRotatef(rotation[1],0.0f,1.0f,0.0f);
		glRotatef(rotation[2],0.0f,0.0f,1.0f);
	}

	GLvoid CEntity::rotateBack()
	{
		glRotatef(-rotation[0],1.0f,0.0f,0.0f);
		glRotatef(-rotation[1],0.0f,1.0f,0.0f);
		glRotatef(-rotation[2],0.0f,0.0f,1.0f);
	}
};