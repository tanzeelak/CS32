#include "StudentWorld.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <stack>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
    
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    player = nullptr;
    m_nActors = 0;
    m_internalTicks = 0;
    m_nProtestors = 0;
    m_nBarrels = 0;
}

int StudentWorld::init()
{
    player = new FrackMan(this);
    
    
    int w, h;
    for (w = 0; w < 64; w++)
    {
        for (h = 0; h < (64); h++)
        {
            
            field[w][h] = nullptr;
        }
    }
    
    //add dirt field!
    for (w = 0; w < 64; w++)
    {
        for (h = 0; h < (60); h++)
        {
            field[w][h] = new Dirt(this, w,h);//new dirt object

        }
    }
    
    for (w = 30; w < 34; w++)
    {
        for (h = 4; h < (60); h++)
        {
            delete field[w][h];
            field[w][h] = nullptr;
        }
    }
    const int z = getLevel();
    
    //add boulders!
    int b = min(z/2+2, 6);
    int i;
    for (i = 0; i < b; i++)
    {
        int tempx = randInt(60); int tempy = randInt(56);
        
        if (!isRadiusTooSmall(tempx, tempy) && !((tempx>=26 && tempx<34) || (tempy==0)) )
            actors.push_back(new Boulder(this, tempx, tempy));
        else
            i--;
    }
    
    
    //add gold!
    int g = max(5-z / 2, 2);
    for (i = 0; i < g; i++)
    {
        int tempx = randInt(60); int tempy = randInt(56);
        
        if (!isRadiusTooSmall(tempx, tempy) && !((tempx>=26 && tempx<34) || (tempy==0)) )
            actors.push_back(new Gold(this, tempx, tempy, true, 1, 20));
        else
            i--;
    }
    
    //add oil!!
    int l = min(2 + z, 20);
    for (i = 0; i < l; i++)
    {
        int tempx = randInt(60); int tempy = randInt(56);
        
        if (!isRadiusTooSmall(tempx, tempy) && !((tempx>=26 && tempx<34) || (tempy==0)) )
            actors.push_back(new Barrel(this, tempx, tempy));
        else
            i--;
    }
    
    
    //actors.push_back(new Gold(this, 10, 10, false, 1, 20));
    //actors.push_back(new RegularProtestor(this, 20, 20, 10));
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setDisplayText();
    
//    setGameStatText( "Scr: " + std::to_string(getScore()) + " Lvl: " + std::to_string(getLevel()) + " Lives: " + std::to_string(getLives()));
    // The term “Actors” refers to all Protesters, the player, Goodies,
    // Boulders, Barrels of oil, Holes, Squirts, the Exit, etc.
    // Give each Actor a chance to do something
    
    
    std::vector<Actor*>::iterator it = actors.begin();
    player->doSomething();
    //std::vector<Actor*>::iterator it = actors.begin();
    for (it = actors.begin(); it != actors.end(); it++)
    {
        if (Barrel* b = dynamic_cast<Barrel*>(*it))
        {
            if (b->pickedUp())
            {
                //decrease counter for gold
            }
            //if counter == 0;
            //end level
        }
        
        
        (*it)->doSomething();
    }
    
    ///ADDING NEW ACTORS DURING TICKS

    const int z = getLevel();
    
    if (m_internalTicks == 0)
    {
        actors.push_back(new RegularProtestor(this, 10));
    }
    
    
    int t = max(25, 200 - z);
    const int store = (2 + z* 1.5);
    int p = min(15, store);
    
//    if (m_internalTicks%t == 0)// && m_nProtestors < p) PROBLEM! SHOULD ADD TWO PROTEST!
//    {
//        actors.push_back(new RegularProtestor(this, 60, 60, 10));
//    }
//    
    
    int g = z * 25 + 300;
    if (randInt(g) == 1)
    {
        switch (randInt(5))
        {
            case 1:
                actors.push_back(new Sonar(this));
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                int tempx, tempy;
                do
                {
                    tempx = randInt(60);
                    tempy = randInt(56);
                }while (isDirtinArea(tempx, tempy));
                actors.push_back(new WaterPool(this, tempx, tempy));
        }

    }
    
    //remove dead actors

    
    
    
    it = actors.begin();
    for (; it != actors.end();)
    {
        if (!(*it)->isAlive())
        {
            delete *it;
            it = actors.erase(it);
        }
        else
            it++;
    }
    
    
    
    if (getBarrelsCount() <= 0)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    
    
    if (!player->isAlive())
    {
        decLives();
        //if (getLives() == 0)
            
    
        return GWSTATUS_PLAYER_DIED;
    }
    
    
    m_internalTicks++;
    
    return GWSTATUS_CONTINUE_GAME;
    
 //   decLives();
//    return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
    int w, h;
    for (w = 0; w < 64; w++)
    {
        for (h = 0; h < (60); h++)
        {
            
            delete field[w][h];
        }
    }
    
    delete player;
    m_nActors = 0;
    
    std::vector<Actor*>::iterator it = actors.begin();
    
    for (; it != actors.end();)
    {
            delete *it;
            it = actors.erase(it);
    }
    
    player = nullptr;
    m_nActors = 0;
    m_internalTicks = 0;
    m_nProtestors = 0;
    m_nBarrels = 0;
    
    
    
}


