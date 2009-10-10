#include "commons.h"
#include "Creature.h"

using namespace game_utils;
using namespace std;
using namespace loaders;
using namespace cml;

namespace game_objects
{
	CCreature::CCreature(): CEntity()
	{
		currentAction = AA_WALK;
		model = NULL;
		name = "";
		moveSpeed = 1.0f;
		moveVector = vector3f(0.0f,0.0f,0.0f);
	}

	CCreature::~CCreature()
	{
		if (model)
		{
			CV_GAME_MANAGER->getResourceManager()->returnModel(name,model);
		}
	}

	GLvoid CCreature::setName(string name)
	{
		this->name = name;
	}

	GLvoid CCreature::setModel(loaders::CBR5Model *model)
	{
		this->model = model;
		this->model->setInterpolate(true);
		this->model->setConnected(true);
	}

	GLvoid CCreature::setAction(GLint action, GLint startFrame, GLint endFrame)
	{
		actions[action] = model->registerAction(startFrame,endFrame);
	}

	GLvoid CCreature::useAction(GLint action)
	{
		this->currentAction = action;

		if (model)
		{
			model->doAction(actions[action]);
		}
	}

	GLvoid CCreature::draw(GLfloat deltaTime)
	{
		if (model)
		{
			CEntity::moveTo();
			CEntity::rotateTo();
			model->draw(deltaTime);
			CEntity::rotateBack();
			CEntity::moveBack();
		}
	}

	CBR5Model *CCreature::getModel()
	{
		return model;
	}

	string CCreature::getName()
	{
		return name;
	}

	GLvoid CCreature::update(GLfloat deltaTime)
	{
	}
};