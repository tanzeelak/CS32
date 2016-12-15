#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
//#include "GameController.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include <algorithm>
#include <list>
#include <stack>
#include <iostream>

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Squirt;
class Sonar;
class Gold;

class Actor: public GraphObject
{
public:
    //    i. It must have a simple constructor and destructor.
    Actor(int ID, StudentWorld* a, int x, int y, Direction dir, double size, int depth);
    virtual void doSomething();
    virtual ~Actor();
    bool isAlive();
    void kill();
    StudentWorld* getWorld() const;
    virtual void b() const = 0;
    int getTicks();
    void setTicks(int num);
    
private:
    StudentWorld* a_studentWorld;
    bool m_alive = true;
    int m_ticksCount;
};

//////////////////////-----------------AGENT-----------------/////////////////////
class Agent: public Actor
{
public:
    Agent(StudentWorld* a, int x, int y, int ID, Direction dir, int hitPts);
    virtual void b() const = 0;
    void  incHitPoints (int num);
    int getHitPoints();
    virtual ~Agent(){}
private:
    int m_hitPts = 0;

};


class FrackMan: public Agent
{
public:
    FrackMan(StudentWorld* a);
    virtual void doSomething();
    virtual void b() const {}
    //void incPoints (int num);
    void incGold(int num);
    int getGoldCt();
    void incSonar(int num);
    int getSonarCt();
    void incSquirt(int num);
    int getSquirtCt();
    virtual ~FrackMan();
private:
    
    Squirt* m_squirt;
    Sonar* m_sonarActive;
    Gold* m_goldActive;
    //int m_hitPts;
    int m_squirts;
    int m_sonarChg;
    int m_gold;
    
};

class Protestor: public Agent
{
public:
    Protestor(StudentWorld* a, int numSquares, int ID);
    enum State {leave_the_oil_field, rest, moving, isAnnoyed};
    virtual void doSomething();
    void setState(State s);
    void shout(bool shout);
    bool getShout();
    void makeRightTurn(bool rightTurn);
    bool getRightTurn();
    

    State getState(){return curState;};
    void getsGold();
    bool hasGold();
    int getNumSquares();
    void incNumSquares(int num);    
    
    int getInternalMovingTicks();
    void setInternalMovingTicks(int num);
    
    int getInternalRightTicks();
    void setInternalRightTicks(int num);
    virtual void b() const = 0;
    virtual ~Protestor(){}
private:
    bool m_receivedGold;
    bool m_shouted;
    bool m_madeRightTurn;
    int m_internalNonRestTicks;
    
    int m_internalRightTicks;
    
    State curState;
    int numSquaresToMoveInCurrentDirection;
    int m_squaresMoved;
    
};

class RegularProtestor: public Protestor
{
public:
    RegularProtestor(StudentWorld* a, int numSquares);
    virtual void doSomething();
    virtual void b() const {};
    virtual ~RegularProtestor(){}
};

class HardcoreProtestor: public Protestor
{
public:
    HardcoreProtestor(StudentWorld* a, int numSquares);
    virtual void doSomething();
    virtual void b() const {};
    virtual ~HardcoreProtestor(){}
};



///////////////////------------------REGULAR OBJECTS!!!------------/////////////////

class Dirt: public Actor
{
public:
    Dirt(StudentWorld* a, int x, int y);
    virtual void doSomething();
    virtual void b() const {}
    virtual ~Dirt(){}
};

class Boulder: public Actor
{
public:
    Boulder(StudentWorld* a, int x, int y);
    virtual void doSomething();
    virtual void b() const {};
    enum State {stable, waiting, falling};
    virtual ~Boulder(){}
private:
    //string state = "stable";//bool m_stable = true;
    State curState;
};


class Squirt: public Actor
{
public:
    Squirt(StudentWorld* a, int x, int y, Direction dir);
    virtual void doSomething();
    virtual void b() const {};
    virtual ~Squirt(){}
private:
    int travelDist = 4;
};


//////////////-----------------ACTIVATINGOBJECT-----------------///////////////
class ActivatingObject: public Actor
{
public:
    ActivatingObject(StudentWorld* a, int x, int y, int ID);
    virtual void b() const = 0;
    virtual ~ActivatingObject(){}
};



class Barrel: public ActivatingObject
{
public:
    Barrel(StudentWorld* a, int x, int y);
    virtual void doSomething();
    virtual void b() const {};
    bool pickedUp();
    virtual ~Barrel(){}
private:
    bool plucked;
};

class Gold: public ActivatingObject
{
public:
    enum Picker {frackman, protestor};
    enum State {permanent, temporary};
    Gold(StudentWorld* a, int x, int y, bool isVis, int frackOrProt, int tempTicks); //Nuggets
    //    buried within the Dirt of the oil field always start out invisible, whereas Nuggets
    //    dropped by the FrackMan start out visible
    virtual void doSomething();
    virtual void b() const {};
    virtual ~Gold(){}
private:
    Picker truePicker;
    //bool m_frackOrProt;
    State curState;
};

class Sonar: public ActivatingObject
{
public:
    Sonar(StudentWorld* a);
    virtual void doSomething();
    virtual void b() const {};
    virtual ~Sonar(){}
};

class WaterPool: public ActivatingObject
{
public:
    WaterPool(StudentWorld* a, int x, int y);
    virtual void doSomething();
    virtual void b() const {};
    virtual ~WaterPool(){}
};



#endif // ACTOR_H_
