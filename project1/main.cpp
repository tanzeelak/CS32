//
//  main.cpp
//  cs32p1
//
//  Created by Tanzeela Khan on 1/6/16.
//  Copyright © 2016 me. All rights reserved.
//

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;
#include "Game.h"


int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);
    
    // Play the game
    g.play();
}



