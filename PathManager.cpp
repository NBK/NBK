#include "commons.h"
#include "PathManager.h"

// Console values here
#define DUMMY   "DUMMY"

namespace game_utils
{
	namespace managers
	{
		const int CPathManager::directions[8][2] = {{0,-1} , {1,0}, {0,1}, {-1,0}, {1,-1}, {1,1}, {-1,1}, {-1,-1}};

		CPathManager::CPathManager():	CManager(), CConsoleListener(),
										punishDirectionChange(false),
										directionChangePenalty(100),
										diagonalMoves(true),
										heavyDiagonals(false),
										diagonalPenalty(100),
										searchLimit(85*85),
										formula(None),
										heuristicEstimate(2),
										tieBreaker(true),
										tieBreakerValue(0.001f),
										allowWalkOnLava(false)
		{
		}

		CPathManager::~CPathManager()
		{
		}

		bool CPathManager::init()
		{
			//CLogger::setEntryStart();
			//CLogger::setEntryEnd("\tStarting path finder");

			//CV_GAME_MANAGER->getConsole()->registerClass(this,"PATH MANAGER");
			//CV_GAME_MANAGER->getConsole()->addParam(DUMMY,"() Just here for future use");

			return true;
		}

		bool CPathManager::update()
		{
			return true;
		}

		bool CPathManager::shutdown()
		{
			return true;
		}

		string CPathManager::onAction(string keyword, string params)
		{
			string checkResult = "";
			std::vector<string> tParams;

			if (keyword==DUMMY)
			{
			}

			return "<>";
		}

		void CPathManager::setFormula(HeuristicFormula formula)
		{
			this->formula = formula;
		}

		void CPathManager::setHeuristicEstimate(int heuristicEstimate)
		{
			this->heuristicEstimate = heuristicEstimate;
		}

		void CPathManager::setPunishDirectionChange(bool punishDirectionChange)
		{
			this->punishDirectionChange = punishDirectionChange;
		}

		void CPathManager::setDirectionChangePenalty(int directionChangePenalty)
		{
			this->directionChangePenalty = directionChangePenalty;
		}

		void CPathManager::setDiagonalMoves(bool diagonalMoves)
		{
			this->diagonalMoves = diagonalMoves;
		}

		void CPathManager::setHeavyDiagonals(bool heavyDiagonals)
		{
			this->heavyDiagonals = heavyDiagonals;
		}

		void CPathManager::setDiagonalPenalty(int diagonalPenalty)
		{
			this->diagonalPenalty = diagonalPenalty;
		}

		void CPathManager::setTieBreaker(bool tieBreaker)
		{
			this->tieBreaker = tieBreaker;
		}

		void CPathManager::setTieBreakerValue(float tieBreakerValue)
		{
			this->tieBreakerValue = tieBreakerValue;
		}

		void CPathManager::setSearchLimit(int searchLimit)
		{
			this->searchLimit = searchLimit;
		}

		void CPathManager::setAllowWalkOnLava(bool allowWalkOnLava)
		{
			this->allowWalkOnLava = allowWalkOnLava;
		}

		CPathManager::HeuristicFormula CPathManager::getFormula()
		{
			return formula;
		}
		
		int CPathManager::getHeuristicEstimate()
		{
			return heuristicEstimate;
		}
		
		bool CPathManager::getPunishDirectionChange()
		{
			return punishDirectionChange;
		}

		int CPathManager::getDirectionChangePenalty()
		{
			return directionChangePenalty;
		}
		
		bool CPathManager::getDiagonalMoves()
		{
			return diagonalMoves;
		}
		
		bool CPathManager::getHeavyDiagonals()
		{
			return heavyDiagonals;
		}

		int CPathManager::getDiagonalPenalty()
		{
			return diagonalPenalty;
		}
		
		bool CPathManager::getTieBreaker()
		{
			return tieBreaker;
		}

		float CPathManager::getTieBreakerValue()
		{
			return tieBreakerValue;
		}
		
