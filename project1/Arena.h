//
//  arena.hpp
//  tryagain
//
//  Created by Tanzeela Khan on 1/7/16.
//  Copyright © 2016 me. All rights reserved.
//

#ifndef Arena_hpp
#define Arena_hpp

#include <stdio.h>


#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;
#include "globals.h"
#include "History.h"

/* arena_hpp */
class History;
class Player;
class Robot;

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     nRobotsAt(int r, int c) const;
    bool    determineNewPosition(int& r, int& c, int dir) const;
    void    display() const;
    
    // Mutators
    bool   addRobot(int r, int c);
    bool   addPlayer(int r, int c);
    bool   attackRobotAt(int r, int c, int dir);
    bool   moveRobots();
    History& history();
    
    
private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;
    History m_history;
};


#endif /* arena_hpp */
