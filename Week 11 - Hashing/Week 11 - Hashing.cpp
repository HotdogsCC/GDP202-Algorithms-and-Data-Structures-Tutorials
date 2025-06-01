// Week 11 - Hashing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "HashTableBasic.h"

int main()
{
    //create the hash map
    HashTableBasic<const char*> myHashTable(8);

    //add some items to it;
    myHashTable["tony"] = "teacher";
    myHashTable["skippy"] = "kangaroo";
    myHashTable["cheddar"] = "cheese";

    //view the table
    myHashTable.DebugDisplayTable();
}
