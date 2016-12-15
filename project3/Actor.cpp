#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
//#include "GameWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/////////////////////ACTOR//////////////////////////////////

Actor::Actor(int ID, StudentWorld* a, int x, int y, Direction dir, double size, int depth):GraphObject(ID, x, y, dir, size, depth)
{
    a_studentWorld = a;
    setVisible(true);
    m_alive = true;
    getWorld()->incActorsCount();
    m_ticksCount = 0;
}


//    ii. It must be derived from our GraphObject class.

//    iii. It (or its base class) must make itself visible via a call to
//    setVisible(true);

//    iv. It must have a single virtual method called doSomething() that can
//    be called by the World to get one of the game’s actors to do
//        something.

//        v. You may add other public/private methods and private member
//        variables to this base class, as you see fit.


void Actor::doSomething()
{
    
}


Actor::~Actor()
{
    setVisible(false);
}

bool Actor::isAlive()
{
    return m_alive;
}

void Actor::kill()
{
    m_alive = false;
    setVisible(false);
    m_ticksCount = 0;
}


StudentWorld* Actor::getWorld()const
{
    return this->a_studentWorld;
}

int Actor::getTicks()
{
    return m_ticksCount;
}

void Actor::setTicks(int num)
{
    if (num == 0)
        m_ticksCount = 0;
    else
        m_ticksCount = m_ticksCount+num;
}
//////////////////////--------------------AGENT-------------------///////////////////
Agent::Agent(StudentWorld* a, int x, int y, int ID, Direction dir, int hitPts):Actor(ID, a, x, y, dir, 1.0, 0)
{
    m_hitPts = hitPts;
    
}

void Agent::incHitPoints(int num)
{
    m_hitPts+= num;
    //getWorld()->playSound(SOUND_PROTESTER_YELL);
}

int Agent::getHitPoints()
{
    return m_hitPts;
}


///////////////////////FRACKMAN///////////////////////////
FrackMan::FrackMan(StudentWorld* a):Agent(a, 30, 60, IID_PLAYER, right, 10)
{
    m_squirts = 5;
    m_sonarChg = 1;
    m_gold = 0;
}


void FrackMan::doSomething()
{
    //1
    if (!isAlive())
        return;
    if (getHitPoints() <=0)
    {
        kill();
        return;
    }
    
    int x = getX();
    int y = getY();
    int k;
    StudentWorld* ptr = getWorld();
    ptr->getKey(k);
    
    //2
    if (getWorld()->isDirtinArea(x, y))
    {
        getWorld()->destroyDirt(x, y);
    }
    
    //3
    if (k == KEY_PRESS_ESCAPE)
    {
        kill();
    }
    if (k == KEY_PRESS_UP && y < 60)
    {
        if (getDirection() == up && !getWorld()->isBoulder(x, y+1)) //radius function!
        {
            moveTo(x, y+1);
        }
        setDirection(up);
    }
    if (k == KEY_PRESS_DOWN && y > 0)
    {
        if (getDirection() == down && !getWorld()->isBoulder(x, y-1))
            moveTo(x, y-1);
        setDirection(down);
    }
    
    if (k == KEY_PRESS_RIGHT && x < 60)
    {
        if (getDirection() == right && !getWorld()->isBoulder(x+1, y))
            moveTo(x+1, y);
        setDirection(right);
    }
    if (k ==KEY_PRESS_LEFT && x > 0)
    {
        if (getDirection() == left && !getWorld()->isBoulder(x-1, y))
            moveTo(x-1, y);
        setDirection(left);
    }
    if (k == KEY_PRESS_SPACE)
    {
        if (m_squirts > 0){
            
            m_squirt = new Squirt(getWorld(),getX(),getY(), getDirection());
            getWorld()->addActortoVector(m_squirt);
            //m_squirt->doSomething();

            
//        m_squirt = new Squirt(getWorld(),getX(),getY(), getDirection());
//        getWorld()->addActortoVector(m_squirt);
//        m_squirt->doSomething();
            m_squirts--;
        }
    }
    if (k == 'z' || k == 'Z')
    {
        if (m_sonarChg > 0)
        {
            //activate sonar!
            getWorld()->illuminate(getX(), getY());
            m_sonarChg--;
        }
    }
    if (k == KEY_PRESS_TAB)
    {
        if (m_gold > 0)
        {
            //cout << "pls1" <<endl;
            getWorld()->addActortoVector(new Gold(getWorld(), getX() , getY(), true, 2, 100));// new Gold(getWorld(), getX() , getY(), true, 2, 100);
            
            m_gold--;
        }
    }
    
    
    
    getWorld()->destroyDirt(x, y);
    
    //getWorld()->accActors().push_back(new Squirt(this));
    
    
}

