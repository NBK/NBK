#include "Entity.h"

using namespace cml;
using namespace geometry;

namespace game_objects
{
	GLint CEntity::IDCount = 0;

	CEntity::CEntity(): mPosition(vector3f(0.0f,0.0f,0.0f)), 
						mRotation(vector3f(0.0f,0.0f,0.0f)), 
						active(false)
	{
		ID = IDCount++;
	}

	CEntity::CEntity(CEntity &entity)
	{
		this->mPosition=entity.mPosition;
		this->mRotation=entity.mRotation;
		this->active=entity.active;

		//this->ID=ID;
		ID = IDCount++;

		printf("CEntity copy constructor... evil\n");
	}

	CEntity::~CEntity()
	{
	}

	GLvoid CEntity::setPosition(vector3f position)
	{
		this->mPosition=position;
	}

	GLvoid CEntity::setRotation(vector3f rotation)
	{
		this->mRotation=rotation;
	}

	GLvoid CEntity::setActive(bool active)
	{
		this->active=active;
	}

	vector3f CEntity::getPosition()
	{
		return mPosition;
	}

	GLfloat	*CEntity::getPositionP()
	{
		return &mPosition[0];
	}

	vector3f CEntity::getRotation()
	{
		return mRotation;
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
		glTranslatef(mPosition[0],mPosition[1],mPosition[2]);
	}

	GLvoid CEntity::moveBack()
	{
		glTranslatef(-mPosition[0],-mPosition[1],-mPosition[2]);
	}

	GLvoid CEntity::rotateTo()
	{
		glRotatef(mRotation[0],1.0f,0.0f,0.0f);
		glRotatef(mRotation[1],0.0f,1.0f,0.0f);
		glRotatef(mRotation[2],0.0f,0.0f,1.0f);
	}

	GLvoid CEntity::rotateBack()
	{
		glRotatef(-mRotation[0],1.0f,0.0f,0.0f);
		glRotatef(-mRotation[1],0.0f,1.0f,0.0f);
		glRotatef(-mRotation[2],0.0f,0.0f,1.0f);
	}
};