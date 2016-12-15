//
//  WeightMap.cpp
//  cs32
//
//  Created by Tanzeela Khan on 1/15/16.
//  Copyright © 2016 me. All rights reserved.
//

#include "WeightMap.h"

WeightMap::WeightMap()       // Create an empty weight map.
{
    w_currentAmount = 0;
    //std::cerr << "pls" << std::endl;
}

bool WeightMap::enroll(KeyType name, ValueType startWeight)
{
    if (m_map.contains(name) && startWeight <= 0 && w_currentAmount != DEFAULT_MAX_ITEMS)
        return false;
    m_map.insert(name, startWeight);
    w_currentAmount++;
    return true;
}
// If a person with the given name is not currently in the map,
// there is room in the map, and the startWeight is not negative,
// add an entry for that person and weight and return true.
// Otherwise make no change to the map and return false.

ValueType WeightMap::weight(KeyType name) const
{
    const KeyType version = name;
    ValueType v;
    if (m_map.contains(version))
    {
        m_map.get(version, v);
        return v;
    }
    return -1;
}
// If a person with the given name is in the map, return that
// person's weight; otherwise, return -1.

bool WeightMap::adjustWeight(KeyType name, ValueType amt)
{
    if (!m_map.contains(name) && weight(name)+amt <0)
        return false;
    double newAmount = amt+weight(name);
    m_map.erase(name);
    m_map.insert(name, newAmount);
    return true;
}
// If no person with the given name is in the map or if
// weight() + amt is negative, make no change to the map and return
// false.  Otherwise, change the weight of the indicated person by
// the given amount and return true.  For example, if amt is -8.2,
// the person loses 8.2 pounds; if it's 3.7, the person gains 3.7
// pounds.

int WeightMap::size() const
{
    return w_currentAmount;
}// Return the number of people in the WeightMap.

void WeightMap::print() const
{
    KeyType key;
    ValueType value;
    for (int i = 0; i < w_currentAmount; i++)
    {
        m_map.get(i, key, value);
        std::cout << key << " " << value << std::endl;
    }
}
// Write to cout one line for every person in the map.  Each line
// has the person's name, followed by one space, followed by that
// person's weight.