void FrackMan::incGold(int num)
{
    m_gold = m_gold+ num;
}

int FrackMan::getGoldCt()
{
    return m_gold;
}

void FrackMan::incSonar(int num)
{
    m_sonarChg+= num;
}

int FrackMan::getSonarCt()
{
    return m_sonarChg;
}

void FrackMan::incSquirt(int num)
{
    m_squirts += num;
}

int FrackMan::getSquirtCt()
{
    return m_squirts;
}

FrackMan::~FrackMan()
{
}


/////////------PROTESTOR----////////
Protestor::Protestor(StudentWorld* a, int numSquares, int ID):Agent(a, 60, 60, ID, left, 5)
{
    
    m_receivedGold = false;
    numSquaresToMoveInCurrentDirection = numSquares; //must be between 8 and 60. how?
    m_squaresMoved = 0;
    m_shouted = false;
    m_madeRightTurn = false;
    m_internalNonRestTicks = 0;
    m_internalRightTicks = 0;
    getWorld()->incProtestorCount(1);
    //PROBLEM: INITALIZE STATE HERE
    
    const int b = 3-(getWorld()->getLevel())/4;
    int ticksToWaitBetweenMoves = max(0, b);
    setTicks(ticksToWaitBetweenMoves); //resting state
    curState = rest;
}

