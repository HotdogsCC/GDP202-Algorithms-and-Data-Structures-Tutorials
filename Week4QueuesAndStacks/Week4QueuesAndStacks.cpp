#include <iostream>
#include <cassert>
#include "Week4QueuesAndStacks.h"

void stackExample()
{
    // instantiate the stack of ints with a max of 10 elements
    Stack<int> exampleStack(10);
    exampleStack.Debug();
    exampleStack.Push(69);
    exampleStack.Push(420);
    exampleStack.Push(14);
    exampleStack.Debug();
    exampleStack.Pop();
    exampleStack.Debug();
    exampleStack.Pop();
    exampleStack.Debug();
    exampleStack.Push(3);
    exampleStack.Debug(); 

    // Copy the stack  
    Stack<int> stackCopy = exampleStack;
    // pop everything off the stack
    while (exampleStack.Size() > 0)
    {
        exampleStack.Pop();
    }
    exampleStack.Debug();
    stackCopy.Debug();


}

void queueExample()
{
    Queue<int, 6> exampleQueue;
    // Instantiate a stack of integers with a maximum of 10 elements
    exampleQueue.Debug();
    exampleQueue.EnQueue(10);
    exampleQueue.EnQueue(15);
    exampleQueue.EnQueue(25);
    exampleQueue.Debug();
    exampleQueue.DeQueue();
    exampleQueue.DeQueue();
    exampleQueue.Debug();
    exampleQueue.EnQueue(30);
    exampleQueue.EnQueue(45);
    exampleQueue.EnQueue(55);
    exampleQueue.EnQueue(60);
    exampleQueue.Debug();
    exampleQueue.DeQueue();
    exampleQueue.DeQueue();
    exampleQueue.Debug();
    exampleQueue.EnQueue(65);
    exampleQueue.EnQueue(75);
    exampleQueue.EnQueue(85);
    exampleQueue.Debug();
}


int main()
{
    //stackExample();
    queueExample();
}