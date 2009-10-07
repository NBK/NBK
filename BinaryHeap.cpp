#include "BinaryHeap.h"

//Reset the heap
void BinaryHeap::ResetHeap()
{
	lSize = 0;
    Heap.resize(lSize);
	Heap.reserve(85*85);
}

//Remove the Root Object from the heap
void BinaryHeap::RemoveRoot()
{
	//If only the root exists
	if(lSize <= 1)
	{
		ResetHeap();
		return;
	}
	//First copy the very bottom object to the top
	Heap[1] = Heap[lSize];

	//Resize the count
    lSize -= 1;

	//Shrink the array
    Heap.pop_back();

	//Sort the top item to it's correct position
	int Parent = 1;
	int ChildIndex = 1;

	//Sink the item to it's correct location
    while(true)
	{
		ChildIndex = Parent;
		if(2 * ChildIndex + 1 <= lSize)
		{
			//Find the lowest value of the 2 child nodes
			if (Heap[ChildIndex].Score >= Heap[2 * ChildIndex].Score)
				Parent = 2 * ChildIndex;
			if(Heap[Parent].Score >= Heap[2 * ChildIndex + 1].Score)
				Parent = 2 * ChildIndex + 1;
		}
        else //Just process the one node
		{
			if(2 * ChildIndex <= lSize)
			{
				if(Heap[ChildIndex].Score >= Heap[2 * ChildIndex].Score)
					Parent = 2 * ChildIndex;
			}
		}

		//Swap out the child/parent
		if(Parent != ChildIndex)
		{
			BinHeapData tHeap = Heap[ChildIndex];
			Heap[ChildIndex] = Heap[Parent];
			Heap[Parent] = tHeap;
		}
		else
		{
			return;
		}
	}
}

//Add the new element to the heap
void BinaryHeap::Add(int inScore,int inX, int inY)
{
	//**We will be ignoring the (0) place in the heap array because
	//**it's easier to handle the heap with a base of (1..?)

	//Increment the array count
	lSize += 1;

	//BinHeapData tmp;
	//tmp.Score = inScore;
	//tmp.X = inX;
	//tmp.Y = inY;
	//Heap.resize(lSize);
	Heap[lSize].Score = inScore;
	Heap[lSize].X = inX;
	Heap[lSize].Y = inY;
	//Heap.push_back(tmp);

	//Bubble the item to its correct location
    int sPos = lSize;

	while(sPos != 1)
	{
		if(Heap[sPos].Score <= Heap[sPos / 2].Score)
		{
			BinHeapData tHeap = Heap[sPos / 2];
			Heap[sPos / 2] = Heap[sPos];
			Heap[sPos] = tHeap;
			sPos /= 2;
		}
		else
		{
			return;
		}
	}
}