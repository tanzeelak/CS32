//
//  mazestack.cpp
//  hw2
//
//  Created by Tanzeela Khan on 1/26/16.
//  Copyright © 2016 me. All rights reserved.
//
//NUMBER 1
#include <string>
#include <stack>
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

stack<Coord> coordStack;
stack<char> directions;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    
    if (!(maze[sr][sc] == '.' && maze[er][ec] == '.'))
        return  false;
    coordStack.push(Coord(sr,sc));
    
    maze[sr][sc] = 'f';
//    Push the starting coordinate (sr,sc) onto the coordinate stack and
//    update maze[sr][sc] to indicate that the algorithm has encountered
//    it (i.e., set maze[sr][sc] to have a value other than '.').
    
    int r = sr, c = sc;
    //int howManyPopped = 0;
    while (!coordStack.empty()) //    While the stack is not empty,
    {
        //    {   Pop the top coordinate off the stack. This gives you the current
        //        (r,c) location that your algorithm is exploring.
        Coord curr = coordStack.top();
        r = curr.r(), c = curr.c();
        coordStack.pop();

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
            coordStack.push(Coord(r-1,c));
            directions.push('n');
            maze[r-1][c] = 'f';
            
        }
        else if ((c+1) < (nCols-1) && maze[r][c+1] == '.') //move east
        {
            coordStack.push(Coord(r,c+1));
            directions.push('e');
            maze[r][c+1] = 'f';
        }
        else if ((r+1) < (nRows-1) && maze[r+1][c] == '.') //move south
        {
            coordStack.push(Coord(r+1,c));
            directions.push('s');
            maze[r+1][c] = 'f';
        }
        else if ((c-1) > 0 && maze[r][c-1] == '.') //move west
        {
            coordStack.push(Coord(r,c-1));
            directions.push('w');
            maze[r][c-1] = 'f';
        }
        else //nowhere to go, so must backtrack
        {
            if (directions.empty()) //if there are no directions to backtrack, then no solution
                return  false;
            char store = directions.top(); //look at first direction
            directions.pop();
            
            switch (store)
            {
                case 'n': //north
                    r++; //move in opposite direction!
                    coordStack.push(Coord(r,c));
                    break;
                case 'e': //east
                    c--;
                    coordStack.push(Coord(r,c));
                    break;
                case 's': //south
                    r--;
                    coordStack.push(Coord(r,c));
                    break;
                case 'w': //west
                    c++;
                    coordStack.push(Coord(r,c));
                    break;
                default:
                    break;
            }
        }
    }
//    There was no solution, so return false
    return false;
}