		int CPathManager::getSearchLimit()
		{
			return searchLimit;
		}

		bool CPathManager::getAllowWalkOnLava()
		{
			return allowWalkOnLava;
		}

		bool CPathManager::findPath(int startX, int startY, int endX, int endY, std::vector<cml::vector2i> *path)
		{
			return findPath(cml::vector2i(startX,startY),cml::vector2i(endX,endY),path);
		}

		bool CPathManager::findPath(cml::vector2i start, cml::vector2i end, std::vector<cml::vector2i> *path)
		{
			//If start is the same as end, set path to end so we walk to the center of the square
			//Skip the finding path function because it's not needed
			if(start==end)
			{
				path->push_back(end);
				return true;
			}

			//reference levelManager as a variable to stop long calls
			CLevelManager *lvlMan = CV_GAME_MANAGER->getLevelManager();

			//If start is a wall, it's not possible to move.
			//Don't check isWalkable because it may be lava
			if(!lvlMan->getBlock(start[0],start[1])->isLow())
				return false;

			//Set variables
			cellData PathMap[85][85];

			int closedNodeCount = 0;
			cml::vector2i currDirection;
			int heuristic;

			bool PathFound = false;
			bool PathHunt = true;

			//Reset the path map
			//No need to reset all the variables.
			//If OCList is set to 0, it bypasses the checks and just sets the variables
			//A quicker way would be to make the inOpened and inClosed variables and increment them by 2
			//A problem would be that it might go round without setting a blocks value though.
			for(int yCnt=0;yCnt<85;yCnt++)
			{
				for(int xCnt=0;xCnt<85;xCnt++)
				{
					PathMap[xCnt][yCnt].OCList=inNeither;
				}
			}

			PathMap[start[0]][start[1]].Parent[0] = start[0];
			PathMap[start[0]][start[1]].Parent[1] = start[1];
			PathMap[start[0]][start[1]].OCList = inOpened;
			PathMap[start[0]][start[1]].FCost = heuristicEstimate;
			PathMap[start[0]][start[1]].GCost = 0;


			heap.ResetHeap();
			heap.Add(PathMap[start[0]][start[1]].FCost,start[0],start[1]);

			while(heap.Count() && PathHunt)
			{
				cml::vector2i currPos = cml::vector2i(heap.GetX(),heap.GetY());
				heap.RemoveRoot();

				cellData *currBlock = &PathMap[currPos[0]][currPos[1]];

				//If cell is not closed (open or neither)
				if(currBlock->OCList != inClosed)
				{
					//If the position is the end, it is the lowest value path. return it
					if(currPos == end)
					{
						currBlock->OCList = inClosed;
						PathHunt = false;
						PathFound = true;
						break;
					}

					//If closedNodeCount is above search limit, no path was found
					if(closedNodeCount > searchLimit)
						return false;

					//If we punish direction changes, we need to know what direction we are travelling
					if(punishDirectionChange)
						currDirection = currPos - currBlock->Parent;
					
					for(int i = 0; i < (diagonalMoves ? 8 : 4); i++)
					{
						cml::vector2i newPos = cml::vector2i(currPos[0]+directions[i][0],currPos[1]+directions[i][1]);
						
						//Is new position outside the bounds, skip
						if(newPos[0] < 0 || newPos[0] > CV_LEVEL_MAP_SIZE || newPos[1] < 0 || newPos[1] > CV_LEVEL_MAP_SIZE)
							continue;

						//If block is not walkable and isn't the end, skip
						if(!lvlMan->getBlock(newPos[0], newPos[1])->isWalkable(allowWalkOnLava) && newPos != end)
							continue;

						//If this is a diagonal, check that we're not cutting a wall
						if(i > 3)
							if(!lvlMan->getBlock(newPos[0],currPos[1])->isWalkable(allowWalkOnLava) || !lvlMan->getBlock(currPos[0],newPos[1])->isWalkable(allowWalkOnLava))
								continue;

						//Define a variable to hold the new GCost
						int newGCost;

						//Check if diagonal (diagonals should have sqrt(2) value due to pythagoras' theorum
						//Also add diagonal penalty if heavy diagonals
						if(i > 3)
							newGCost = PathMap[currPos[0]][currPos[1]].GCost + 141 + (heavyDiagonals ? diagonalPenalty : 0);
						else
                            newGCost = PathMap[currPos[0]][currPos[1]].GCost + 100;

						//If we're punishing direction, check it and add penalty accordingly
						if(punishDirectionChange)
						{
							if(newPos-currPos != currDirection)
							{
								newGCost += directionChangePenalty;
							}
						}

						if(PathMap[newPos[0]][newPos[1]].OCList != inNeither)
							if(PathMap[newPos[0]][newPos[1]].GCost <= newGCost)
								continue;

						PathMap[newPos[0]][newPos[1]].Parent = currPos;
						PathMap[newPos[0]][newPos[1]].GCost = newGCost;

						int hDiagonal;
						int hStraight;
						int hOrthogonal;
						cml::vector2i dxy;

						switch(formula)
						{
							case Manhattan:
								heuristic = heuristicEstimate * (abs(newPos[0] - end[0]) + abs(newPos[1] - end[1]));
								break;
							case MaxDXDY:
								heuristic = heuristicEstimate * (max(abs(newPos[0] - end[0]), abs(newPos[1] - end[1])));
								break;
							case DiagonalShortcut:
								hDiagonal  = min(abs(newPos[0] - end[0]), abs(newPos[1] - end[1]));
                                hStraight  = (abs(newPos[0] - end[0]) + abs(newPos[1] - end[1]));
								heuristic = (heuristicEstimate * 2) * hDiagonal + heuristicEstimate * (hStraight - 2 * hDiagonal);
								break;
							case Euclidean:
								heuristic = (int) (heuristicEstimate * sqrt(pow((float)(newPos[0] - end[0]) , 2) + pow((float)(newPos[1] - end[1]), 2)));
								break;
							case EuclideanNoSQR:
								heuristic = (int) (heuristicEstimate * (pow((float)(newPos[0] - end[0]) , 2) + pow((float)(newPos[1] - end[1]), 2)));
								break;
							case Custom1:
								dxy         = cml::vector2i(abs(end[0] - newPos[0]), abs(end[1] - newPos[1]));
                                hOrthogonal  = abs(dxy[0] - dxy[1]);
                                hDiagonal    = abs(((dxy[0] + dxy[1]) - hOrthogonal) / 2);
                                heuristic = heuristicEstimate * (hDiagonal + hOrthogonal + dxy[0] + dxy[1]);
                                break;
							default:
							case None:
								heuristic = 0;
								break;
						}

						if (tieBreaker)
                        {
                            int dx1 = currPos[0] - end[0];
                            int dy1 = currPos[1] - end[1];
                            int dx2 = start[0] - end[0];
                            int dy2 = start[1] - end[1];
                            int cross = abs(dx1 * dy2 - dx2 * dy1);
                            heuristic += (int)(cross * tieBreakerValue);
                        }

						PathMap[newPos[0]][newPos[1]].FCost = newGCost + heuristic;
						PathMap[newPos[0]][newPos[1]].OCList = inOpened;
						heap.Add(PathMap[newPos[0]][newPos[1]].FCost, newPos[0], newPos[1]);
					}

					closedNodeCount++;
					PathMap[currPos[0]][currPos[1]].OCList = inClosed;
				}
			}

			if(PathFound)
			{
				cml::vector2i t = end;
				if(lvlMan->getBlock(t[0],t[1])->isLow())
					path->push_back(t);
				while(true)
				{
					path->push_back(PathMap[t[0]][t[1]].Parent);
					t = PathMap[t[0]][t[1]].Parent;
					if(t == start)
						return true;
				}
			}
			return false;
		}
	}
}