void StudentWorld::setDisplayText()
{
    
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = accPlayer()->getHitPoints();
    int squirts = accPlayer()->getSquirtCt();
    int gold = accPlayer()->getGoldCt();
    int sonar = accPlayer()->getSonarCt();
    int barrelsLeft = getBarrelsCount();
    string s = "Scr: "+std::to_string(score)+" Lvl: " + std::to_string(level)+ " Lives: "+std::to_string(lives)+" Hlth: "+std::to_string(health)+" Wtr: "+std::to_string(squirts)+" Gld: "+std::to_string(gold)+" Sonar: "+std::to_string(sonar)+" Oil Left: "+std::to_string(barrelsLeft);
    setGameStatText(s);
    
    
}





int StudentWorld:: actorsCount()
{
    return m_nActors;
}

void StudentWorld::incActorsCount()
{
    m_nActors++;
}

void StudentWorld::destroyDirt(int x, int y)
{
    for(int tempx = 0; tempx < 4; tempx++)
    {
        for (int tempy = 0; tempy < 4; tempy++)
        {
            if (field[tempx +x][tempy +y] != nullptr)
            {
                delete field[tempx +x][tempy +y];
                playSound(SOUND_DIG);
                field[tempx +x][tempy +y] = nullptr;
            }
        }
    }
}

bool StudentWorld::isDirtBelow(int x, int y)
{
    if (field[x][y-1])
        return true;
    if (field[x+1][y-1])
        return true;
    if (field[x+2][y-1])
        return true;
    if (field[x+3][y-1])
        return true;
    return false;
}

bool StudentWorld::isDirtinArea(int x, int y)
{
    int tempx = x+4, tempy = y+4;
    
    
    for (int i = x; i < tempx; i++)
    {
        for (int j = y; j < tempy; j++)
        {
            if (field[i][j] != NULL)
                return true;
        }
    }
    return false;
}



bool StudentWorld::isBoulder(int x, int y)
{    
    std::vector<Actor*>::iterator it = actors.begin();
    
    for (; it != actors.end(); it++)
    {
        if (Boulder* b = dynamic_cast<Boulder*>(*it)) // if valid ptr, will succeed
        {
            if (b->getX() == x && b->getY() == y)
                return true;
            
            int tempX = b->getX();
            int tempY = b->getY();


            if (findRadius(tempX, tempY, x, y) <4)
            {
                return true;
            }
            
        }
        
    }
    
    return false;
}

//bool StudentWorld::isDirt(int x, int y)
//{
//    
//    
//    if (field[x][y] != nullptr)
//        return true;
//    return false;
//}

