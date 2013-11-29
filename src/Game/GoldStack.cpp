#include "commons.h"
#include "GoldStack.h"

using namespace cml;
using namespace game_utils;

namespace game_objects
{
	namespace block_objects
	{
		CGoldStack::CGoldStack(vector3f position, CGoldStack::GOLD_STACK_TYPE goldstackType)
			: CBlockObject("goldstack",position,NULL,0.0f)
		{
			this->goldstackType = goldstackType;
			
			setModel(CV_GAME_MANAGER->getResourceManager()->getModel("MODEL_GOLD250"));
			getModel()->scaleToMaxY(0.1);
		}

		CGoldStack::~CGoldStack()
		{

		}

		GLvoid CGoldStack::setGold(GLint amount)
		{
			this->gold = amount;
		}
	}
}