#pragma once

template<typename T>
class Stack
{
    T* data = nullptr;
    unsigned int top = 0;
    unsigned int capacity = 0;
public:
    // constructor which creates a stack with the correct capacity
    Stack(int capacity) : capacity(capacity)
    {
        data = new T[capacity];
    }

    // copy constructor
    Stack(const Stack& other)
    {
        // checks if they are the same
        if (this == &other)
        {
            return;
        }

        // deep copy the data in other to this
        data = new T[other.capacity];
        for (unsigned int i = 0; i < other.capacity; i++)
        {
            data[i] = other.data[i];
        }

        // copies top and capacity indexs
        top = other.top;
        capacity = other.capacity;
    }

    Stack& operator= (const Stack& other)
    {
        // checks if they are the same
        if (this == &other)
        {
            return;
        }

        // deep copy the data in other to this
        data = new T[other.capacity];
        for (unsigned int i = 0; i < other.capacity; i++)
        {
            data[i] = other.data[i];
        }

        // copies top and capacity indexs
        top = other.top;
        capacity = other.capacity;
    }

    // destructor
    ~Stack()
    {
        if (data != nullptr)
        {
            delete[] data;
            data = nullptr;
        }
    }

    // push element onto the top of the stack
    void Push(T newItem)
    {
        std::cout << "Push " << newItem << "\n";
        assert(top < capacity);
        data[top++] = newItem;
    }

    // pop element of the topof the stack
    T Pop()
    {
        std::cout << "Pop\n";
        assert(top != 0);
        return data[--top];
    }

    // get the top element without changing the stack contents
    T Peek()
    {
        assert(top != 0);
        return data[top - 1];
    }

    // debug function so we can see what the stack looks like
    void Debug()
    {
        if (top == 0)
        {
            std::cout << "Stack empty\n";
        }

        std::cout << "Stack contents: ";
        for (unsigned int i = 0; i < top; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
    }

    int Size()
    {
        return top;
    }
};

template<typename T, int capacity>
class Queue
{
    T* pData = nullptr;

    unsigned int front = 0;
    unsigned int rear = capacity - 1;
    unsigned int count = 0;

public:
    Queue()
    {
        pData = new T[capacity];
    }

    Queue(const Queue& other)
    {
        if (this == &other)
        {
            return;
        }

        front = other.front;
        rear = other.rear;
        count = other.count;

        // deep copy
        for (unsigned int i = 0; i < capacity; i++)
        {
            pData[i] = other.pData[i];
        }
    }

    Queue& operator=(const Queue& other)
    {
        if (this == &other)
        {
            return;
        }

        front = other.front;
        rear = other.rear;
        count = other.count;

        // deep copy
        for (unsigned int i = 0; i < capacity; i++)
        {
            pData[i] = other.pData[i];
        }
    }
    
    ~Queue()
    {
        if (pData != nullptr)
        {
            delete[] pData;
            pData = nullptr;
        }
    }

    // util function to return the size of the queue
    int Size()
    {
        return count;
    }


};