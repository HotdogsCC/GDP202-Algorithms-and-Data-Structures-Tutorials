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
    // pop everything off the stack..
    while (exampleStack.Size() > 0)
    {
        exampleStack.Pop();
    }
    exampleStack.Debug();
    stackCopy.Debug();


}

int main()
{
    stackExample();
}