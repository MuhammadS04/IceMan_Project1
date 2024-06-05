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
    return GWSTATUS_CONTINUE_GAME;

}

void StudentWorld::clearIce(int x, int y, int dir) 
{
    int startX = x;
    int startY = y;
    int endX = x + 4;
    int endY = y + 4;

    // Adjust the area to clear based on the direction
    switch (dir) {
    case KEY_PRESS_LEFT:
        startX = x - 1;
        endX = x + 3;
        break;
    case KEY_PRESS_RIGHT:
        startX = x;
        endX = x + 4;
        break;
    case KEY_PRESS_UP:
        startY = y;
        endY = y + 4;
        break;
    case KEY_PRESS_DOWN:
        startY = y - 1;
        endY = y + 3;
        break;
    }

    for (int i = startX; i < endX; i++) {
        for (int j = startY; j < endY; j++) {
            if (i >= 0 && i < 64 && j >= 0 && j < 60 && m_iceField[j][i] != nullptr) {
                delete m_iceField[j][i];
                m_iceField[j][i] = nullptr;
            }
        }
    }
}