void Protestor::doSomething() 
{
    
//    bool through = false;
//    int xOrYDir;
//    if (!isAlive()) //1
//        return;
//    if (getHitPoints() <= 0 || hasGold())
//    {
//        setState(leave_the_oil_field);//kill();
//        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
//        setTicks(0);
//        return;
//    }
//    
//    ///IMPLEMENT LEAVE OIL FIELD
//    
//    if (getState() == leave_the_oil_field)
//    {
//        //getWorld()->pathExists(this, getX(), getY(), getWorld()->accPlayer()->getX(), getWorld()->accPlayer()->getY());
//        //getWorld()->getStack().fro
//    }
//    
//    if (getTicks() > 0) //waiting state! //2
//    {
//        setState(rest);
//        setTicks(-1);
//        return;
//    }
//    else //getTicks() == 0 //3
//    {
//        setTicks(4);
//    }
//    if (getState() == isAnnoyed)
//    {
//        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
//        const int z = getWorld()->getLevel();
//        
//        setTicks(max(50,100-z*10));
//    }
//    
//    
//    
//    if (getInternalMovingTicks() == 15) //shouting mechanism
//    {
//        shout(false);
//        setInternalMovingTicks(0);
//    }
//    if (getShout())
//    {
//        setInternalMovingTicks(1);
//    }
//    
//    if (getInternalRightTicks() == 200) //perpen mechanism
//    {
//        makeRightTurn(false);
//        setInternalRightTicks(0);
//    }
//    if (getRightTurn())
//    {
//        setInternalRightTicks(1);
//    }
//    
//    
//    int someNumber;
//    bool didMove = false;
//    
//    
//    bool isNearFrackMan = getWorld()->isNearFrackman(getX(), getY(), through);
//    int playersX = getWorld()->accPlayer()->getX();
//    int playersY = getWorld()->accPlayer()->getY();
//    Direction use;
//    
//    
//    //4
//    if (isNearFrackMan && getWorld()->isProtestorFacingFrackMan(this) && !getShout()) //shouting mechanism //4
//    {
//        shout(true);
//        getWorld()->accPlayer()->incHitPoints(-2);
//        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
//        //setState(isAnnoyed);
//        setInternalMovingTicks(1);
//        return;
//    }
//    
//    
//    if (getNumSquares() <= 0 || getWorld()->isObstacle(getX(), getY())) //if no more squares, change direction! //6
//    {
//        incNumSquares(0); //reset squares
//        incNumSquares(rand()%51 +8);
//        do //PROBLEM: UNCOMMENT LATER
//        {
//            setDirection(getWorld()->chooseRandDir());
//        }while(!getWorld()->isValidDir(this, getDirection()));
//    }
//    
//    //cout << "ehelo " << getNumSquares() << endl;
//    
//    
//    
//    if (getNumSquares() > 0 && !getWorld()->isObstacle(getX(), getY())) //still squares!
//    {
//        
//        ///5 //will only enter if frack is in sight
//        if (getWorld()->canWalkStraight(getX(), getY(), playersX, playersY, someNumber) && !isNearFrackMan) //walk up
//        {
//            //cout << "can see frackman" << endl;
//            
//            switch(someNumber)
//            {
//                case 1:
//                    setDirection(up);
//                    moveTo(getX(), getY()+1);
//                    didMove = true;
//                    break;
//                case 2:
//                    setDirection(down);
//                    moveTo(getX(), getY()-1);
//                    didMove = true;
//                    break;
//                case 3:
//                    setDirection(right);
//                    moveTo(getX()+1, getY());
//                    didMove = true;
//                    break;
//                case 4:
//                    setDirection(left);
//                    moveTo(getX()-1, getY());
//                    didMove = true;
//                    break;
//            }
//            incNumSquares(0); //reset
//            return;
//        }
//        
//        //7
//        else if (getWorld()->canMovePerpen(this, use) && !getRightTurn())
//        {
//            makeRightTurn(true);
//            setDirection(use);
//            incNumSquares(0);
//            
//        }
//        
//        ///6 -> //8
//        else if(!getWorld()->canWalkStraight(getX(), getY(), playersX, playersY, someNumber))//)//frack is not in sight!
//        {
//            //cout << "pls" << endl;
//            
//            incNumSquares(-1); //PROBLEM: UNCOMMNET LATER
//            switch(getDirection())
//            {
//                    
//                case up:
//                    if (!getWorld()->isObstacle(getX(), getY()+1))
//                    {    moveTo(getX(), getY()+1);
//                        
//                        break;}
//                case down:
//                    if (!getWorld()->isObstacle(getX(), getY()-1))
//                    {    moveTo(getX(), getY()-1);
//                        
//                        break;}
//                case right:
//                    if (!getWorld()->isObstacle(getX()+1, getY()))
//                    {   moveTo(getX()+1, getY());
//                        
//                        break;}
//                case left:
//                    if (!getWorld()->isObstacle(getX()-1, getY()))
//                    {     moveTo(getX()-1, getY());
//                        
//                        break;}
//                default:
//                    //cout << "test" << endl;
//                    incNumSquares(0);
//                    
//                    
//            }
//        }
//        //return;
//    }
}


void Protestor::setState(State s)
{
    curState = s;
}


void Protestor::shout(bool shout)
{
    if (shout)
    {
        getWorld()->playSound(SOUND_PROTESTER_YELL);
    }
    m_shouted = shout;
    
}
bool Protestor::getShout()
{
    return m_shouted;
}


void Protestor::makeRightTurn(bool rightTurn)
{
    m_madeRightTurn = rightTurn;
}
bool Protestor::getRightTurn()
{
    return m_madeRightTurn;
}



void Protestor::getsGold()
{
    m_receivedGold = true;
}

bool Protestor::hasGold()
{
    return m_receivedGold;
}


int Protestor::getNumSquares()
{
    return numSquaresToMoveInCurrentDirection;
}

void Protestor::incNumSquares(int num)
{
    if (num == 0)
        numSquaresToMoveInCurrentDirection = 0;
    else
        numSquaresToMoveInCurrentDirection += num;
}




int Protestor::getInternalMovingTicks()
{
    return m_internalNonRestTicks;
}
void Protestor::setInternalMovingTicks(int num)
{
    if (num == 0)
        m_internalNonRestTicks = 0;
    else
        m_internalNonRestTicks += num;
}


int Protestor::getInternalRightTicks()
{
    return m_internalRightTicks;
}
void Protestor::setInternalRightTicks(int num)
{
    if (num == 0)
        m_internalRightTicks = 0;
    else
        m_internalRightTicks += num;
}






///////REGULAR PROTESTOR////////
RegularProtestor::RegularProtestor(StudentWorld* a, int numSquares): Protestor(a, numSquares, IID_PROTESTER)
{
    
}

