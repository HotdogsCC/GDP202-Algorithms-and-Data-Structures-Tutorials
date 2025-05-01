#pragma once
#include "BinaryTree.h"

template<typename T>
class BinaryTreeCanonical
{
private:
    unsigned int maxSize; // size of the tree when full
    unsigned int size; // current size of the tree
    T* pData; // pointer to the data array
    
public:

    BinaryTreeCanonical(BinaryTree<T> bTree) // copy tree
    {
        bTree.getCanonicalRepresentation(maxSize, size, pData);
    }
};
