//
//  Map.cpp
//  cs32
//
//  Created by Tanzeela Khan on 1/13/16.
//  Copyright © 2016 me. All rights reserved.
//

#include "Map.h"

Map::Map()         // Create an empty map (i.e., one with no key/value pairs)
{
    currentAmount = 0;
    headPtr = nullptr;
    
}

Map::Map(const Map& old)
{
    headPtr = nullptr;
    DataMember* curr = old.headPtr; //running through old map

    if (curr == nullptr) //old list is empty
    {
        currentAmount = old.size();
        return;
    }
    
    while (curr->nextPtr != nullptr) //running to end of list
    {
        curr = curr->nextPtr;
    }
    
    
    while (curr != nullptr)
    {
        insert(curr->getKey(), curr->getValue()); //insert key/values into our map BACKWORDS from old list
        curr = curr->prevPtr; //for exact copy
    }
    currentAmount = old.size();

}
//When a brand new Map is created as a copy of an existing Map, enough new nodes must be allocated to hold a duplicate of the original list.

Map& Map::operator=(const Map& old)
{
    while (headPtr != nullptr) //destroy our list's nodes
    {
        DataMember* curr = headPtr;
        headPtr = headPtr->nextPtr;
        delete curr;
    }
    
    
    currentAmount = old.currentAmount; //change size of our list
    
    
    Map daNew(old); //call copy constructor
    headPtr = daNew.headPtr; //change our nodes to match
    
    daNew.currentAmount = 0;
    daNew.headPtr = nullptr;
    
    return *this;
}
//When an existing Map (the left-hand side) is assigned the value of another Map (the right-hand side), the result must be that the left-hand side object is a duplicate of the right-hand side object, with no memory leak of list nodes (i.e. no list node from the old value of the left-hand side should be still allocated yet inaccessible).


void Map::DataMember::setKey(KeyType key) //helper functions to access keys/values
{
    this->key = key;
}
ValueType Map::DataMember::getValue()const
{
    return value;
}
void Map::DataMember::setValue(ValueType value)
{
    this->value = value;
}
KeyType Map::DataMember::getKey()const
{
    return key;
}

Map::DataMember* Map::search(KeyType key) const //helper function searches list and returns pointer to node
{
    DataMember* daSearch = headPtr;
    while (daSearch != nullptr)
    {
        if (daSearch->getKey() == key)
        {
            return daSearch;
        }
        daSearch = daSearch->nextPtr;
    }
    return nullptr;
}




bool Map::empty()const  // Return true if the map is empty, otherwise false.
{
    if (currentAmount == 0)
        return true;
    return false;
}
int Map::size()const   // Return the number of key/value pairs in the map.
{
    return currentAmount;
}
bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (headPtr == nullptr) //if null, create new list and assign head
    {
        DataMember* p = new DataMember;
        p->setValue(value); //not deleted
        p->setKey(key);
    
        
        p->nextPtr = nullptr;
        p->prevPtr = nullptr;
        headPtr =  p;

        currentAmount++;
        return true;
    }
    
    
    //SEARCH for key!!!
    DataMember* daSearch = headPtr;
    while (daSearch != nullptr)
    {
        if (daSearch->getKey() == key)
            return false;
        daSearch = daSearch->nextPtr;
    }
        //IT AIN'T IN THE LIST
    DataMember* p = new DataMember;
    p->setValue(value);
    p->setKey(key);
    
    
    p->nextPtr = headPtr; //not head
    p->prevPtr = nullptr;
    headPtr->prevPtr = p;
    headPtr = p;
    
    currentAmount++;
    return true;

}
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).

bool Map::update(const KeyType& key, const ValueType& value)
{
    DataMember* found = search(key);
    if (found == nullptr)
    {
        return  false;
    }
    
    //IT AIN'T IN THE LIST
    found->setValue(value);
    return true;
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    if (update(key, value))
        return true;
    if (insert(key, value))
        return true;
    return false;
}
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).

bool Map::erase(const KeyType& key)
{
    //SEARCH!!!
    //SEARCH for key!!!
    
    DataMember*  found = search(key);
    
    
    if (found != nullptr)
    {
        if (found->prevPtr == nullptr && found->nextPtr == nullptr)
        {
            delete found;
            headPtr = nullptr;
            currentAmount--;
        }
        else if (found->prevPtr == nullptr) //if found is headptr
        {
            found->nextPtr->prevPtr = nullptr;
            headPtr = found->nextPtr;
            delete found;
            currentAmount--;
        }
        else if (found->nextPtr == nullptr) //if found is tailptr
        {
            found->prevPtr->nextPtr = nullptr;
            delete found;
            currentAmount--;
        }
        else //found is in the middle
        {
            
            found->prevPtr->nextPtr = found->nextPtr;
            found->nextPtr->prevPtr = found->prevPtr;
            delete found;
            currentAmount--;
        }
        return true;
    }
    return false;
}
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.

