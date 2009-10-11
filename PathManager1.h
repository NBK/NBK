#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H

#include "Manager.h"
#include "ConsoleListener.h"
#include "BinaryHeap.h"

/* In charge of finding paths between squares */

#define inOpened 1
#define inClosed 2

namespace game_utils
{
	namespace managers
	{
		class CPathManager: public CManager, public control::CConsoleListener
		{
		public:
			CPathManager();
			~CPathManager();

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

			void CPathManager::findPath(GLint startX, GLint startY, GLint endX, GLint endY, std::vector<cml::vector2i> *path);
			void CPathManager::findPath(cml::vector2i start, cml::vector2i end, std::vector<cml::vector2i> *path);

		private:
			BinaryHeap Heap;

			struct cellData
			{
				int OCList;
				int GCost;
				int HCost;
				int FCost;
				bool isOpen;
				cml::vector2i Parent;
			};
		};
	};
};

#endif // PATH_MANAGER_H