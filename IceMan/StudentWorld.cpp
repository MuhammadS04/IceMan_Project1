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



	m_iceman = new Iceman(this, 30, 60);

    for (int x = 0; x <= 64; x++) {                              
        for (int y = 0; y < 60; y++) {                          
            if (x >= 30 && x <= 33 && y >= 4 && y <= 59) {
                m_iceField[y][x] = nullptr;                     
            }
            else if (x >= 0 && x <= 64 && y >= 60 && y <= 64) {
                m_iceField[y][x] = nullptr;                     
            }
            else {
                m_iceField[y][x] = new Ice(this, x, y);
            }
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
    if (!m_iceman->isAlive())
    {
        return GWSTATUS_PLAYER_DIED;
    }
}

void StudentWorld::clearIce(int x, int y) 
{

    for (int i = x; i < x + 4; i++)
    {
        for (int j = y; j < y + 4; j++)
        {
            if (i >= 0 && i < 64 && j >= 0 && j < 64 && m_iceField[i][j] != nullptr) // Check within bounds and if initialized
            {
                delete m_iceField[j][i];
                m_iceField[i][j] = nullptr;
            }
        }
    }
}

