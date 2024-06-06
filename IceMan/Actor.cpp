#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

// Actor implementation

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir,
    bool visible, int imageID, double size, int depth)
    : GraphObject(imageID, startX, startY, startDir, size, depth), m_world(world), m_isAlive(true) {
    setVisible(visible);
}

bool Actor::isAlive() const {
    return m_isAlive;
}

void Actor::setDead() {
    m_isAlive = false;
}

bool Actor::annoy(unsigned int amt) {
    return false;
}

StudentWorld* Actor::getWorld() const {
    return m_world;
}

bool Actor::canActorsPassThroughMe() const {
    return false;
}

bool Actor::canDigThroughIce() const {
    return false;
}

bool Actor::canPickThingsUp() const {
    return false;
}

bool Actor::huntsIceMan() const {
    return false;
}

bool Actor::needsToBePickedUpToFinishLevel() const {
    return false;
}

bool Actor::moveToIfPossible(int x, int y) {
    if (getWorld()->canActorMoveTo(this, x, y)) {
        moveTo(x, y);
        return true;
    }
    return false;
}

// Agent implementation

Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir,
    int imageID, unsigned int hitPoints)
    : Actor(world, startX, startY, startDir, true, imageID, 1.0, 0), m_hitPoints(hitPoints) {}

unsigned int Agent::getHitPoints() const {
    return m_hitPoints;
}

bool Agent::annoy(unsigned int amount) {
    if (amount >= m_hitPoints) {
        m_hitPoints = 0;
        setDead();
        return true;
    }
    m_hitPoints -= amount;
    return true;
}

bool Agent::canPickThingsUp() const {
    return true;
}

// IceMan implementation

IceMan::IceMan(StudentWorld* world, int startX, int startY)
    : Agent(world, startX, startY, right, IID_PLAYER, 10), m_gold(0), m_sonar(1), m_water(5) {
    setVisible(true);
}

void IceMan::move() {
    if (!isAlive()) return;

    int key;
    if (getWorld()->getKey(key)) {
        switch (key) {
        case KEY_PRESS_LEFT:
            if (getDirection() != left)
                setDirection(left);
            else if (moveToIfPossible(getX() - 1, getY()))
                getWorld()->clearIce(getX(), getY());
            break;
        case KEY_PRESS_RIGHT:
            if (getDirection() != right)
                setDirection(right);
            else if (moveToIfPossible(getX() + 1, getY()))
                getWorld()->clearIce(getX(), getY());
            break;
        case KEY_PRESS_UP:
            if (getDirection() != up)
                setDirection(up);
            else if (moveToIfPossible(getX(), getY() + 1))
                getWorld()->clearIce(getX(), getY());
            break;
        case KEY_PRESS_DOWN:
            if (getDirection() != down)
                setDirection(down);
            else if (moveToIfPossible(getX(), getY() - 1))
                getWorld()->clearIce(getX(), getY());
            break;
        case KEY_PRESS_SPACE:
            if (m_water > 0) {
                // Shoot squirt
                int x = getX();
                int y = getY();
                Direction dir = getDirection();
                if (dir == left) x -= 4;
                else if (dir == right) x += 4;
                else if (dir == up) y += 4;
                else if (dir == down) y -= 4;
                if (getWorld()->canActorMoveTo(this, x, y)) {
                    getWorld()->addActor(new Squirt(getWorld(), x, y, dir));
                    m_water--;
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                }
            }
            break;
        case 'z':
        case 'Z':
            if (m_sonar > 0) {
                m_sonar--;
                getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
            }
            break;
        case KEY_PRESS_TAB:
            if (m_gold > 0) {
                m_gold--;
                getWorld()->addActor(new GoldNugget(getWorld(), getX(), getY(), true));
            }
            break;
        case KEY_PRESS_ESCAPE:
            setDead();
            break;
        }
    }
}

bool IceMan::annoy(unsigned int amount) {
    if (Agent::annoy(amount)) {
        if (getHitPoints() == 0) {
            // IceMan died
            getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        }
        return true;
    }
    return false;
}

void IceMan::addGold() {
    m_gold++;
}

bool IceMan::canDigThroughIce() const {
    return true;
}

void IceMan::addSonar() {
    m_sonar++;
}

void IceMan::addWater() {
    m_water += 5;
}

unsigned int IceMan::getGold() const {
    return m_gold;
}

unsigned int IceMan::getSonar() const {
    return m_sonar;
}

unsigned int IceMan::getWater() const {
    return m_water;
}

// Protester implementation

Protester::Protester(StudentWorld* world, int startX, int startY, int imageID,
    unsigned int hitPoints, unsigned int score)
    : Agent(world, startX, startY, left, imageID, hitPoints), m_score(score), m_leaveOilField(false), m_ticksToNextMove(0) {
    setVisible(true);
}

