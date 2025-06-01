#pragma once
#include <iostream>

template <typename T>
class HashTableBasic
{
public:
    T& operator[](const char* key)
    {
        return pValues[HashFunctionSimple(key)];
    }

    HashTableBasic(unsigned int uInitSize): uSize(uInitSize)
    {
        //makes array for the values
        pValues = new T[uSize];

        //sets all to nullptr
        for(unsigned int i = 0; i < uSize; i++)
        {
            pValues[i] = nullptr;
        }
    }
    ~HashTableBasic()
    {
        //frees memory
        delete[] pValues;
    }

    void DebugDisplayTable()
    {
        std::cout << "\nDisplaying table contents\n";

        for(unsigned int index = 0; index < uSize; index++)
        {
            //print the index this is inferring
            std::cout << index;

            //if there is a value, print it
            if(pValues[index] != nullptr)
            {
                std::cout << ": " << pValues[index];
            }

            //new line
            std::cout << std::endl;
        }
    }

private:
    T* pValues;
    unsigned int uSize;

    unsigned int HashFunctionSimple(const char* key)
    {
        //sum the ascii value of all characters
    
        int sum = 0;
        const char* pChar = key; //pointer to the next character

        //while there are more characters in the string
        while (*pChar != 0)
        {
            sum += *pChar++; //add the next char to the running total
        }

        int uIndex = sum % uSize;
        std::cout << "\nApplying Hash to Key: " << key << " gives index: " << uIndex << std::endl;
        return uIndex;
    }
   
};

template <typename T>
class HashTable2
{
    struct KeyValuePair
    {
        const char* key;
        T value;
    };

    KeyValuePair* pKeyValuePairs;
    unsigned int uSize;
public:
    HashTable2(unsigned int uInitSize) : uSize(uInitSize)
    {
        pKeyValuePairs = new KeyValuePair[uInitSize];
        
        //sets all to nullptr
        for(unsigned int i = 0; i < uSize; i++)
        {
            pKeyValuePairs[i] = nullptr;
        }
    }

    ~HashTable2()
    {
        delete[] pKeyValuePairs;
    }

    bool Set(const char* key, const T value)
    {
        int index = HashFunctionSimple(key);
        const int oldIndex = index;
        //check if the bucket we have hashed already has something in it
        while(pKeyValuePairs[index].key != nullptr)
        {
            index = (index +  1) % uSize;
            //if we checked all the buckets
            if(index == oldIndex)
            {
                std::cout << "\nTable is full\n";
                return false;
            }
        }

        //index is either the hashed one or the next free one
        pKeyValuePairs[index].key = key;
        pKeyValuePairs[index].value = value;
        return true;
    }

    const T operator[](const char* key)
    {
        int index = HashFunctionSimple(key);
        const int oldIndex = index;
        //check if the bucket we have hashed already has something in it
        while(strcmp(key, pKeyValuePairs[index].key) != 0)
        {
            index = (index +  1) % uSize;
            //if we checked all the buckets
            if(index == oldIndex)
            {
                std::cout << "\nCant find the key\n";
                return "";
            }
        }

        //we have found the key
        return pKeyValuePairs[index].value;
    }

private:
    unsigned int HashFunctionSimple(const char* key)
    {
        //sum the ascii value of all characters
    
        int sum = 0;
        const char* pChar = key; //pointer to the next character

        //while there are more characters in the string
        while (*pChar != 0)
        {
            sum += *pChar++; //add the next char to the running total
        }

        int uIndex = sum % uSize;
        std::cout << "\nApplying Hash to Key: " << key << " gives index: " << uIndex << std::endl;
        return uIndex;
    }
};
