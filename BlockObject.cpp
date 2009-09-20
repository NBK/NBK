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
			this->name = name;
			this->position = position;
			this->model = model;
			this->rotateY = rotateY;

			effect = NULL;

			marked = false;
			className = "";
		}

		CBlockObject::~CBlockObject()
		{
		}

		GLvoid CBlockObject::drawModel(GLfloat deltaTime)
		{
			if (model)
			{
				model->draw(deltaTime);
			}
		}

		GLvoid CBlockObject::drawEffect()
		{
			if (effect)
			{
				effect->draw();
			}
		}

		string CBlockObject::getName()
		{
			return name;
		}

		GLfloat CBlockObject::getRotateY()
		{
			return rotateY;
		}

		CBR5Model *CBlockObject::getModel()
		{
			return model;
		}

		GLvoid	CBlockObject::setName(std::string name)
		{
			this->name = name;
		}

		GLvoid CBlockObject::setRotateY(GLfloat rotateY)
		{
			this->rotateY = rotateY;
		}

		GLvoid CBlockObject::setModel(CBR5Model *model)
		{
			this->model = model;
		}

		CLUAEffect *CBlockObject::getEffect()
		{
			return effect;
		}

		GLvoid	CBlockObject::setEffect(CLUAEffect *effect)
		{
			this->effect = effect;
		}

		string CBlockObject::getEffectName()
		{
			return effectName;
		}

		bool CBlockObject::isMarked()
		{
			return marked;
		}

		GLvoid CBlockObject::setMarked(bool marked)
		{
			this->marked = marked;
		}

		GLvoid CBlockObject::setClassName(string className)
		{
			this->className = className;
		}

		string CBlockObject::getClassName()
		{
			return className;
		}
	};
};