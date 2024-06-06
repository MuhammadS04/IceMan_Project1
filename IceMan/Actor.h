
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir,
        bool visible, int imageID, double size, int depth);

    virtual void move() = 0;

    bool isAlive() const;
    void setDead();
    virtual bool annoy(unsigned int amt);
    StudentWorld* getWorld() const;
    virtual bool canActorsPassThroughMe() const;
    virtual bool canDigThroughIce() const;
    virtual bool canPickThingsUp() const;
    virtual bool huntsIceMan() const;
    virtual bool needsToBePickedUpToFinishLevel() const;
    bool moveToIfPossible(int x, int y);

private:
    bool m_isAlive;
    StudentWorld* m_world;
};

class Agent : public Actor {
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
        int imageID, unsigned int hitPoints);

    virtual void addGold() = 0;
    unsigned int getHitPoints() const;
    virtual bool annoy(unsigned int amount);
    virtual bool canPickThingsUp() const;

private:
    unsigned int m_hitPoints;
};

class IceMan : public Agent {
public:
    IceMan(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool canDigThroughIce() const;

    void addSonar();
    void addWater();
    unsigned int getGold() const;
    unsigned int getSonar() const;
    unsigned int getWater() const;

private:
    unsigned int m_gold;
    unsigned int m_sonar;
    unsigned int m_water;
};

class Protester : public Agent {
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
        unsigned int hitPoints, unsigned int score);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsIceMan() const;
    void setMustLeaveOilField();
    void setTicksToNextMove();

private:
    unsigned int m_score;
    bool m_leaveOilField;
    int m_ticksToNextMove;
};

class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};

class HardcoreProtester : public Protester {
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};

