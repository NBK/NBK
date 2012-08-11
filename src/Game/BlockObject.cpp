#include "BlockObject.h"

using namespace std;
using namespace cml;
using namespace loaders;
using namespace LUA_effects;

namespace game_objects
{
namespace block_objects
{
CBlockObject::CBlockObject(string name, vector3f position, CBR5Model *model, GLfloat rotateY): CEntity()	
{
	this->m_name = name;
	this->mPosition = position;
	this->m_model = model;
	this->m_rotateY = rotateY;

	m_effect = NULL;

	m_marked = false;
	m_className = "";
}

CBlockObject::~CBlockObject()
{
}

void CBlockObject::drawModel(GLfloat deltaTime)
{
	if (m_model)
	{
		m_model->draw(deltaTime);
	}
}

void CBlockObject::drawEffect()
{
	if (m_effect)
	{
		m_effect->draw();
	}
}

string CBlockObject::getName()
{
	return m_name;
}

GLfloat CBlockObject::getRotateY()
{
	return m_rotateY;
}

CBR5Model *CBlockObject::getModel()
{
	return m_model;
}

void CBlockObject::setName(std::string name)
{
	this->m_name = name;
}

void CBlockObject::setRotateY(GLfloat rotateY)
{
	this->m_rotateY = rotateY;
}

void CBlockObject::setModel(CBR5Model *model)
{
	this->m_model = model;
}

CLUAEffect *CBlockObject::getEffect()
{
	return m_effect;
}

void CBlockObject::setEffect(CLUAEffect *effect)
{
	this->m_effect = effect;
}

string CBlockObject::getEffectName()
{
	return m_effectName;
}

void CBlockObject::setEffectName(string effectName)
{
	this->m_effectName = effectName;
}

bool CBlockObject::isMarked()
{
	return m_marked;
}

void CBlockObject::setMarked(bool marked)
{
	this->m_marked = marked;
}

void CBlockObject::setClassName(string className)
{
	this->m_className = className;
}

string CBlockObject::getClassName()
{
	return m_className;
}

} //namespace block_objects
} //namespace game_objects
	