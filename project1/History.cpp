//
//  history.cpp
//  cs32p1
//
//  Created by Tanzeela Khan on 1/6/16.
//  Copyright © 2016 me. All rights reserved.


#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
using namespace std;
#include "History.h"


History::History(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_nRows = nRows;
    m_nCols = nCols;
    
    for (int r = 0; r < m_nRows; r++)
        for (int c = 0; c < m_nCols; c++)
            grid[r][c] = 0; //default
}


bool History::record(int r, int c)
{
    if (r < 1 || r> m_nRows || c <1 || c > m_nCols)
        return false;
    
    grid[r-1][c-1]++;
    return true;
    
}
void History::display() const
{
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
    
    
    clearScreen();
    for (int r = 0; r < m_nRows; r++) 
    {
        for (int c = 0; c < m_nCols; c++)
        {
            //cout << grid[r][c];
            
            char ch;
            if (grid[r][c] == 0)
            {
                ch = '.';
            }
            else if (grid[r][c] >= 26)
            {
                ch = 'Z';
            }
            else //grid[r][c] is through 1-25
            {
                switch(grid[r][c])
                            {
                                case 1:
                                    ch = 'A'; break;
                                case 2:
                                    ch = 'B'; break;
                                case 3:
                                    ch = 'C'; break;
                                case 4:
                                    ch = 'D'; break;
                                case 5:
                                    ch = 'E'; break;
                                case 6:
                                    ch = 'F'; break;
                                case 7:
                                    ch = 'G'; break;
                                case 8:
                                    ch = 'H'; break;
                                case 9:
                                    ch = 'I'; break;
                                case 10:
                                    ch = 'J'; break;
                                case 11:
                                    ch = 'K'; break;
                                case 12:
                                    ch = 'L'; break;
                                case 13:
                                    ch = 'M'; break;
                                case 14:
                                    ch = 'N'; break;
                                case 15:
                                    ch = 'O'; break;
                                case 16:
                                    ch = 'P'; break;
                                case 17:
                                    ch = 'Q'; break;
                                case 18:
                                    ch = 'R'; break;
                                case 19:
                                    ch = 'S'; break;
                                case 20:
                                    ch = 'T'; break;
                                case 21:
                                    ch = 'U'; break;
                                case 22:
                                    ch = 'V'; break;
                                case 23:
                                    ch = 'W'; break;
                                case 24:
                                    ch = 'X'; break;
                                case 25:
                                    ch = 'Y'; break;

                            }
            }
            
            //char ch = '.';
            
            
            //ch = grid[r][c] + '0';
            
            
//            
//            
//            switch(grid[r][c])
//            {
//                case 1:
//                    ch = 'A'; break;
//                case 2:
//                    ch = 'B'; break;
//                case 3:
//                    ch = 'C'; break;
//                case 4:
//                    ch = 'D'; break;
//                case 5:
//                    ch = 'E'; break;
//                case 6:
//                    ch = 'F'; break;
//                case 7:
//                    ch = 'G'; break;
//                case 8:
//                    ch = 'H'; break;
//                case 9:
//                    ch = 'I'; break;
//                case 10:
//                    ch = 'J'; break;
//                case 11:
//                    ch = 'K'; break;
//                case 12:
//                    ch = 'L'; break;
//                case 13:
//                    ch = 'M'; break;
//                case 14:
//                    ch = 'N'; break;
//                case 15:
//                    ch = 'O'; break;
//                case 16:
//                    ch = 'P'; break;
//                case 17:
//                    ch = 'Q'; break;
//                case 18:
//                    ch = 'R'; break;
//                case 19:
//                    ch = 'S'; break;
//                case 20:
//                    ch = 'T'; break;
//                case 21:
//                    ch = 'U'; break;
//                case 22:
//                    ch = 'V'; break;
//                case 23:
//                    ch = 'W'; break;
//                case 24:
//                    ch = 'X'; break;
//                case 25:
//                    ch = 'Y'; break;
//                default: //Y or Z
//                    ch = 'Z'; break;
//            }
            cout << ch;
            
        }
        cout << endl;
    }
    cout << endl;
}
