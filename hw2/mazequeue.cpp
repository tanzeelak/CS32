//
//  mazequeue.cpp
//  hw2
//
//  Created by Tanzeela Khan on 1/27/16.
//  Copyright © 2016 me. All rights reserved.
//
//NUMBER 3
#include <string>
#include <queue>
#include <iostream>
using namespace std;

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

queue<Coord> coordQueue; //change
//queue<char> directions; //change


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (!(maze[sr][sc] == '.' && maze[er][ec] == '.'))
        return  false;
    coordQueue.push(Coord(sr,sc)); //change
    
    maze[sr][sc] = 'f';
    //    Push the starting coordinate (sr,sc) onto the coordinate stack and
    //    update maze[sr][sc] to indicate that the algorithm has encountered
    //    it (i.e., set maze[sr][sc] to have a value other than '.').
    
    int r = sr, c = sc;
    
    while (!coordQueue.empty()) //    While the queue is not empty,
    {
        //  {   Pop the top coordinate off the stack. This gives you the current
            //        (r,c) location that your algorithm is exploring.

        Coord curr = coordQueue.front(); //change
        r = curr.r(), c = curr.c();
        
        coordQueue.pop(); //change
        
        //        If the current (r,c) coordinate is equal to the ending coordinate,
        //        then we've solved the maze so return true!
        
        if (r == er && c == ec)
        {
            return  true;
        }
        
        //        Check each place you can move from the current cell as follows:
        //        If you can move NORTH and haven't encountered that cell yet,
        //        then push the coordinate (r-1,c) onto the stack and update
        //        maze[r-1][c] to indicate the algorithm has encountered it.
        if ((r-1) > 0 && maze[r-1][c] == '.') //move north
        {
            coordQueue.push(Coord(r-1,c));
            //directions.push('n');
            maze[r-1][c] = 'f';
            
        }
        if ((c+1) < (nCols-1) && maze[r][c+1] == '.') //move east //CHANGED else if
        {
            coordQueue.push(Coord(r,c+1));
            //directions.push('e');
            maze[r][c+1] = 'f';
        }
        if ((r+1) < (nRows-1) && maze[r+1][c] == '.') //move south //CHANGED
        {
            coordQueue.push(Coord(r+1,c));
            //directions.push('s');
            maze[r+1][c] = 'f';
        }
        if ((c-1) > 0 && maze[r][c-1] == '.') //move west //CHANGED
        {
            coordQueue.push(Coord(r,c-1));
            //directions.push('w');
            maze[r][c-1] = 'f';
        }
        //CHANGED removed last else
    }
    //    There was no solution, so return false
    return false;
}