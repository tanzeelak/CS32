//
//  Map.cpp
//  cs32
//
//  Created by Tanzeela Khan on 1/13/16.
//  Copyright © 2016 me. All rights reserved.
//

#include "newMap.h"

Map::Map()         // Create an empty map (i.e., one with no key/value pairs)
{
    array = new DataMember[DEFAULT_MAX_ITEMS];
    currentAmount = 0;
    linkMax = DEFAULT_MAX_ITEMS;
}
Map::Map(int newAmt)
{
    if (newAmt <=0)
        linkMax = DEFAULT_MAX_ITEMS;
    else
        linkMax = newAmt;
    array = new DataMember[linkMax];
    currentAmount = 0;
}

void Map::DataMember::setKey(KeyType key)
{
    this->key = key;
}
ValueType Map::DataMember::getValue()
{
    return value;
}
void Map::DataMember::setValue(ValueType value)
{
    this->value = value;
}
KeyType Map::DataMember::getKey()
{
    return key;
}

Map::Map(const Map& old)
{
    array = new DataMember[old.linkMax];
    currentAmount = old.currentAmount;
    linkMax = old.linkMax;
    for (int i = 0; i < old.size(); i++)
    {
        array[i].setKey(old.array[i].getKey());
        array[i].setValue(old.array[i].getValue());
    }
}
//createws a new object and makes a copy of the existing object. uses existing object to intiialize it


Map& Map::operator=(const Map& old)
{
    array = new DataMember[old.linkMax];
    currentAmount = old.currentAmount;
    linkMax = old.linkMax;
    for (int i = 0; i < old.size(); i++)
    {
        array[i].setKey(old.array[i].getKey());
        array[i].setValue(old.array[i].getValue());
    }
    return *this;
}
//takes two already existing objects and assigns one to another

bool Map::empty()const  // Return true if the map is empty, otherwise false.
{
    if (currentAmount == 0)
        return true;
    return false;
}
int Map::size()const   // Return the number of getKey()/value pairs in the map.
{
    return currentAmount;
}
bool Map::insert(const KeyType& key, const ValueType& value)
{
    if (currentAmount == linkMax)
        return false;
    for (int i = 0; i < currentAmount; i++)
    {
        if ((array+i)->getKey() == key)
            return false;
    }
    array[currentAmount].setKey(key);
    (array+currentAmount)->setValue(value);
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
    int i;
    for (i = 0; i < currentAmount; i++)
    {
        if ((array+i)->getKey() ==(key))
            break;
    }
    if (i == currentAmount)
        return false;
    (array+i)->setValue(value);
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
    int i;
    for (i = 0; i < currentAmount; i++)
    {
        if ((array+i)->getKey() == key)
            break;
    }
    if ((array+i)->getKey() != key)
        return false;
    
    //YOU FOUND THE KEY TO ERASE
    
    
    (array+i)->setKey((array+currentAmount-1)->getKey());
    (array+i)->setValue((array+currentAmount-1)->getValue());
    
    currentAmount--;
    return true;
}
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.

bool Map::contains(const KeyType& key)const
{
    for (int i = 0; i < currentAmount; i++)
    {
        if (array[i].getKey() == key)
            return true;
    }
    return false;
}
// Return true if key is equal to a key currently in the map, otherwise
// false.

bool Map::get(const KeyType& key, ValueType& value)const
{
    int i;
    for (i = 0; i < currentAmount; i++)
    {
        if ((array+i)->getKey() == key)
            break;
    }
    if ((array+i)->getKey() != key)
    {
        
        return false;
    }
    value = (array+i)->getValue();
    return true;
}
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.

bool Map::get(int i, KeyType& key, ValueType& value)const
{
    if (i >= 0 && i < size())
    {
        key = array[i].getKey();
        value = array[i].getValue();
        return true;
    }
    return false;
}
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)

void Map::swap(Map& other)
{
    
    DataMember* temp = new DataMember;
    
    for (int i = 0; i < linkMax; i++)
    {
        temp[i] = this->array[i];
        
    }
    for (int i = 0; i < linkMax; i++) //MAKE MORE EFFICIENT problem
    {
        this->array[i] = other.array[i];
    }
    for (int i = 0; i < linkMax; i++)
    {
        other.array[i] = temp[i];
    }
    
    int tempAmt = this->currentAmount;
    this->currentAmount = other.currentAmount;
    other.currentAmount = tempAmt;
    
    int tempCap = this->linkMax;
    this->linkMax = other.linkMax;
    other.linkMax = tempCap;
    delete temp;
}
// Exchange the contents of this map with the other one.

Map::~Map()
{
    delete[] array;
}

