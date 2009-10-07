#include "commons.h"
#include "PathManager.h"
#include "BinaryHeap.h"

using namespace utils;

// Console values here
#define DUMMY   "DUMMY"

namespace game_utils
{
	namespace managers
	{
		CPathManager::CPathManager(): CManager(), CConsoleListener()
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

		void CPathManager::findPath(cml::vector2i start, cml::vector2i end, std::vector<cml::vector2i> *path)
		{
			return findPath(start[0],start[1],end[0],end[1],path);
		}

		void CPathManager::findPath(GLint startX, GLint startY, GLint endX, GLint endY, std::vector<cml::vector2i> *path)
		{
			int xCnt, yCnt;	
			int ParentX, ParentY;

			cellData PathMap[85][85];

//Make sure the starting point and ending point are not the same
			if((startX == endX) && (startY == endY))
				return;

			CLevelManager *lvlMan = CV_GAME_MANAGER->getLevelManager();
//Make sure the starting/ending point is not a wall
			if(!lvlMan->getBlock(startX,startY)->isLow())
				return;

//Set the flags
			bool PathFound = false;
			bool PathHunt = true;

//Put the starting point on the open list
			for(yCnt=0;yCnt<85;yCnt++)
			{
				for(xCnt=0;xCnt<85;xCnt++)
				{
					PathMap[xCnt][yCnt].FCost=0;
					PathMap[xCnt][yCnt].GCost=0;
					PathMap[xCnt][yCnt].HCost=0;
					PathMap[xCnt][yCnt].OCList=0;
					PathMap[xCnt][yCnt].Parent[0]=0;
					PathMap[xCnt][yCnt].Parent[1]=0;
				}
			}
			Heap.ResetHeap();
			PathMap[startX][startY].OCList = inOpened;
			Heap.Add(0, startX, startY);

//Find the children
			while(PathHunt)
			{
				if(Heap.Count() != 0)
				{
//Get the parent node
					ParentX = Heap.GetX();
					ParentY = Heap.GetY();

//Remove the root
					PathMap[ParentX][ParentY].OCList = inClosed;
					Heap.RemoveRoot();

//Find the available children to add to the open list
					for(yCnt=ParentY - 1;yCnt<=ParentY + 1;yCnt+=1)
					{
						for(xCnt=ParentX - 1;xCnt<=ParentX + 1;xCnt+=1)
						{
//Make sure we are not out of bounds
							if(xCnt != -1 && xCnt != 85 + 1 && yCnt != -1 && yCnt <= 85)
							{
//Make sure it's not on the closed list
								if(PathMap[xCnt][yCnt].OCList != inClosed)
								{
//Make sure no wall
									if(lvlMan->getBlock(xCnt,yCnt)->isWalkable(false) || (xCnt == endX && yCnt == endY))
									{
//Don't cut across corners
										bool CanWalk = true;
										if(xCnt == ParentX - 1)
										{
											if(yCnt == ParentY - 1)
											{
												if(!lvlMan->getBlock(ParentX - 1,ParentY)->isWalkable(false) || !lvlMan->getBlock(ParentX,ParentY - 1)->isWalkable(false))
													CanWalk = false;
											}
											else if(yCnt == ParentY + 1)
											{
												if(!lvlMan->getBlock(ParentX,ParentY + 1)->isWalkable(false) || !lvlMan->getBlock(ParentX - 1,ParentY)->isWalkable(false))
													CanWalk = false;
											}
										}
										else if(xCnt == ParentX + 1)
										{
											if(yCnt == ParentY - 1)
											{
												if(!lvlMan->getBlock(ParentX,ParentY - 1)->isWalkable(false) || !lvlMan->getBlock(ParentX + 1,ParentY)->isWalkable(false))
													CanWalk = false;
											}
											else if(yCnt == ParentY + 1)
											{
												if(!lvlMan->getBlock(ParentX + 1,ParentY)->isWalkable(false) || !lvlMan->getBlock(ParentX,ParentY + 1)->isWalkable(false))
													CanWalk = false;
											}
										}
//If we can move this way
										if(CanWalk)
										{
											if(PathMap[xCnt][yCnt].OCList != inOpened)
											{
//Calculate the GCost
												if(std::abs(xCnt - ParentX) == 1 && std::abs(yCnt - ParentY) == 1)
													PathMap[xCnt][yCnt].GCost = PathMap[ParentX][ParentY].GCost + 14;
												else
													PathMap[xCnt][yCnt].GCost = PathMap[ParentX][ParentY].GCost + 10;

//Calculate the HCost
												PathMap[xCnt][yCnt].HCost = 10 * (std::abs(xCnt - endX) + std::abs(yCnt - endY));
												PathMap[xCnt][yCnt].FCost = (PathMap[xCnt][yCnt].GCost + PathMap[xCnt][yCnt].HCost);

//Add the parent value
												PathMap[xCnt][yCnt].Parent[0] = ParentX;
												PathMap[xCnt][yCnt].Parent[1] = ParentY;

//Add the item to the heap
												Heap.Add(PathMap[xCnt][yCnt].FCost, xCnt, yCnt);

//Add the item to the open list
												PathMap[xCnt][yCnt].OCList = inOpened;
											}
											else
											{
//We will check for better value
												int AddedGCost;
												if(std::abs(xCnt - ParentX) == 1 && std::abs(yCnt - ParentY) == 1)
													AddedGCost = 14;
												else
													AddedGCost = 10;
												int tempCost = PathMap[ParentX][ParentY].GCost + AddedGCost;

												if(tempCost < PathMap[xCnt][yCnt].GCost)
												{
													PathMap[xCnt][yCnt].GCost = tempCost;
													PathMap[xCnt][yCnt].Parent[0] = ParentX;
													PathMap[xCnt][yCnt].Parent[1] = ParentY;
													if(PathMap[xCnt][yCnt].OCList == inOpened)
													{
														int NewCost = PathMap[xCnt][yCnt].HCost + PathMap[xCnt][yCnt].GCost;
														Heap.Add(NewCost, xCnt, yCnt);
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
				else
				{
					PathFound = false;
					PathHunt = false;
					return;
				}

//If we find a path
				if(PathMap[endX][endY].OCList == inOpened)
				{
					PathFound = true;
					PathHunt = false;
				}
			}

			if(PathFound)
			{
				int tX = endX;
				int tY = endY;
				int sX;
				int sY;
				if(lvlMan->getBlock(tX,tY)->isLow())
					path->push_back(cml::vector2i(tX,tY));
				while(true)
				{
					sX = tX;
					sY = tY;
					path->push_back(PathMap[sX][sY].Parent);
					tX = PathMap[sX][sY].Parent[0];
					tY = PathMap[sX][sY].Parent[1];
					if(tX == startX && tY == startY)
						return;
				}
			}
			return;
		}
	}
}
