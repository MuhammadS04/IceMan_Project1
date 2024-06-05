#include "StudentWorld.h"
#include <string>
#include <iomanip>
#include <sstream>
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

    updateDisplayText();
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

void StudentWorld::updateDisplayText() {
    // Get info
    int level = getLevel();
    int lives = getLives();
    int hp = m_iceman->getHitPoints() * 10; // Since IceMan has 10 health, we *10 to keep track of percentage
    int squirts = m_iceman->getWater();
    int gold = m_iceman->getGold();
    int barrelsLeft = getBarrelsLeft();
    int sonar = m_iceman->getSonar();
    int score = getScore();

    // Format the info using sstream
    ostringstream oss;
    oss << "Lvl: " << setw(2) << setfill(' ') << level << " "
        << "Lives: " << lives << " "
        << "Hlth: " << setw(3) << setfill(' ') << hp << "% "
        << "Wtr: " << setw(2) << setfill(' ') << squirts << " "
        << "Gld: " << setw(2) << setfill(' ') << gold << " "
        << "Oil Left: " << setw(2) << setfill(' ') << barrelsLeft << " "
        << "Sonar: " << setw(2) << setfill(' ') << sonar << " "
        << "Scr: " << setw(6) << setfill('0') << score;

    // Update the display text
    setGameStatText(oss.str());
}

unsigned int StudentWorld::getBarrelsLeft()
{
    return m_barrelsLeft;
}

void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}

int StudentWorld::annoyAllNearbyActors(Actor* a, int points, int radius) {
    int actorsAnnoyed = 0;

    // annoyer coordinates
    int annoyerX = a->getX();
    int annoyerY = a->getY();

    // loop through actors
    for (const auto& actor : m_actors) {
        // calculate the euclidian distance
        int distance = sqrt(pow(actor->getX() - annoyerX, 2) + pow(actor->getY() - annoyerY, 2));

        if (distance <= radius) {   // ** MAYBE CHECK IF ACTOR CAN BE ANNOYED HERE **
            // annoy actor and increase the count
            actor->annoy(points);
            actorsAnnoyed++;
        }
    }
    return actorsAnnoyed;
}