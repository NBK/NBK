#ifndef SANDBOX_AI_HELPER_H
#define SANDBOX_AI_HELPER_H

#include "AIHelper.h"

namespace AI
{
	class CSandboxAIHelper: public CAIHelper
	{
	public:
		CSandboxAIHelper();
		~CSandboxAIHelper();

		void getUnclaimedBlocks(std::vector<sAbstractBlock> &blocks_);

		int pathFind(cml::vector2i &from, cml::vector2i &to);
		bool pathIsReady(int pathID, std::vector<cml::vector2i> &path_);
	};
};

#endif // SANDBOX_AI_HELPER_H