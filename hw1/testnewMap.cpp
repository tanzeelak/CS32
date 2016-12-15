


//
//  testNewMap.cpp
//  cs32hw1
//
//  Created by Tanzeela Khan on 1/16/16.
//  Copyright © 2016 me. All rights reserved.
//

#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map a(12);
    a.insert("heyo", 4);
    std::cout << a.insert("hey", 3)<<std::endl; //1
    std::cout << a.insert("heyo", 5) <<std::endl; //0
    std::cout << a.update("heyihj", 5) <<std:: endl; //0
    std::cout << a.insert("heyo", 4) <<std::endl; //0
    std::cout << a.insertOrUpdate("heyoewf",4) << std::endl; //1
    cout << a.erase("ghjlghg") << endl; //0
    cout << a.contains("") << endl; //0
    std::cout << a.size() <<std::endl;

}