std::vector<Actor*> StudentWorld::accActors()
{
    return actors;
}

void StudentWorld::addActortoVector(Actor* daNewActor)
{
    actors.push_back(daNewActor);
}



int StudentWorld::randInt(int max)
{
    return rand()%(max) +1;
}


int StudentWorld::findRadius(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x1-x2), 2)+pow((y1-y2),2));
}




bool StudentWorld::isRadiusTooSmall(int x, int y)
{
    int tempx = x-6, tempy = y-6;
    
    std::vector<Actor*>::iterator it = actors.begin();
    
    for(; it != actors.end(); it++)
    {
        tempx = (*it)->getX();
        tempy = (*it)->getY();
        if (sqrt(pow(tempx-x, 2)+pow(tempy-y,2)) < 6)
        {
            return true;
        }
    }

    return false;
}

bool StudentWorld::isNearFrackman(int x, int y, bool& through)
{
    int tempx = player->getX();
    int tempy = player->getY();
    if (sqrt(pow(tempx-x, 2)+pow(tempy-y,2)) <= 3) //PROBLEM!! WILL NEVER ENTER HERE
    {
        through = true;
        return true;
    }
    
    else if (sqrt(pow(tempx-x, 2)+pow(tempy-y,2)) <= 4 )
    {
        through = false;
        return true;
    }
    through = false;
    return false;
    
}

bool StudentWorld::isObstacle(int x, int y)
{
    if (isBoulder(x, y) || isDirtinArea(x, y)|| (x == VIEW_WIDTH) || (x == 1) || (y == VIEW_HEIGHT) || (y == 1))
        return true;
    return false;
    
}


bool StudentWorld::isNearProtestor(int x, int y, Protestor*& daProtestor)
{
    int tempx = x-3, tempy = y-3;
    
    std::vector<Actor*>::iterator it = actors.begin();
    
    for (; it != actors.end(); it++)
    {
        if (Protestor* p = dynamic_cast<Protestor*>(*it)) //will only if protestor!
        {
            tempx = (*p).getX();
            tempy = (*p).getY();
            if (sqrt(pow(tempx-x, 2)+pow(tempy-y,2)) <= 3)
            {
                daProtestor = p;
                return true;
            }
        }
    }
    daProtestor = nullptr;
    return false;
}

FrackMan* StudentWorld::accPlayer() const
{
    
    //if (player != nullptr)
    return this->player;
    //return nullptr;
}


bool StudentWorld::isActorThere(int x, int y)
{
    std::vector<Actor*>::iterator it = actors.begin();
    for (; it != actors.end(); it++)
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
            return true;
    }
    return false;
}

bool StudentWorld::canWalkStraight(int sx, int sy, int ex, int ey, int& whichDirection)
{
    //cout << "can walk straight" << endl;
    if (sx == ex) //if same x column, then you gotta see if you can walk up/down
    {
        
        if (sy < ey) //going up!
        {
            whichDirection = 1;
            while (sy != ey)
            {
                if (!isObstacle(sx, ++sy))
                    return true;
                else
                    return false;
            }
        }
        else if (sy > ey) //going down!
        {
            whichDirection = 2;
            while (sy != ey)
            {
                if (!isObstacle(sx, --sy))
                    return true;
                else
                    return false;
            }
        }
    }
    
    else if (sy == ey) //if same y column, then you gotta see if you can wakl left/right
    {
        
        if (sx < ex) // going right!
        {
            whichDirection = 3;
            while (sx != ex)
            {
                if (!isObstacle(++sx, sy))
                    return true;
                else
                    return false;
            }
        }
        else if (sx > ex) //going left!
        {
            whichDirection = 4;
            while (sx != ex)
            {
                if (!isObstacle(--sx, sy))
                    return true;
                else
                    return false;
            }
        }
    }
    
    return false;
}

