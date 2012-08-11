#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H

#include "Manager.h"
#include "RenderManager.h"
#include "ConsoleListener.h"
#include "BinaryHeap.h"

/* In charge of finding paths between squares */

#define inNeither 0
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
			virtual ~CPathManager();

			// from CManager
			virtual bool init();
			virtual bool update();
			virtual bool shutdown();

			// from CConsoleListener
			virtual std::string onAction(std::string keyword, std::string params);

			virtual bool findPath(int startX, int startY, int endX, int endY, std::vector<cml::vector2i> *path);
			virtual bool findPath(cml::vector2i start, cml::vector2i end, std::vector<cml::vector2i> *path);

			enum HeuristicFormula
			{
				Manhattan           = 1,
				MaxDXDY             = 2,
				DiagonalShortcut    = 3,
				Euclidean           = 4,
				EuclideanNoSQR      = 5,
				Custom1             = 6,
				None                = 7
			};

			void setFormula(HeuristicFormula formula);
			void setHeuristicEstimate(int heuristicEstimate);
			void setPunishDirectionChange(bool punishDirectionChange);
			void setDirectionChangePenalty(int directionChangePenalty);
			void setDiagonalMoves(bool diagonalMoves);
			void setHeavyDiagonals(bool heavyDiagonals);
			void setDiagonalPenalty(int diagonalPenalty);
			void setTieBreaker(bool tieBreaker);
			void setTieBreakerValue(float tieBreakerValue);
			void setSearchLimit(int searchLimit);
			void setAllowWalkOnLava(bool allowWalkOnLava);
			void setAllowEndDiagonal(bool allowEndDiagonal);

			HeuristicFormula getFormula();
			int getHeuristicEstimate();
			bool getPunishDirectionChange();
			int getDirectionChangePenalty();
			bool getDiagonalMoves();
			bool getHeavyDiagonals();
			int getDiagonalPenalty();
			bool getTieBreaker();
			float getTieBreakerValue();
			int getSearchLimit();
			bool getAllowWalkOnLava();
			bool getAllowEndDiagonal();

		private:
			BinaryHeap heap;
			HeuristicFormula formula;
			int heuristicEstimate;
			bool punishDirectionChange;
			int directionChangePenalty;
			bool diagonalMoves;
			bool heavyDiagonals;
			int diagonalPenalty;
			bool tieBreaker;
			float tieBreakerValue;
			int searchLimit;
			bool allowWalkOnLava;
			bool allowEndDiagonal;

			static const int directions[8][2];

			struct cellData
			{
				int OCList;
				int GCost;
				int FCost;
				cml::vector2i Parent;
			};
		};
	};
};

#endif // PATH_MANAGER_H