void Protester::move() {
    if (!isAlive()) return;

    // Implement Protester specific move logic
    if (m_leaveOilField) {
        // logic to move towards the exit
    }
    else {
        // regular protester behavior (shouting, moving randomly, etc.)
    }
}

bool Protester::annoy(unsigned int amount) {
    if (Agent::annoy(amount)) {
        if (getHitPoints() == 0) {
            setMustLeaveOilField();
            getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        }
        return true;
    }
    return false;
}

void Protester::addGold() {
    setMustLeaveOilField();
}

bool Protester::huntsIceMan() const {
    return true;
}

void Protester::setMustLeaveOilField() {
    m_leaveOilField = true;
}

void Protester::setTicksToNextMove() {
    m_ticksToNextMove = std::max(0, m_ticksToNextMove - 1);
}

// RegularProtester implementation

RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY, int imageID)
    : Protester(world, startX, startY, imageID, 5, 100) {
}

void RegularProtester::move() {
    Protester::move();
    // RegularProtester specific move logic
}

void RegularProtester::addGold() {
    Protester::addGold();
    // RegularProtester specific add gold logic
}

// HardcoreProtester implementation

HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID)
    : Protester(world, startX, startY, imageID, 20, 250) {
}

void HardcoreProtester::move() {
    Protester::move();
    // HardcoreProtester specific move logic
}

void HardcoreProtester::addGold() {
    Protester::addGold();
    // HardcoreProtester specific add gold logic
}

// Ice implementation

Ice::Ice(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, right, true, IID_ICE, 0.25, 3) {
}

void Ice::move() {
    // Ice does not move
}

// Boulder implementation

Boulder::Boulder(StudentWorld* world, int startX, int startY)
    : Actor(world, startX, startY, down, true, IID_BOULDER, 1.0, 1), m_state(stable), m_waitingTicks(0) {
}

void Boulder::move() {
    if (!isAlive()) return;

    switch (m_state) {
    case stable:
        if (!getWorld()->canActorMoveTo(this, getX(), getY() - 1)) {
            m_state = waiting;
            m_waitingTicks = 30;
        }
        break;
    case waiting:
        if (m_waitingTicks > 0) {
            m_waitingTicks--;
        }
        else {
            m_state = falling;
            getWorld()->playSound(SOUND_FALLING_ROCK);
        }
        break;
    case falling:
        if (getWorld()->canActorMoveTo(this, getX(), getY() - 1)) {
            moveTo(getX(), getY() - 1);
        }
        else {
            setDead();
        }
        break;
    }
}

bool Boulder::canActorsPassThroughMe() const {
    return false;
}

// Squirt implementation

Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir)
    : Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1.0, 1), m_ticksToLive(4) {
}

void Squirt::move() {
    if (!isAlive()) return;

    if (m_ticksToLive > 0) {
        int newX = getX(), newY = getY();
        switch (getDirection()) {
        case left:  newX -= 1; break;
        case right: newX += 1; break;
        case up:    newY += 1; break;
        case down:  newY -= 1; break;
        default: break;
        }
        if (getWorld()->canActorMoveTo(this, newX, newY)) {
            moveTo(newX, newY);
            m_ticksToLive--;
        }
        else {
            setDead();
        }
    }
    else {
        setDead();
    }
}

// ActivatingObject implementation

ActivatingObject::ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
    int soundToPlay, bool activateOnPlayer,
    bool activateOnProtester, bool initiallyActive)
    : Actor(world, startX, startY, right, initiallyActive, imageID, 1.0, 2), m_ticksToLive(0) {
}

void ActivatingObject::move() {
    if (!isAlive()) return;

    // ActivatingObject specific move logic
}

void ActivatingObject::setTicksToLive() {
    m_ticksToLive = 0;
}

// OilBarrel implementation

OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
    : ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL, true, false, false) {
}

void OilBarrel::move() {
    if (!isAlive()) return;

    if (isVisible() && getWorld()->isNearIceMan(this, 4)) {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(1000);
        setDead();
    }
    else if (!isVisible() && getWorld()->isNearIceMan(this, 4)) {
        setVisible(true);
    }
}

bool OilBarrel::needsToBePickedUpToFinishLevel() const {
    return true;
}

// GoldNugget implementation

GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool temporary)
    : ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, true, true, temporary) {
}

void GoldNugget::move() {
    if (!isAlive()) return;

    if (isVisible() && getWorld()->isNearIceMan(this, 4)) {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        setDead();
    }
    else if (!isVisible() && getWorld()->isNearIceMan(this, 4)) {
        setVisible(true);
    }
}

