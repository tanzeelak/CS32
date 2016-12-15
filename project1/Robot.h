//
//  robot.hpp
//  cs32p1
//
//  Created by Tanzeela Khan on 1/6/16.
//  Copyright © 2016 me. All rights reserved.
//

#ifndef Robot_h
#define Robot_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;
#include "globals.h"

class Arena;
class History;

class Robot
{
public:
    // Constructor
    Robot(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool getAttacked(int dir);
    
private:
    Arena* m_arena;
    //History* m_history; //MINE
    int    m_row;
    int    m_col;
    int    m_health;
};
#endif /* robot_hpp */