class Ice : public Actor {
public:
    Ice(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class Boulder : public Actor {
public:
    Boulder(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool canActorsPassThroughMe() const;

private:
    enum State { stable, waiting, falling } m_state;
    int m_waitingTicks;
};

class Squirt : public Actor {
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();

private:
    int m_ticksToLive;
};

class ActivatingObject : public Actor {
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
        int soundToPlay, bool activateOnPlayer,
        bool activateOnProtester, bool initiallyActive);
    virtual void move();
    void setTicksToLive();

private:
    int m_ticksToLive;
};

class OilBarrel : public ActivatingObject {
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const;
};

class GoldNugget : public ActivatingObject {
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool temporary);
    virtual void move();
};

class SonarKit : public ActivatingObject {
public:
    SonarKit(StudentWorld* world, int startX, int startY);
    virtual void move();
};

class WaterPool : public ActivatingObject {
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void move();
};

#endif // ACTOR_H_

























//---------------------------------------------------------------------------------------------------------------------------
//#ifndef ACTOR_H_
//#define ACTOR_H_
//
//#include "GraphObject.h"
//using namespace std;
//
//class StudentWorld;
//
//// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//
//class Actor : public GraphObject
//{
//private:
//	StudentWorld* world;
//	bool m_isAlive;
//public:
//	Actor(StudentWorld* world, int startX, int startY, Direction startDir,
//		bool visible, int imageID, double size, int depth)
//		: GraphObject(imageID, startX, startY, startDir, size, depth), m_isAlive(true), world(world)
//	{
//		setVisible(visible);
//	}
//	
//	bool isAlive()
//	{
//		return m_isAlive;
//	}
//
//	virtual void move() = 0;
//
//	StudentWorld* getWorld()
//	{
//		return world;
//	}
//
//	void setDead()
//	{
//		m_isAlive = false;
//		setVisible(false);
//	}
//
//	// Move this actor to x,y if possible, and return true; otherwise,
//	// return false without moving.
//		bool moveToIfPossible(int x, int y)
//	{
//		// Make sure x and y are within the screen
//		if (x < 0 || y < 0 || x >= VIEW_WIDTH - 2 || y >= VIEW_HEIGHT - 3)
//		{
//			return false;
//		}
//
//		////make sure its not blocked
//		//if (getWorld()->isBlocked(x, y))
//		//    return false;
//
//
//		GraphObject::moveTo(x, y);
//		return true;
//
//		//delete this
//		//if (getWorld()->canActorMoveTo(this, x, y))
//		//{
//		//	moveTo(x, y);
//		//	return true;
//		//}
//		//return false;
//	}
//	virtual bool annoy(unsigned int amt)
//	{
//		return true;
//	}
//
//	// Can other actors pass through this actor?
//	virtual bool canActorsPassThroughMe() const
//	{
//		return true;
//	}
//
//	// Can this actor dig through Ice?
//	virtual bool canDigThroughIce() const
//	{
//		return false;
//	}
//
//	// Can this actor pick items up?
//	virtual bool canPickThingsUp() const
//	{
//		return false;
//	}
//
//	// Does this actor hunt the IceMan?
//	virtual bool huntsIceMan() const
//	{
//		return false;
//	}
//
//	// Can this actor need to be picked up to finish the level?
//	virtual bool needsToBePickedUpToFinishLevel() const
//	{
//		return false;
//	}
//};
//
//class Agent : public Actor
//{
//private: 
//	unsigned int m_hitPoints;
//public:
//	Agent(StudentWorld* world, int startX, int startY, Direction startDir,
//		int imageID, unsigned int hitPoints)
//		: Actor(world, startX, startY, startDir, true, imageID, 1.0, 0), m_hitPoints(hitPoints)
//	{
//
//	}
//
//	// Pick up a gold nugget.
//	virtual void addGold() = 0;
//
//	// How many hit points does this actor have left?
//	unsigned int getHitPoints() const
//	{
//		return m_hitPoints;
//	}
//
//	virtual bool annoy(unsigned int amount);
//
//
//	virtual bool canPickThingsUp() const
//	{
//		return true;
//	}
//};
//
//class Iceman : public Agent
//{
//private:
//	unsigned int m_gold;
//	unsigned int m_water;
//	unsigned int m_sonar;
//	
//public:
//	Iceman(StudentWorld* world, int startX, int startY)
//		:Agent(world, startX, startY, right, IID_PLAYER, 10), m_gold(0) , m_sonar(1), m_water(5)
//	{
//		setVisible(true);
//	}
//
//	virtual void move();
//
//	virtual bool annoy(int amount);
//
//	void addGold()
//	{
//		m_gold++;
//	}
//	void addSonar()
//	{
//		m_sonar++;
//	}
//	void addWater(int n)
//	{
//		m_water += n;
//	}
//
//	unsigned int getGold()
//	{
//		return m_gold;
//	}
//
//	unsigned int getSonar() const {
//		return m_sonar;
//	}
//
//	unsigned int getWater() const {
//		return m_water;
//	}
//
//	virtual bool canDigThroughIce()
//	{
//		return true;
//	}
//};
//
//
//
//class Ice : public Actor
//{
//public:
//    Ice(StudentWorld* world, int startX, int startY)
//        : Actor(world, startX, startY, right, true, IID_ICE, .25, 3)
//    {
//        setVisible(true);
//    }
//
//	virtual void move() {};
//};
//
//class Boulder : public Actor {
//private:
//	enum State { STABLE, WAITING, FALLING };
//	State m_state;
//	int m_ticksToFall;
//	bool m_soundHasPlayed;
//
//public:
//	Boulder(StudentWorld* world, int startX, int startY)
//		: Actor(world, startX, startY, down, true, IID_BOULDER, 1.0, 1),
//		m_state(STABLE), m_ticksToFall(30), m_soundHasPlayed(false)
//	{
//		setVisible(true);
//	}
//
//	virtual void move();
//
//	virtual bool canActorsPassThroughMe() const override {
//		return false;
//	}
//
//	bool isIceBelow();
//	void annoyNearbyActors();
//	double distance(int x1, int y1, int x2, int y2) const;
//};
////class Boulder : public Actor
////{
////private:
////	bool m_alive = true;
////	bool m_falling = false;
////	int m_fallWait = 60;
////	bool m_soundHasPlayed = false;
////
////public:
////    Boulder(StudentWorld* world, int startX, int startY)
////        : Actor(world, startX, startY, down, true, IID_BOULDER, 1.0, 2)
////    {
////        setVisible(true);
//////		getWorld()->clearIce(startX, startY, KEY_PRESS_DOWN);
////    }
////
////	virtual void move();
////
////    virtual bool canActorsPassThroughMe() const override
////    {
////        return false;
////    }
////
////	bool isIceBelow();
////
////	//bool canFallTo(int x, int y) const;
////};
//
//class Squirt : public Actor
//{
//private:
//	unsigned int m_travelDis;
//public:
//	Squirt(StudentWorld* world, int startX, int startY, Direction startDir)
//		: Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1.0, 1)
//		, m_travelDis(4)
//	{
//		setVisible(true);
//	}
//
//	virtual void move();
//
//};
//
//
//class Protestor : public Agent
//{
//private: 
//	bool m_mustLeaveField;
//	int m_numOfTicks;
//	unsigned int m_score;
//public:
//	Protestor(StudentWorld* world, int startX, int startY)
//		: Agent(world, startX, startY, left, IID_PROTESTER, 9)
//		, m_mustLeaveField(false), m_score(0)
//	{
//		setVisible(true);
//		//m_numOfTicks = fmax(0, 3 - getWorld()->getLevel() / 4);
//		m_numOfTicks = 8;
//	}
//
//	virtual void move() {};
//	virtual bool annoy(unsigned int amount) {
//		return false;
//	}
//	virtual void addGold() {};
//	//virtual bool huntsIceman() const {};
//
//	//void setMustLeaveOilField();
//
//	//void setTicksToNextMove();
//
//};
//
//#endif // ACTOR_H_
