#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
private:
	StudentWorld* world;
	bool m_isAlive;
public:
	Actor(StudentWorld* world, int startX, int startY, Direction startDir,
		bool visible, int imageID, double size, int depth)
		: GraphObject(imageID, startX, startY, startDir, size, depth), m_isAlive(true), world(world)
	{
		setVisible(visible);
	}

	bool isAlive()
	{
		return m_isAlive;
	}

	virtual void move() = 0;

	StudentWorld* getWorld()
	{
		return world;
	}

	void setDead()
	{
		m_isAlive = false;
		setVisible(false);
	}

	// Move this actor to x,y if possible, and return true; otherwise,
	// return false without moving.
	bool moveToIfPossible(int x, int y)
	{
		// Make sure x and y are within the screen
		if (x < 0 || y < 0 || x >= VIEW_WIDTH - 2 || y >= VIEW_HEIGHT - 3)
		{
			return false;
		}

		////make sure its not blocked
		//if (getWorld()->isBlocked(x, y))
		//    return false;


		GraphObject::moveTo(x, y);
		return true;

		//delete this
		//if (getWorld()->canActorMoveTo(this, x, y))
		//{
		//	moveTo(x, y);
		//	return true;
		//}
		//return false;
	}
	virtual bool annoy(unsigned int amt)
	{
		return true;
	}

	// Can other actors pass through this actor?
	virtual bool canActorsPassThroughMe() const
	{
		return true;
	}

	// Can this actor dig through Ice?
	virtual bool canDigThroughIce() const
	{
		return false;
	}

	// Can this actor pick items up?
	virtual bool canPickThingsUp() const
	{
		return false;
	}

	// Does this actor hunt the IceMan?
	virtual bool huntsIceMan() const
	{
		return false;
	}

	// Can this actor need to be picked up to finish the level?
	virtual bool needsToBePickedUpToFinishLevel() const
	{
		return false;
	}
};

class Agent : public Actor
{
private:
	unsigned int m_hitPoints;
public:
	Agent(StudentWorld* world, int startX, int startY, Direction startDir,
		int imageID, unsigned int hitPoints)
		: Actor(world, startX, startY, startDir, true, imageID, 1.0, 0), m_hitPoints(hitPoints)
	{

	}

	// Pick up a gold nugget.
	virtual void addGold() = 0;

	// How many hit points does this actor have left?
	unsigned int getHitPoints() const
	{
		return m_hitPoints;
	}

	virtual bool annoy(unsigned int amount);


	virtual bool canPickThingsUp() const
	{
		return true;
	}
};

class Iceman : public Agent
{
private:
	unsigned int m_gold;
	unsigned int m_water;
	unsigned int m_sonar;

public:
	Iceman(StudentWorld* world, int startX, int startY)
		:Agent(world, startX, startY, right, IID_PLAYER, 10), m_gold(0), m_sonar(1), m_water(5)
	{
		setVisible(true);
	}

	virtual void move();

	virtual bool annoy(int amount);

	void addGold()
	{
		m_gold++;
	}
	void addSonar()
	{
		m_sonar++;
	}
	void addWater()
	{
		m_water += 5;
	}

	unsigned int getGold()
	{
		return m_gold;
	}

	unsigned int getSonar() const {
		return m_sonar;
	}

	unsigned int getWater() const {
		return m_water;
	}

	virtual bool canDigThroughIce()
	{
		return true;
	}
};



class Ice : public Actor
{
public:
	Ice(StudentWorld* world, int startX, int startY)
		: Actor(world, startX, startY, right, true, IID_ICE, .25, 3)
	{
		setVisible(true);
	}

	virtual void move() {};
};

class Boulder : public Actor
{
private:
	unsigned int m_state;
	unsigned int m_numTicks;
public:
	Boulder(StudentWorld* world, int startX, int startY)
		: Actor(world, startX, startY, down, true, IID_BOULDER, 1.0, 1)
		, m_state(1), m_numTicks(0)
	{}

	virtual void move();

};

class Squirt : public Actor
{
private:
	unsigned int m_travelDis;
public:
	Squirt(StudentWorld* world, int startX, int startY, Direction startDir)
		: Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1.0, 1)
		, m_travelDis(4)
	{	}

	virtual void move();

};

class Protestor : public Actor {
private:
	int m_ticksToWait;
	bool m_leavingOilField;
	int m_shoutTicks;
	int m_ticksToTurn;
	int m_numSquaresToMoveInCurrentDirection;
	bool m_hasPickedUpGold;
	int m_waitingTicks;

public:
	Protestor(StudentWorld* world, int imageID = IID_PROTESTER)
		: Actor(world, 60, 60, left, true, imageID, 1.0, 0),
		m_ticksToWait(0), m_leavingOilField(false),
		m_shoutTicks(0), m_ticksToTurn(0),
		m_numSquaresToMoveInCurrentDirection(0),
		m_hasPickedUpGold(false), m_waitingTicks(0) {}

	virtual void move();

};


#endif // ACTOR_H_
