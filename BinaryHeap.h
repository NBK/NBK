#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>

class BinaryHeap
{
public:
	struct BinHeapData
	{
        //Data Structure to hold FScore and Location
        int Score;
        int X;
        int Y;
	};
private:
	int lSize; //Size of the heap array
    std::vector<BinHeapData> Heap; //Heap Array

public:

	//Return heap count
	int Count() {return lSize;};

	//Return X position
	int GetX() {return Heap[1].X;};

	//Return Y position
	int GetY() {return Heap[1].Y;};

	//Return the lowest score
	int GetScore() {return Heap[1].Score;};

	//Reset the heap
	void ResetHeap();

	//Remove the Root Object from the heap
	void RemoveRoot();

	//Add the new element to the heap
	void Add(int inScore, int inX, int inY);
};
#endif