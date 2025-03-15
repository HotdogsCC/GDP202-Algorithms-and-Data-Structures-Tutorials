#pragma once

#include <cassert>

template<typename DataType, int size>
class MemoryPool
{
public:

	class iterator
	{
	public:
		iterator(DataType* inPtr) : ptr(inPtr) {}//constructor
		
		iterator operator ++ () // increment the pointer
		{
			++ptr;
			return *this;
		}

		iterator operator --() // Decrement the pointer
		{
			--ptr;
			return *this;
		}

		bool operator != (const iterator& other) const // returns true if two iterators are not equal
		{
			return ptr != other.ptr;
		}

		DataType* operator*() //returns current itertor pointer
		{
			return ptr;
		}
	private:
		DataType* ptr;
	};

	MemoryPool()
	{
		pNextFree = poolData;
	}

	// returns an iterator which points at the start of the pool
	iterator begin() { return iterator(poolData);}

	// returns an iterator which points the next element after the memory pool
	iterator end() { return iterator(pNextFree); }

	//takes the next available item from the pool and returns true if it allocated
	bool addItem(DataType& item)
	{
		if (pNextFree < (poolData + size))
		{
			*pNextFree = item;
			++pNextFree;
			return true;
		}
		return false;
	}

	void erase(DataType* item)
	{
		assert(pNextFree > poolData);
		*item = *(pNextFree - 1);
		--pNextFree;
	}

private:
	DataType poolData[size]; //Array for the pool
	DataType* pNextFree = nullptr; //Pointer to the next free item
};