bool Map::contains(const KeyType& key)const
{
    if (search(key) != nullptr)
        return true;
    return false;
}
// Return true if key is equal to a key currently in the map, otherwise
// false.

bool Map::get(const KeyType& key, ValueType& value)const
{
    DataMember* daSearch = headPtr;
    while (daSearch != nullptr) //run through list
    {
        if (daSearch->getKey() == key)
        {
            value = daSearch->getValue();
            return true;
        }
        daSearch = daSearch->nextPtr;
    }
    return false;
}
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.

bool Map::get(int i, KeyType& key, ValueType& value)const
{
    
    if (i >= 0 && i < size())
    {
        
        DataMember* curr = headPtr;
        while (curr->nextPtr != nullptr) //look for last node
        {
            curr = curr->nextPtr;
        }
        
        for (int j = 0; j <i; j++) //go backwards till you get to i'th node
        {
            curr = curr->prevPtr;
        }
        
        key = curr->getKey();
        value = curr->getValue();
        return true;
    }
    return false;
}
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)


//PROBLEM WITH SWAP
void Map::swap(Map& other)
{
    DataMember* tempHeadPtr; //swap headptrs
    tempHeadPtr = headPtr;
    headPtr = other.headPtr;
    other.headPtr = tempHeadPtr;
    
    int tempSize; //swap sizes
    tempSize = currentAmount;
    currentAmount = other.currentAmount;
    other.currentAmount = tempSize;
}
// Exchange the contents of this map with the other one.


bool combine(const Map& m1, const Map& m2, Map& result)
{
    
    bool split = true;
    int i;
    KeyType key;
    ValueType value;
    
    for (i = 0; i < result.size(); i++) //erase result
    {
        result.get(i, key, value);
        result.erase(key);
    }

    for (i = 0; i < m1.size(); i++) //insert all of m1 into result
    {
        m1.get(i, key, value);
        result.insert(key, value);
    }
    
    for (i = 0; i < m2.size(); i++) //running through m2
    {
        m2.get(i, key, value); //get m2's value
        if (result.contains(key)) //if result has m2 key
        {
            ValueType tempValue;
            result.get(key, tempValue); //get result's value
            if (tempValue != value)
            {
                split = false;
                result.erase(key);
                continue;
            }
            else //don't want to change result's list
                continue;
            
        }
        result.insert(key, value);
    }
    
    return split;
}


//    When this function returns, result must consist of pairs determined by these rules:
//
//    If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
//    If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain a pair with that key and value.

void subtract(const Map& m1, const Map& m2, Map& result)
{
    int i;
    KeyType key;
    ValueType value;
    
        //erase result
    for (i = 0; i < result.size(); i++)
    {
        result.get(i, key, value);
        result.erase(key);
    }
    
    //insert all of m1 into result
    for (i = 0; i < m1.size(); i++)
    {
        m1.get(i, key, value);
        result.insert(key, value);
        
    }
    
    //run through m2
    for (i = 0; i < m2.size(); i++)
    {
        m2.get(i, key, value);
        if (result.contains(key)) //if result contains any keys of m2, delete from result
        {
            result.erase(key);
        }
        
    }
    
}
//    When this function returns, result must contain a copy of all the pairs in m1 whose keys don't appear in m2; it must not contain any other pairs. (You must not assume result is empty when it is passed in to this function; it may not be.)


void Map::dump() const
{
    DataMember* daSearch = headPtr;
    while (daSearch != nullptr)
    {
        //std::cerr << daSearch->getKey() << " " << daSearch->getValue() << std::endl;
        daSearch = daSearch->nextPtr;
    }
}
//The intent of this function is that for your own testing purposes, you can call it to print information about the map; we will never call it. You do not have to add this function if you don't want to, but if you do add it, it must not make any changes to the map; if we were to replace your implementation of this function with one that simply returned immediately, your code must still work correctly. The dump function must not write to cout, but it's allowed to write to cerr.


Map::~Map()
{
    DataMember* move = headPtr;
    DataMember* killMe;
    
    while (move != nullptr)
    {
        killMe = move; //killMe will hold move as move goes to the next
        move = killMe->nextPtr;
        delete killMe;
        
    }
}

