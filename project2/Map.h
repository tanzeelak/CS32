//
//  Map.hpp
//  cs32
//
//  Created by Tanzeela Khan on 1/13/16.
//  Copyright © 2016 me. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <stdio.h>
#include <iostream>


typedef  std::string KeyType; //COME BACK
typedef  double ValueType;
const int DEFAULT_MAX_ITEMS = 200;

class Map
{
public:
    Map();         // Create an empty map (i.e., one with no key/value pairs)
    
    Map(const Map& old);
    //When a brand new Map is created as a copy of an existing Map, enough new nodes must be allocated to hold a duplicate of the original list.
    
    Map& operator=(const Map& old);
    //When an existing Map (the left-hand side) is assigned the value of another Map (the right-hand side), the result must be that the left-hand side object is a duplicate of the right-hand side object, with no memory leak of list nodes (i.e. no list node from the old value of the left-hand side should be still allocated yet inaccessible).
    
    bool empty()const;  // Return true if the map is empty, otherwise false.
    
    int size()const;    // Return the number of key/value pairs in the map.
    
    bool insert(const KeyType& key, const ValueType& value);
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).
    
    bool update(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).
    
    bool erase(const KeyType& key);
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    
    bool contains(const KeyType& key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    
    bool get(const KeyType& key, ValueType& value)const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    
    bool get(int i, KeyType& key, ValueType& value)const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.  (See below for details about this function.)
    
    void swap(Map& other);
    // Exchange the contents of this map with the other one.
    
    void dump() const;
    //The intent of this function is that for your own testing purposes, you can call it to print information about the map; we will never call it. You do not have to add this function if you don't want to, but if you do add it, it must not make any changes to the map; if we were to replace your implementation of this function with one that simply returned immediately, your code must still work correctly. The dump function must not write to cout, but it's allowed to write to cerr.
    
    ~Map();
    //When a Map is destroyed, the nodes in the linked list must be deallocated.
    
private:
    class DataMember
    {
    public:
        void setKey(KeyType key);
        ValueType getValue()const;
        void setValue(ValueType value);
        KeyType getKey()const;
        DataMember* nextPtr;
        DataMember* prevPtr;
    private:
        KeyType key;
        ValueType value;
    };
    
    
    DataMember* search(KeyType key) const;
    int currentAmount;
    DataMember* headPtr;
};

bool combine(const Map& m1, const Map& m2, Map& result);
//    When this function returns, result must consist of pairs determined by these rules:
//
//    If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
//    If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain a pair with that key and value.

void subtract(const Map& m1, const Map& m2, Map& result);
//    When this function returns, result must contain a copy of all the pairs in m1 whose keys don't appear in m2; it must not contain any other pairs. (You must not assume result is empty when it is passed in to this function; it may not be.)




#endif /* Map_hpp */