GraphObject::Direction StudentWorld::chooseRandDir()
{
    switch(rand()%4 +1)
    {
        case 1: return ActivatingObject::up;
            break;
        case 2: return ActivatingObject::down;
            break;
        case 3: return ActivatingObject::right;
            break;
        case 4: return ActivatingObject::left;
            break;
    }
    return ActivatingObject::left;
}

bool StudentWorld::isProtestorFacingFrackMan(Protestor* p)
{
    
    int protesX = p->getX();
    int protesY = p->getY();
    int frackX = accPlayer()->getX();
    int frackY = accPlayer()->getY();
    
    
    if (p->getDirection() == ActivatingObject::up)
    {
        if (protesY < frackY)
            return true;
    }
    else if (p->getDirection() == ActivatingObject::down)
    {
        if (protesY > frackY)
            return true;
    }
    else if (p->getDirection() == ActivatingObject::right)
    {
        if (protesX < frackX)
            return true;
    }
    else if (p->getDirection() == ActivatingObject::left)
    {
        if (protesX > frackX)
            return true;
    }
    return false;
    
}


bool StudentWorld::isValidDir(Protestor* p, GraphObject::Direction dir)
{
    int protesX = p->getX();
    int protesY = p->getY();
    switch (dir)
    {
        case ActivatingObject::up:
            if (isObstacle(protesX, protesY+1))
            { //cout << "up" << endl;
                return false;
            }
            break;
        case ActivatingObject::down:
            if (isObstacle(protesX, protesY-1))
            {
                //cout <<  "down" << endl;;
                return false;
            }
                break;
        case ActivatingObject::right:
            if (isObstacle(protesX+1, protesY))
            {
                //cout << "right" << endl;
                return false;
            }
            break;
        case ActivatingObject::left:
            if (isObstacle(protesX-1, protesY))
            {
                //cout << "left" << endl;
                return false;
            }
            break;
    }
    return true;
}

bool StudentWorld::canMovePerpen(Protestor* p, GraphObject::Direction& dir)
{
    int protesX = p->getX();
    int protesY = p->getY();
    int protesDir = p->getDirection();
    
    
    switch (protesDir)
    {
        case ActivatingObject::up:
        case ActivatingObject::down:
            if (!isObstacle(protesX-1, protesY))
            {
                dir = ActivatingObject::left;
                return true;
            }
            else if (!isObstacle(protesX+1, protesY))
            {
                dir = ActivatingObject::right;
                return true;
            }
            break;
        case ActivatingObject::right:
        case ActivatingObject::left:
            if (!isObstacle(protesX, protesY-1))
            {
                dir = ActivatingObject::down;
                return true;
            }
            else if (!isObstacle(protesX, protesY+1))
            {
                dir = ActivatingObject::up;
                return true;
            }
            break;
    }
    return false;
    
}

int StudentWorld::getBarrelsCount()
{
    return m_nBarrels;
}
void StudentWorld::incBarrelsCount(int num)
{
    m_nBarrels += num;
}



int StudentWorld::getProtestorCount()
{
    return m_nProtestors;
}
void StudentWorld::incProtestorCount(int num)
{
    m_nProtestors += num;
}


void StudentWorld::illuminate(int x, int y)
{
    std::vector<Actor*>::iterator it = actors.begin();
    for (; it != actors.end(); it++)
    {
        int curX = (*it)->getX();
        int curY = (*it)->getY();
        if (findRadius(curX, curY, x, y)  <12)
        {
            (*it)->setVisible(true);
        }
        
    }
}

