#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{



	m_iceman = new Iceman(30, 60, this);

    for (int x = 0; x < 64; ++x)
    {
        for (int y = 0; y < 64; ++y)
        {
            iceField[x][y] = new Ice(x, y, this);
        }
    }





	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_iceman->move();
	//// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	//// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();

	return GWSTATUS_PLAYER_DIED;
}