void RegularProtestor::doSomething()
{
    //GET TICKS IS RESTING TICKS!!!!!!!!
    //GET TICKS COUNTS DOWN TO ZEROOOO
    //GET TICKS IS RESTING TICKS
    //SET TICKS IS SETTING TO REST!!! BITCH
    //INTERNAL TICKS IS MOVING TICKS
    
    bool through = false;
    int xOrYDir;
    if (!isAlive()) //1
        return;
    if (getHitPoints() <= 0 || hasGold())
    {
        setState(leave_the_oil_field);//kill();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        kill();
        setTicks(0);
        return;
    }
    
    ///IMPLEMENT LEAVE OIL FIELD
    
    if (getState() == leave_the_oil_field)
    {
        kill();
        return;
        //getWorld()->pathExists(this, getX(), getY(), getWorld()->accPlayer()->getX(), getWorld()->accPlayer()->getY());
        //getWorld()->getStack().fro
    }

    if (getTicks() > 0) //waiting state! //2
    {
        setState(rest);
        setTicks(-1);
        return;
    }
    else //getTicks() == 0 //3
    {
        setTicks(4);
    }
    if (getState() == isAnnoyed)
    {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        const int z = getWorld()->getLevel();
        
        setTicks(max(50,100-z*10));
    }

    
    
    if (getInternalMovingTicks() == 15) //shouting mechanism
    {
        shout(false);
        setInternalMovingTicks(0);
    }
    if (getShout())
    {
        setInternalMovingTicks(1);
    }
    
    if (getInternalRightTicks() == 200) //perpen mechanism
    {
        makeRightTurn(false);
        setInternalRightTicks(0);
    }
    if (getRightTurn())
    {
        setInternalRightTicks(1);
    }
    

    int someNumber;
    bool didMove = false;
    
    
    bool isNearFrackMan = getWorld()->isNearFrackman(getX(), getY(), through);
    int playersX = getWorld()->accPlayer()->getX();
    int playersY = getWorld()->accPlayer()->getY();
    Direction use;
    
    
    //4
    if (isNearFrackMan && getWorld()->isProtestorFacingFrackMan(this) && !getShout()) //shouting mechanism //4
    {
        shout(true);
        getWorld()->accPlayer()->incHitPoints(-2);
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        //setState(isAnnoyed);
        setInternalMovingTicks(1);
        return;
    }

    
    if (getNumSquares() <= 0 || getWorld()->isObstacle(getX(), getY())) //if no more squares, change direction! //6
    {
        incNumSquares(0); //reset squares
        incNumSquares(rand()%51 +8);
        do //PROBLEM: UNCOMMENT LATER
        {
            setDirection(getWorld()->chooseRandDir());
        }while(!getWorld()->isValidDir(this, getDirection()));
    }

    //cout << "ehelo " << getNumSquares() << endl;

    
    
        if (getNumSquares() > 0 && !getWorld()->isObstacle(getX(), getY())) //still squares!
        {
            
            ///5 //will only enter if frack is in sight
            if (getWorld()->canWalkStraight(getX(), getY(), playersX, playersY, someNumber) && !isNearFrackMan) //walk up
            {
                //cout << "can see frackman" << endl;
                
                switch(someNumber)
                {
                    case 1:
                        setDirection(up);
                        moveTo(getX(), getY()+1);
                        didMove = true;
                        break;
                    case 2:
                        setDirection(down);
                        moveTo(getX(), getY()-1);
                        didMove = true;
                        break;
                    case 3:
                        setDirection(right);
                        moveTo(getX()+1, getY());
                        didMove = true;
                        break;
                    case 4:
                        setDirection(left);
                        moveTo(getX()-1, getY());
                        didMove = true;
                        break;
                }
                incNumSquares(0); //reset
                return;
            }
            
            //7
            else if (getWorld()->canMovePerpen(this, use) && !getRightTurn())
            {
                makeRightTurn(true);
                setDirection(use);
                incNumSquares(0);
                
            }
            
            ///6 -> //8
            else if(!getWorld()->canWalkStraight(getX(), getY(), playersX, playersY, someNumber))//)//frack is not in sight!
            {
                //cout << "pls" << endl;
                
                incNumSquares(-1); //PROBLEM: UNCOMMNET LATER
                switch(getDirection())
                {
                        
                    case up:
                        if (!getWorld()->isObstacle(getX(), getY()+1))
                        {    moveTo(getX(), getY()+1);
                            
                            break;}
                    case down:
                        if (!getWorld()->isObstacle(getX(), getY()-1))
                        {    moveTo(getX(), getY()-1);
                        
                            break;}
                    case right:
                        if (!getWorld()->isObstacle(getX()+1, getY()))
                        {   moveTo(getX()+1, getY());
                       
                            break;}
                    case left:
                        if (!getWorld()->isObstacle(getX()-1, getY()))
                        {     moveTo(getX()-1, getY());
                        
                            break;}
                    default:
                        //cout << "test" << endl;
                        incNumSquares(0);
                        
                
                  }
            }
            //return;
        }

}