//REMOVE LATER
//void StudentWorld::createFakeMaze(Coord fake[VIEW_WIDTH][VIEW_HEIGHT])
//{
//    for (int i = 0; i < VIEW_WIDTH; i++)
//    {
//        for (int j = 0; j < VIEW_HEIGHT; j++)
//        {
//            if (!isObstacle(i, j))
//            {
//                fake[i][j] = *new Coord(i, j);
//                fake[i][j]->setDir(null);
//            }
//            else
//            {
//                 fake[i][j] = ;
//            }
//            
//        }
//    }
//}
//
//
//
//#include <queue>
//
//
//const char WALL = 'X';
//const char OPEN = '.';
//const char SEEN = 'o';
//enum State {U, D, R, L, E};
//
//int StudentWorld::pathExists(int nRows, int nCols, int sr, int sc, int er, int ec)
//{
//    queue<Coord> toDo;
//    int w,h;
//    for (w = 0; w < 64; w++)
//    {
//        for (h = 0; h < (60); h++)
//        {
//            fake[w][h] = new Coord(w, h);//new coord objects
//            
//        }
//    }
//    
//    for (int i = 0; i < VIEW_WIDTH; i++)
//    {
//        for (int j = 0; j < VIEW_HEIGHT; j++)
//        {
//            if (!isObstacle(i, j))
//            {
//            //    fake[i][j]->setDir(none); //no obstacle
//            }
//            else
//            {
//              //  fake[i][j]->setDir(none); //obstacle!
//            }
//        }
//    }
//
//    
//    if (sr < 0  ||  sr > nRows  ||  sc < 0  || sc > nCols  ||
//        er < 0  ||  er > nRows  ||  ec < 0  || ec > nCols  ||
//        fake[sr][sc]->dir() != 0 ||  fake[er][ec]->dir() != 0)
//        return false;
//    
//    fake[sr][sc] = 0;
//    
//    
//   
//    
//    
//    while ( ! toDo.empty() )
//    {
//        Coord curr = toDo.front();
//        toDo.pop();
//        
//        const int cr = curr.r();
//        const int cc = curr.c();
//        
//        if (cr == er  &&  cc == ec)
//            return true;
//        
//        if (fake[cr][cc+1] == 0)
//        {
//            toDo.push(Coord(cr,cc+1));
//            fake[cr][cc+1]->setDir(GraphObject::up); //up
//            // anything non-OPEN will do
//        }
//        if (fake[cr][cc-1] == 0)
//        {
//            toDo.push(Coord(cr,cc-1));
//            fake[cr][cc-1]->setDir(GraphObject::down); //down
//            // anything non-OPEN will do
//        }
//        if (fake[cr+1][cc] == 0)
//        {
//            toDo.push(Coord(cr+1,cc)); //right
//            fake[cr+1][cc]->setDir(GraphObject::right);
//            // anything non-OPEN will do
//        }
//        if (fake[cr-1][cc] == 0)
//        {
//            toDo.push(Coord(cr-1,cc)); //left
//            fake[cr-1][cc]->setDir(GraphObject::left);
//            // anything non-OPEN will do
//        }
//        
//    }
//    return false;
//}
//
//int StudentWorld::pathExists(Protestor* p, int sx, int sy, int ex, int ey)
//{
//    createFakeMaze(fake);
//    //if found end, return 1 and add to stack
//    //if hit obstruction, return 0
//    
//    if (fake[sx][sy] != ".")
//        return 0;
//
//    
//    if (sx == ex  &&  sy == ey)
//    {
//        fake[sx][sy] = toDo.push(Coord(sx,ey));
//        return 1;
//    }
//
//    
//    //field[sr][sc] = ;  // anything non-'.' will do
//    
//    if (pathExists(p, sx-1, sy, ex, ey))
//        return 1;
//    if (pathExists(p, sx+1, sy, ex, ey))
//        return 1;
//    if (pathExists(p, sx+1, sy, ex, ey))
//        return 1;
//    if (pathExists(p, sx, sy+1, ex, ey))
//        return 1;
//    
//    return 0;
//}
//


StudentWorld::~StudentWorld()
{
    int w,h;
    for (w = 0; w < 64; w++)
    {
        for (h = 0; h < (60); h++)
        {
            
            delete field[w][h];
        }
    }
    
    delete player;
    m_nActors = 0;
}