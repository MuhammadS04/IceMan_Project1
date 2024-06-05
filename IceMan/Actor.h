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
	//virtual void addGold() = 0;

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
	
public:
	Iceman(StudentWorld* world, int startX, int startY)
		:Agent(world, startX, startY, right, IID_PLAYER, 10)
	{}

	virtual void move();

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

#endif // ACTOR_H_