////////////////////HARDCOREPROTESTOR////////////////

HardcoreProtestor::HardcoreProtestor(StudentWorld* a, int numSquares):Protestor(a, numSquares, IID_HARD_CORE_PROTESTER)
{
    incHitPoints(20);
}

void HardcoreProtestor::doSomething()
{
    bool through = false;
    int xOrYDir;
    if (!isAlive()) //1
        return;
    if (getHitPoints() <= 0 || hasGold())
    {
        setState(leave_the_oil_field);//kill();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        kill();
        setTicks(0);
        return;
    }
    
    ///IMPLEMENT LEAVE OIL FIELD
    
    if (getState() == leave_the_oil_field)
    {
        kill();
        return;
    }
    
    if (getTicks() > 0) //waiting state! //2
    {
        setState(rest);
        setTicks(-1);
        return;
    }
    else
    {
        setTicks(4);
    }
    
    
    if (getInternalMovingTicks() == 15) //shouting mechanism
    {
        shout(false);
        setInternalMovingTicks(0);
    }
    if (getShout())
    {
        setInternalMovingTicks(1);
    }
    
    if (getInternalRightTicks() == 200) //perpen mechanism
    {
        makeRightTurn(false);
        setInternalRightTicks(0);
    }
    if (getRightTurn())
    {
        setInternalRightTicks(1);
    }
    
    
    int someNumber;
    bool didMove = false;
    
    
    bool isNearFrackMan = getWorld()->isNearFrackman(getX(), getY(), through);
    int playersX = getWorld()->accPlayer()->getX();
    int playersY = getWorld()->accPlayer()->getY();
    Direction use;
    
    
    //4
    if (isNearFrackMan && getWorld()->isProtestorFacingFrackMan(this) && !getShout()) //shouting mechanism //4
    {
        shout(true);
        getWorld()->accPlayer()->incHitPoints(-2);
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        setState(isAnnoyed);
        setInternalMovingTicks(1);
        return;
    }
    
    //7
    if (getNumSquares() <= 0) //if no more squares, change direction!
    {
        incNumSquares(0); //reset squares
        incNumSquares(rand()%51 +8);
        do //PROBLEM: UNCOMMENT LATER
        {
            setDirection(getWorld()->chooseRandDir());
        }while(!getWorld()->isValidDir(this, getDirection()));
    }
    
    
    if (getNumSquares() > 0 && !getWorld()->isObstacle(getX(), getY())) //still squares!
    {
        //5
        int const z = getWorld()->getLevel();
        
        int m = 16+z*2;
        //IMPLEMENT QUEUE ALGORITHM TO FRACKMAN!
        //GET NUMBRE OF STEPS TO PROTESTOR AND COMPARE TO M
        //CHAGNE CURR DIRECTION
        //MOVE ON SETP IN THIS DIR
        
        
        ///6 //will only enter if frack is in sight ***DIFFERENT
        if (getWorld()->canWalkStraight(getX(), getY(), playersX, playersY, someNumber) && !isNearFrackMan) //walk up
        {
            switch(someNumber)
            {
                case 1:
                    setDirection(up);
                    moveTo(getX(), getY()+1);
                    didMove = true;
                    break;
                case 2:
                    setDirection(down);
                    moveTo(getX(), getY()-1);
                    didMove = true;
                    break;
                case 3:
                    setDirection(right);
                    moveTo(getX()+1, getY());
                    didMove = true;
                    break;
                case 4:
                    setDirection(left);
                    moveTo(getX()-1, getY());
                    didMove = true;
                    break;
            }
            incNumSquares(0); //reset
            return;
        }
        
        //7
        else if (getWorld()->canMovePerpen(this, use) && !getRightTurn())
        {
            makeRightTurn(true);
            setDirection(use);
            incNumSquares(0);
            
        }
        
        ///6 -> //8
        else if(!getWorld()->canWalkStraight(getX(), getY(), playersX, playersY, someNumber))//)//frack is not in sight!
        {
            //cout << "pls" << endl;
            
            incNumSquares(-1); //PROBLEM: UNCOMMNET LATER
            switch(getDirection())
            {
                    
                case up:
                    if (!getWorld()->isObstacle(getX(), getY()+1))
                    {    moveTo(getX(), getY()+1);
                        
                        break;}
                case down:
                    if (!getWorld()->isObstacle(getX(), getY()-1))
                    {    moveTo(getX(), getY()-1);
                        
                        break;}
                case right:
                    if (!getWorld()->isObstacle(getX()+1, getY()))
                    {   moveTo(getX()+1, getY());
                        
                        break;}
                case left:
                    if (!getWorld()->isObstacle(getX()-1, getY()))
                    {     moveTo(getX()-1, getY());
                        
                        break;}
                default:
                    incNumSquares(0);
   
            }
        }
    }

}





