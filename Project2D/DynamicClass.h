#pragma once
#include <vector>
#include <cassert>

template<typename DataType>
class DynamicClass
{
public:
	std::vector<DataType*> elementPointerArray;
	auto begin()
	{
		return elementPointerArray.begin(); //update start and end iterators
	}

	auto end()
	{
		return elementPointerArray.end();
	}

	void addItem(const DataType& item)
	{
		DataType* pNewItem = new DataType(item); //create a copy of the new item on the heap
		elementPointerArray.push_back(pNewItem); //add the pointer to the dynamic array
	}

	auto erase(typename std::vector<DataType*>::iterator item)
	{
		// delete the memory for the item of the heap
		assert(*item != nullptr); // dereferencing an iterator returns the value stored in that position
		delete(*item);
		//remove it's pointer from the dynamic array
		// note how erase returns an update iterator which we need to return to avoid memory corruption
		return elementPointerArray.erase(item);

	}
};

