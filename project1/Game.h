//
//  game.hpp
//  cs32p1
//
//  Created by Tanzeela Khan on 1/6/16.
//  Copyright © 2016 me. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;


class Arena;
//class History;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();
    
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
};

#endif /* game_hpp */