/////////////------------------REGULAROBJECT--------------//////////////////

////////////////DIRT////////////
Dirt::Dirt(StudentWorld* a, int x, int y): Actor(IID_DIRT, a, x, y, right, .25, 3)
{
}

void Dirt::doSomething()
{
}




/////////////BOULDER/////////

Boulder::Boulder(StudentWorld* a, int x, int y):Actor(IID_BOULDER, a, x, y, down, 1.0, 1)
{
    curState = stable;
    getWorld()->destroyDirt(x, y);
    
}

void Boulder::doSomething()
{
    if (!isAlive())
        return;
    if (curState == waiting && getTicks() > 0)
    {
        setTicks(-1);
    }
    else if (curState == waiting && getTicks() == 0)
    {
        curState = falling;
    }
    else if (curState == falling)
    {
        
        if (getY() > 0 && !getWorld()->isDirtBelow(getX(), getY()) && !getWorld()->isBoulder(getX(), getY()-4))
        {
            moveTo(getX(), getY()-1);
            getWorld()->playSound(SOUND_FALLING_ROCK);
            
            //within radius
            setTicks(-1);
            
            bool through = false;
            Protestor* da;
            if (getWorld()->isNearFrackman(getX(), getY(), through) && through == true)
            {
                getWorld()->accPlayer()->incHitPoints(-100);
                return;
            }
            if (getWorld()->isNearProtestor(getX(), getY(), da))
            {
                da->incHitPoints(-100);
            }

            
            
            
            
        }
        if (getWorld()->isDirtBelow(getX(), getY()) || getY() == 0)
        {
            kill();
        }
//        setTicks(0);
//        curState = stable;
    }
    else if (curState == stable)
    {
        if (!getWorld()->isDirtBelow(getX(), getY()))
        {
            curState = waiting; //for the next 30 ticks?
            setTicks(30);
        }
    }
    
}


/////////////////////////SQUIRT/////
Squirt::Squirt(StudentWorld* a, int x, int y, Direction dir): Actor(IID_WATER_SPURT, a, x, y, dir, 1.0, 1)
{
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);// PROBLEM: SQUIRT HAS NO SOUND
    travelDist = 16;

}

void Squirt::doSomething()
{
    if (travelDist > 0)
    {
        travelDist--;
        Protestor* isP;
        if (getWorld()->isNearProtestor(getX(), getY(), isP))
        {
            isP->incHitPoints(-2);
            isP->setState(Protestor::isAnnoyed);
            
            kill();
        }
        else if (getDirection() == up && !getWorld()->isObstacle(getX(), getY()+2))
            moveTo(getX(), getY()+1);
        else if (getDirection() == down && !getWorld()->isObstacle(getX(), getY()))
            moveTo(getX(), getY()-1);
        else if (getDirection() == right && !getWorld()->isObstacle(getX()+2, getY()))
            moveTo(getX()+1, getY());
        else if (getDirection() == left && !getWorld()->isObstacle(getX(), getY()))
            moveTo(getX()-1, getY());
        else //if there is dirt there!
        {
            travelDist = 0;
            kill();
        }
        return;
    }
    else if (travelDist == 0)
    {
        kill();
        return;
    }
    
}
//////////////---------------ACTIVTATINGOBJECT----------//////////////

