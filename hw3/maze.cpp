//
//  maze.cpp
//  cs32hw3
//
//  Created by Tanzeela Khan on 2/4/16.
//  Copyright © 2016 me. All rights reserved.
//
//PROBLEM 3
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

const char OPEN = '.';

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
    {
        return true;
    }
    
    if (maze[sr][sc] == 'f')
        return false;
    
    
    maze[sr][sc] = 'f';
    
    if (!(maze[sr-1][sc] == OPEN || maze[sr+1][sc] == OPEN || maze[sr][sc+1] == OPEN|| maze[sr][sc-1] == OPEN))
        return false;
    
    
    if (maze[sr-1][sc] == OPEN)
    {
        if (pathExists(maze, nRows, nCols, sr-1, sc, er, ec) == true)
            return true;
        
    }
    if (maze[sr+1][sc] == OPEN)
    {
        if (pathExists(maze, nRows, nCols, sr+1, sc, er, ec) == true)
            return true;
    }
    if (maze[sr][sc-1] == OPEN)
    {
        if (pathExists(maze, nRows, nCols, sr, sc-1, er, ec) == true)
            return true;
    }
    if (maze[sr][sc+1] == OPEN)
    {
        if (pathExists(maze, nRows, nCols, sr, sc+1, er, ec) == true)
            return true;
    }
    return false;
    //If the start location is equal to the ending location, then we've
    //solved the maze, so return true.
    //Mark the start location as visted.
    //For each of the four directions,
    //If the location one step in that direction (from the start
    //                                            location) is unvisited,
    //then call pathExists starting from that location (and
    //                                                  ending at the same ending location as in the
    //                                                  current call).
    //If that returned true,
    //then return true.
    //Return false.
    
}