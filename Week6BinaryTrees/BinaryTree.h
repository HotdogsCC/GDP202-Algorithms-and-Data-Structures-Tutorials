#pragma once

template<typename T>
class BinaryTree
{
private:
    class Node
    {
    private:
        T payload; // 
        Node* pLeft; // pointer to the left of the tree
        Node* pRight; // pointer to the left of the tree
    public:
        // constructor intializes payload and sets the left and right pointers to null
        Node(T payload) : payload(payload)
        {
            pLeft = nullptr;
            pRight = nullptr;
        }
        // member functions to get and set the payload
        T GetPayload() const { return payload; }
        void SetPayLoad(T _payload) { payload = _payload; }
        // member functions to get pointers to the left and right tree pointer
        // useful for navigating the tree and deleting nodes
        Node** GetRightPtr() { return &pRight; }
        Node** GetLeftPtr() { return &pLeft; }
        // member functions to pointers to the left and right trees
        Node* GetRight() { return pRight; }
        Node* GetLeft() { return pLeft; }

        void SetLeftPtr(Node** pointer) { pLeft = *pointer; }
    };

    Node* pHead = nullptr;

    void WalkTree(Node* pHead)
    {
        if (pHead == nullptr)
        {
            return;
        }
        WalkTree(pHead->GetLeft());
        std::cout << pHead->GetPayload() << ", ";
        WalkTree(pHead->GetRight());
    }

    Node** findNode(T payloadToFind)
    {
        Node** pCurrent = &pHead;
        int recordsFound = 0;
        while ((*pCurrent) != nullptr)
        {
            if (payloadToFind == (*pCurrent)->GetPayload())
            {
                return pCurrent;
            }
            if (payloadToFind > (*pCurrent)->GetPayload()) pCurrent = (*pCurrent)->GetRightPtr();
            else pCurrent = (*pCurrent)->GetLeftPtr();
        }
        return nullptr;
    }



public:

    BinaryTree()
    {
        pHead = nullptr;
    }

    T GetHead()
    {
        return pHead->GetPayload();
    }

    //puts some item in the correct posiiton
    void insertEntry(T newPayload)
    {
        Node** pCurrent = &pHead; //points to the parent
        while ((*pCurrent) != nullptr) //while there are more children to explore
        {
            //sees if it is greater or less than the value of a node and moves down the correct branch
            //left for smaller, right for larger
            if (newPayload > (*pCurrent)->GetPayload())
            {
                pCurrent = (*pCurrent)->GetRightPtr();
            }
            else
            {
                pCurrent = (*pCurrent)->GetLeftPtr();
            }
        }

        //runs when the end of the branch is found
        //makes the end of a branch point to the new node
        Node* pNewEntry = new Node(newPayload);
        *pCurrent = pNewEntry;
    }

    void WalkTree()
    {
        std::cout << "\n";
        std::cout << "Walk Tree: ";
        WalkTree(pHead);
        std::cout << "\n";
    }

    bool findEntry(T payloadToFind)
    {
        return findNode(payloadToFind) != nullptr;
    }

    bool DeleteEntry(T payload)
    {
        Node** pCurrent = findNode(payload);

        // if pCurrent is Null if we can't find a record to delete
        if (pCurrent == nullptr) return false;

        // If the node to delete has no children then we can delete it imediately and set the pointer to it to null
        if ((*pCurrent)->GetLeft() == nullptr && ((*pCurrent)->GetRight() == nullptr))
        {
            delete (*pCurrent);
            *pCurrent = nullptr;
            return true;
        }
        
        //if there is only children on the right
        if ((*pCurrent)->GetLeft() == nullptr)
        {
            Node* pRight = (*pCurrent)->GetRight();
            delete (*pCurrent);
            *pCurrent = pRight;
            return true;
        }

        std::cout << "ruh roh scoob\n";
        // If we get here then we have a left child so we can't simply delete
        // need to find the biggest on the left childen
        Node** pBiggestLeft = (*pCurrent)->GetLeftPtr();
        while ((*pBiggestLeft)->GetRight() != nullptr)
        {
            pBiggestLeft = (*pBiggestLeft)->GetRightPtr();
        }

        std::cout << "biggest on left is " << (*pBiggestLeft)->GetPayload() << "\n";

        // Copy the content of the found node into the parent node.
        (*pCurrent)->SetPayLoad((*pBiggestLeft)->GetPayload());

        WalkTree();

        // Keep copy of current right so we can link to it after deleting node
        Node* pRight = (*pBiggestLeft)->GetRight();
        if (pRight == nullptr)
        {
            pRight = (*pBiggestLeft)->GetLeft();
        }

        delete(*pBiggestLeft);
        *pBiggestLeft = pRight; // Repoint the parent to the child (it can be nullptr)
        
        return true;

    }

};;