ActivatingObject::ActivatingObject(StudentWorld* a, int x, int y, int ID):Actor(ID, a, x, y, right, 1.0, 2)
{
    
}


//////////BARREL///////////
Barrel::Barrel(StudentWorld* a, int x, int y):ActivatingObject(a, x, y, IID_BARREL)
{
    setVisible(false); //REMOVE LATER
    plucked = false;
    getWorld()->incBarrelsCount(1);
}

void Barrel::doSomething()
{
    if (!isAlive())
    {
        return;
    }
    
    bool through = false;
    
    if (getWorld()->isNearFrackman(getX(), getY(), through) && through == false)
    {
        setVisible(true);
    }
    
    if (getWorld()->isNearFrackman(getX(), getY(), through) && through == true)
    {
        kill();
        getWorld()->incBarrelsCount(-1);
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(10000);//getWorld()->accPlayer()->increaseScore(1000);
    }
}

bool Barrel::pickedUp()
{
    return plucked;
}
//////////////GOLD///////
Gold::Gold(StudentWorld* a, int x, int y, bool isVis, int frackOrProt, int tempTicks):ActivatingObject(a, x, y, IID_GOLD)
{
    setVisible(isVis);
    switch(frackOrProt)
    {
        case 1: truePicker = frackman;
            curState = permanent;
            break;
        case 2: truePicker = protestor;
            curState = temporary;
            setTicks(tempTicks);
            break;
    }
}


void Gold::doSomething()
{
    if (!isAlive())
        return;
    Protestor* findProt;
    bool through = false;
    if (!isVisible() && getWorld()->isNearFrackman(getX(), getY(), through))
    {
        setVisible(true);
        return;
    }
    if(isVisible() &&  truePicker == frackman && getWorld()->isNearFrackman(getX(), getY(), through) && through == true)
    {
        kill();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->accPlayer()->incGold(1);
        return;
    }
    if(truePicker == protestor) //PROBLEM: MAYBE? NEED TO COMPLETE PROTESTOR FIRST
    { //cout << "pls" <<endl;
        if (getTicks() <= 0)
        {
            kill();
            return;
        }
        else if ( getTicks() > 0 && getWorld()->isNearProtestor(getX(), getY(), findProt))
        {
            kill();
            getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
            getWorld()->increaseScore(25);
        
            getWorld()->accPlayer()->incGold(-1);
            findProt->getsGold();
            return;
        }
        else
        {
            setTicks(-1);
        }
        
    }
}
/////////////SONAR////////
Sonar::Sonar(StudentWorld* a):ActivatingObject(a, 0, 60, IID_SONAR)
{
    //only pickupable by frackman. PROBLEM: maybe need code later
    //temporary state!!
    
    const int b = 300-10*getWorld()->getLevel();
    int T = max(100, b);
    setTicks(T);
}

void Sonar::doSomething() //how to fix do somehting for sonar and waterpool?
{
    setTicks(-1);
    if (!isAlive())
        return;
    if (getTicks() == 0)
        kill();
    bool through = false;
    if (getWorld()->isNearFrackman(getX(), getY(), through) && through == true)
    {
        kill();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->accPlayer()->incSonar(1);
        getWorld()->increaseScore(75);
    }
}
/////////WATERPOOL//////////
WaterPool::WaterPool(StudentWorld* a, int x, int y):ActivatingObject(a, x, y, IID_WATER_POOL)
{
    //only pickupable by frackman. PROBLEM: maybe need code later
    //temporary state!!
    
    const int b = 300-10*getWorld()->getLevel();
    int T = max(100, b);
    setTicks(T);
}

void WaterPool::doSomething()
{
    setTicks(-1);
    if (!isAlive())
        return;
    if (getTicks() == 0)
        kill();
    bool through = false;
    if (getWorld()->isNearFrackman(getX(), getY(), through) && through == true)
    {
        kill();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->accPlayer()->incSquirt(5);
        getWorld()->increaseScore(100);
    }
}