// SonarKit implementation

SonarKit::SonarKit(StudentWorld* world, int startX, int startY)
    : ActivatingObject(world, startX, startY, IID_SONAR, SOUND_GOT_GOODIE, true, false, true) {
}

void SonarKit::move() {
    if (!isAlive()) return;

    if (getWorld()->isNearIceMan(this, 4)) {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(75);
        setDead();
        getWorld()->giveIceManSonar();
    }
}

// WaterPool implementation

WaterPool::WaterPool(StudentWorld* world, int startX, int startY)
    : ActivatingObject(world, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, true) {
}

void WaterPool::move() {
    if (!isAlive()) return;

    if (getWorld()->isNearIceMan(this, 4)) {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(100);
        setDead();
        getWorld()->giveIceManWater();
    }
}


//===========================================================================
//#include "Actor.h"
//#include "StudentWorld.h"
//
//
//bool Agent::annoy(unsigned int amount)
//{
//	if (m_hitPoints <= amount)
//	{
//		m_hitPoints = 0;
//		setDead();
//		return true;
//	}
//	else
//	{
//		m_hitPoints -= amount;
//		return false;
//	}
//}
//
////ICEMAN__________________________________________________________
//void Iceman::move()
//{
//	if (!isAlive()) return;
//
//
//	int ch;
//	if (getWorld()->getKey(ch))
//        switch (ch)
//        {
//        case KEY_PRESS_ESCAPE:
//            annoy(100);
//            break;
//
//        case KEY_PRESS_SPACE:
//            if (getWater() > 0) {
//                int squirtX = getX();
//                int squirtY = getY();
//                switch (getDirection()) {
//                case up:
//                    squirtY += 1;
//                    break;
//                case down:
//                    squirtY -= 1;
//                    break;
//                case left:
//                    squirtX -= 1;
//                    break;
//                case right:
//                    squirtX += 1;
//                    break;
//                }
//                if (getWorld()->canActorMoveTo(this, squirtX, squirtY)) {
//                    Squirt* s = new Squirt(getWorld(), squirtX, squirtY, getDirection());
//                    getWorld()->addActor(s);
//                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
//                    m_water--;
//                }
//            }
//            break;
//        case KEY_PRESS_DOWN:
//            if (getDirection() == down)
//                moveToIfPossible(getX(), getY() - 1);
//            else
//                setDirection(down);
//            break;
//        case KEY_PRESS_LEFT:
//            if (getDirection() == left)
//                moveToIfPossible(getX() - 1, getY());
//            else
//                setDirection(left);
//            break;
//        case KEY_PRESS_RIGHT:
//            if (getDirection() == right)
//            {
//                moveToIfPossible(getX() + 1, getY());
//            }
//            else
//                setDirection(right);
//            break;
//        case KEY_PRESS_UP:
//            if (getDirection() == up)
//                moveToIfPossible(getX(), getY() + 1);
//            else
//                setDirection(up);
//            break;
//
//        }
//    
//    getWorld()->clearIce(getX(), getY(), getDirection());
//
//}
//
//bool Iceman::annoy(int amount)
//{
//    if (Agent::annoy(amount)) {
//        setDead();
//        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
//        return true;    // return true if IceMan is completely annoyed
//    }
//    return false;       // still got HP left
//}
//
//
//
////Boulder-------------------------------------------------------------
////void Boulder::move()
////{
////    if (!(isIceBelow())) {
////        m_falling = true;
////    }
////    if (m_falling == true) {
////        if (m_fallWait > 0) {
////            m_fallWait--;
////        }
////        else {
////            if (m_soundHasPlayed == false) {
////                getWorld()->playSound(SOUND_FALLING_ROCK);
////                m_soundHasPlayed = true;
////            }
////            if (isIceBelow() || getY() == 0) {
////                setDead();
////            }
////            else
////            {
////                moveTo(getX(), getY() - 1);
////            }
////        }
////    }
////
////
////
//////    if (!isAlive()) return;
//////
//////    if (m_state == STATE_STABLE)
//////    {
//////        if (!isIceBelow())
//////        {
//////            m_state = STATE_WAITING;
//////            m_waitTicks = 30;
//////        }
//////    }
//////    else if (m_state == STATE_WAITING)
//////    {
//////        if (m_waitTicks > 0)
//////        {
//////            m_waitTicks--;
//////        }
//////        else
//////        {
//////            m_state = STATE_FALLING;
//////            getWorld()->playSound(SOUND_FALLING_ROCK);
//////        }
//////
//////    }
//////    else if (m_state == STATE_FALLING)
//////    {
//////        if (isIceBelow() || getY() == 0) {
//////            setDead();
//////        }
//////        else {
//////            moveTo(getX(), getY() - 1);
//////        }
//////        ////Doing check ice only for now, need to add check for other boulders too
//////        //if (!isIceBelow())
//////        //{
//////        //    moveTo(getX(), getY() - 6);
//////        //   // getWorld()->boulderProtestor(int x, int y);
//////        //    //^ this function should check if there is a protestor at that location and
//////        //    // annoy(100) if there is.
//////        //}
//////        //else
//////        //{
//////        //    setDead();
//////        //}
//////
//////       /* if (canFallTo(getX(), getY() - 1))
//////        {
//////            moveTo(getX(), getY() - 1);
//////            getWorld()->checkr(this);
//////        }
//////        else
//////        {
//////            setDead();
//////        }*/
//////    }
////}
//
//
////bool Boulder::isIceBelow()
////{
////    int x = getX();
////    int y = getY() - 1;
////
////    for (int i = 0; i < 4; ++i)
////    {
////        if (getWorld()->checkIce(x + i, y))
////        {
////            return true;
////        }
////    }
////    return false;
////}
//
////bool Boulder::canFallTo(int x, int y) const
////{
////    if (y < 0 || getWorld()->checkIce(x, y)
////    {
////        return false;
////    }
////    return true;
////}
//
//bool Boulder::isIceBelow() {
//    int x = getX();
//    int y = getY() - 1;
//
//    for (int i = 0; i < 4; ++i) {
//        if (getWorld()->checkIce(x + i, y)) {
//            return true;
//        }
//    }
//    return false;
//}
//
//void Boulder::move() {
//    if (!isAlive()) return;
//
//    switch (m_state) {
//    case STABLE:
//        if (!isIceBelow()) {
//            m_state = WAITING;
//            m_ticksToFall = 30; // Start the countdown
//        }
//        break;
//
//    case WAITING:
//        if (m_ticksToFall > 0) {
//            m_ticksToFall--;
//        }
//        else {
//            m_state = FALLING;
//            if (!m_soundHasPlayed) {
//                getWorld()->playSound(SOUND_FALLING_ROCK);
//                m_soundHasPlayed = true;
//            }
//        }
//        break;
//
//    case FALLING:
//        if (getY() == 0 || isIceBelow()) {
//            setDead();
//        }
//        else {
//            moveTo(getX(), getY() - 1);
//            annoyNearbyActors();
//        }
//        break;
//    }
//}
//
//void Boulder::annoyNearbyActors() {
//    // Logic to annoy actors within a radius of 3
//    std::vector<Actor*> actors = getWorld()->getActors();
//    for (Actor* actor : actors) {
//        if (distance(getX(), getY(), actor->getX(), actor->getY()) <= 3) {
//            actor->annoy(100); // Assuming the Actor class has an annoy method
//        }
//    }
//
//    //if (distance(getX(), getY(), getWorld()->getIceman()->getX(), getWorld()->getIceman()->getY()) <= 3) {
//    //    getWorld()->getIceman()->annoy(100); // Assuming the Iceman class has an annoy method
//    //}
//}
//
//double Boulder::distance(int x1, int y1, int x2, int y2) const {
//    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
//}
//
//
//
//
//
////SQUIRT----------------------------------------------------------------
//void Squirt::move()
//{
//    if (!isAlive()) return;
//
//    //Check if it hit protestors
//    for (auto* protestor : getWorld()->getProtestors())
//    {
//        if (!protestor->isAlive() == false &&
//            getWorld()->checkRadius(getX(), getY(), protestor->getX(), protestor->getY(), 3))
//        {
//            protestor->annoy(2);
//            setDead();
//            return;
//        }
//
//        int nextX = getX();
//        int nextY = getY();
//
//        switch (getDirection())
//        {
//        case left:  nextX--; break;
//        case right: nextX++; break;
//        case up:    nextY++; break;
//        case down:  nextY--; break;
//        default:    break;
//        }
//
//        //check if it hit ice or boulders
//        if (getWorld()->checkIceBoulder(nextX, nextY, this->getDirection()))
//        {
//            setDead();
//            return;
//        }
//
//        //int nextX = getX();
//        //int nextY = getY();
//
//        //switch (getDirection())
//        //{
//        //case left:  nextX--; break;
//        //case right: nextX++; break;
//        //case up:    nextY++; break;
//        //case down:  nextY--; break;
//        //default:    break;
//        //}
//
//        if (m_travelDis == 0)
//        {
//            setDead();
//            return;
//        }
//
//        moveTo(nextX, nextY);
//        m_travelDis--;
//    }
//}
//
//
//// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
