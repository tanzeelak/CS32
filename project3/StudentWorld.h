#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h" //not sure?
#include <list>
#include <vector>
#include <algorithm>
#include <queue>
#include "GameConstants.h"
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class FrackMan;
class Dirt;
class Protestor;
class Boulder;
class Coord;
//std::stack<Coord> todo;




class GameObject;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
    GraphObject::Direction dir() const { return m_dir; }
    void setDir(GraphObject::Direction dir) { m_dir = dir;}
private:
    int m_r;
    int m_c;
    GraphObject::Direction m_dir;
};

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    int init();
    int move();
    virtual void cleanUp();
    void setDisplayText();
    int actorsCount();
    void incActorsCount();
    void destroyDirt(int x, int y);
    bool isDirtBelow(int x, int y);
    bool isDirtinArea(int x, int y);
    bool isBoulder(int x, int y);
    std::vector<Actor*> accActors();
    void addActortoVector(Actor* daNewActor);
    int randInt(int max);
    int findRadius(int x1, int y1, int x2, int y2);
    bool isRadiusTooSmall(int x, int y);
    bool isNearFrackman(int x, int y, bool& through);
    bool isNearProtestor(int x, int y, Protestor*& daProtestor);
    bool isObstacle(int x, int y);
    FrackMan* accPlayer() const;
    bool isActorThere(int x, int y);
    bool canWalkStraight(int sx, int sy, int ex, int ey, int& whichDirection);
    GraphObject::Direction chooseRandDir();
    bool isProtestorFacingFrackMan(Protestor* p);
    bool isValidDir(Protestor* p, GraphObject::Direction dir);
    bool canMovePerpen(Protestor* p, GraphObject::Direction& dir);
    int getProtestorCount();
    int getBarrelsCount();
    void incBarrelsCount(int num);
    void incProtestorCount(int num);
    void illuminate(int x, int y);
    //void createFakeMaze(Coord fake[VIEW_WIDTH][VIEW_HEIGHT]);
    
    std::queue<Coord> getStack(){return toDo;}
    //int pathExists(Protestor* p, int sx, int sy, int ex, int ey);
    
//    void explore(int maze[64][64], std::queue<Coord>& toDo, int r, int c,  int dir)
//    {
//        if (maze[r][c] == 0)
//        {
//            //toDo.enqueue(Coord(r,c));
//            
//            
//            switch(dir)
//            {
//                case 0:
//                    maze[r][c] = 0;
//                    break;
//                case 1:
//                    maze [r][c] = 1;
//                    break;
//                case 2:
//                    maze [r][c] = 2;
//                    break;
//                case 3:
//                    maze [r][c] = 3;
//                    break;
//                case 4:
//                    maze [r][c] = 4;
//                    break;
//            }
//            // anything non-OPEN will do
//        }
//    }
//    
//    
//    int pathExists(int nRows, int nCols, int sr, int sc, int er, int ec);
    virtual ~StudentWorld();

private:
    std::queue<Coord> toDo;
    int m_nActors;
    std::vector<Actor*> actors;
    FrackMan* player;
    Dirt* field[64][64];
    int m_internalTicks;
    int m_nProtestors;
    int m_nBarrels;
    Coord* fake[64][64];
    
};

#endif // STUDENTWORLD_H_
