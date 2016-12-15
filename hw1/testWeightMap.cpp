//
//  testWeightMap.cpp
//  cs32hw1
//
//  Created by Tanzeela Khan on 1/19/16.
//  Copyright © 2016 me. All rights reserved.
//

#include "WeightMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
        WeightMap l;
        l.enroll("Emily", 90);
        l.print();
        cout << l.weight("Emily") << endl;
        cout << l.weight("Fred") << endl;
        l.adjustWeight("Emily", 10);
        l.print();
        l.enroll("Bubba", 110);
        cout << l.size() << endl;
        l.print();
}