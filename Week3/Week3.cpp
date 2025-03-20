// Week3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

void bubbleSort(std::vector<int>& testData)
{
    for(int i = 0; i < testData.size() - 1; i++)
    {
        bool sorted = true;
        for (int j = 0; j < testData.size() - 1 - i; j++)
        {
            if(testData[j] > testData[j + 1])
            {
                auto temp = testData[j];
                testData[j] = testData[j + 1];
                testData[j + 1] = temp;
                sorted = false;
            }
        }
        if (sorted) return;
    }
}

void insertionSort(std::vector<int>& testData)
{
    int i = 1; // first pivot point
    while (i < testData.size())
    {
        auto currentItem = testData[i]; //get the item at the pivot
        auto j = i;
        while (j > 0 && testData[j-1] > currentItem)
        {
            //shuffle items in the list up one place
            testData[j] = testData[j - 1];
            j--;
        }
        //insert the item from the pivot point into the correrct place in the list
        testData[j] = currentItem;
        i++;
    }
}


void createData(std::vector<int>& testData, int numberElements)
{
    testData.clear(); // clear test data just in case there is already somethign in it
    srand(1); // seed the random number generator so we always get the same sequenec of numbers
    for (int i = 0; i < numberElements; i++)
    {
        testData.push_back(rand()); //add random number to test data
    }
}

bool checkData(std::vector<int>& testData)
{
    for(int i = 0; i < testData.size() - 1; i++)
    {
        if(testData[i] > testData[i + 1])
        {
            return false;
        }
    }
    return true;
}

void SortSpeedTest(int numberElementsToSort)
{
    std::vector<int> testData; //Vector to store our test data in
    enum sortTypes // Enumerated type to  make our code more legible
    {
        BUBBLE,
        INSERTION,
        STL_ALGORITHM
    };
    //Array of names, so we can output a meaningful error message
    const char* sortNames[] = {"Bubble", "Insert", "STL Algorithm"};
    //Repeat for the rest for the diffrent algorithms
    for (int sortType = BUBBLE; sortType <= STL_ALGORITHM; sortType++)
    {
        std::cout << numberElementsToSort;

        createData(testData, numberElementsToSort); //create the test data
        auto startTime = std::chrono::system_clock::now(); // record the current system clock
        switch (sortType) // pick the appropriate sort algorithm and sort our data
        {
        case BUBBLE:
            bubbleSort(testData);
            break;
        case INSERTION:
            insertionSort(testData);
            break;
        case STL_ALGORITHM:
            std::sort(testData.begin(), testData.end());
            break;
        }
        auto endTime = std::chrono::system_clock::now(); // record the current system clock at the time of finishing
        double sortExecutionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.0f;
        if (checkData(testData)) // display algorithm execution time if it sorted the data correct
        {
            std::cout << ", " << sortExecutionTime;
        }
        else
        {
            //display error message
            std::cout << "data not correctly sorted" << sortNames[sortType] << "\n";
        }
    }
    std::cout << '\n';
}

int main()
{
    std::cout << "Running sort tests\n";
    for(int i = 10000; i <= 100000; i+= 10000)
    {
        SortSpeedTest(i);
    }
    std::cout << "Finished\n";
    system("pause");
}