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
	Actor(StudentWorld* world, int imageID, int startX, int startY, bool isAlive, Direction startDir, double size, int depth)
		:GraphObject(imageID, startX, startY, startDir, size, depth), world(world), m_isAlive(isAlive)
	{
		setVisible(true);
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
	}

	// Move this actor to x,y if possible, and return true; otherwise,
	// return false without moving.
	bool moveToIfPossible(int x, int y)
	{
		//make sure the x and y are within the screen
		if (x > 60 || y > 60)
		{
			return false;
		}

		////make sure its not blocked
		//if (getWorld()->isBlocked(x, y))
		//    return false;


		GraphObject::moveTo(x, y);
		return true;
	}

};

class Iceman : public Actor
{
public:
	Iceman(int startX, int startY, StudentWorld* world) : 
		Actor(world, IID_PLAYER, startX, startY, true, right, 4, 2)
	{
		setVisible(true);
	}

	virtual void move();

};

class Ice : public Actor
{
public:
    Ice(int startX, int startY, StudentWorld* world)
        : Actor(world, IID_ICE, startX, startY, true, right, .25, 3)
    {
        setVisible(true);
    }

	virtual void move() {};
};


#endif // ACTOR_H_
