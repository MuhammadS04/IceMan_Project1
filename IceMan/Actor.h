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
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction startDir, double size, int depth)
		:GraphObject(imageID, startX, startY, startDir, size, depth), world(world)
	{
		setVisible(true);
	}


};

class Iceman : public Actor
{
public:
	Iceman(int startX, int startY, StudentWorld* world) : 
		Actor(world, IID_PLAYER, startX, startY, right, 4, 2)
	{
		setVisible(true);
	}
};

class Ice : public Actor
{
public:
    Ice(int startX, int startY, StudentWorld* world)
        : Actor(world, IID_ICE, startX, startY, right, .25, 3)
    {
        setVisible(true);
    }

    virtual void move() {};
};


#endif // ACTOR_H_
