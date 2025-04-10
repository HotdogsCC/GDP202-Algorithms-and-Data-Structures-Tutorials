// Week6BinaryTrees.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BinaryTree.h"


void BinaryTreeExample()
{
    BinaryTree<int> myBinaryTree;
    myBinaryTree.insertEntry(50);
    myBinaryTree.insertEntry(25);
    myBinaryTree.insertEntry(75);
    myBinaryTree.insertEntry(65);
    myBinaryTree.insertEntry(5);
    myBinaryTree.insertEntry(2);
    myBinaryTree.insertEntry(245);

    std::cout << myBinaryTree.GetHead();
    myBinaryTree.WalkTree();

    std::cout << myBinaryTree.findEntry(4);
    std::cout << myBinaryTree.findEntry(50);
    std::cout << myBinaryTree.findEntry(-1);
    std::cout << myBinaryTree.findEntry(2);

    myBinaryTree.DeleteEntry(245);

    myBinaryTree.WalkTree();

    myBinaryTree.DeleteEntry(25);

    //std::cout << myBinaryTree.GetHead();
    myBinaryTree.WalkTree();
}

int main()
{
    BinaryTreeExample();
}