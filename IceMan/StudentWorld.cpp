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

    int B = std::min((int)getLevel() / 2 + 2, 9); /// boulder

   // unsigned int target_num_protester = std::min(15, int(2 + getLevel() * 1.5)); /// protester

    createBoulders(B);

    Protestor* n1 = new Protestor(this, 60, 60);
    Protestor* n2 = new Protestor(this, 60, 10);
    Protestor* n3 = new Protestor(this, 10, 10);

    m_protestors.emplace_back(n2);
    m_protestors.emplace_back(n1);
    m_protestors.emplace_back(n3);


	return GWSTATUS_CONTINUE_GAME;
}

vector<Protestor*> StudentWorld::getProtestors()
{
    return m_protestors;
}

void StudentWorld::createBoulders(int numBoulders)
{
    for (int i = 0; i < numBoulders; ++i)
    {
        int x, y;

        //// Generate a random position for the boulder
        //// Ensure the boulder is not placed out of bounds or overlapping with important objects
        //do
        //{
        //    x = rand() % 61; // Random x position between 0 and 60
        //    y = rand() % 37 + 20; // Random y position between 20 and 56
        //} while (isIceAt(x, y) || isBoulderAt(x, y)); // Add your own conditions for valid positions

        x = rand() % 61; // Random x position between 0 and 60
        y = rand() % 37 + 20; // Random y position between 20 and 56

        // Clear ice in a 4x4 region around the boulder's starting position
        //clearIce(x, y);

        // Create a new Boulder object and add it to the vector of actors
        Boulder* newBoulder = new Boulder(this ,x ,y);
        m_actors.push_back(newBoulder);
    }
}

int StudentWorld::move()
{

    // update the game status text
    updateDisplayText();

    //// give each Actor a chance to do something
    //m_iceman->move();
 
    //if (!m_iceman->isAlive())
    //{
    //    return GWSTATUS_PLAYER_DIED;
    //}
    //return GWSTATUS_CONTINUE_GAME;

    if (m_iceman->isAlive())
    {
        m_iceman->move();
        for (auto* a : m_actors)
        {
            a->move();
            return GWSTATUS_CONTINUE_GAME;
        }
        return GWSTATUS_CONTINUE_GAME;
        if (!m_iceman->isAlive())
        {
            return GWSTATUS_PLAYER_DIED;
        }
    }

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
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
    // Check boundaries
    if (x < 0 || x >= 64 || y < 0 || y >= 64) {
        return false;
    }

    // Check if the location is occupied by Ice
    for (int i = x; i < x + 4 && i < 64; i++) {
        for (int j = y; j < y + 4 && j < 60; j++) {
            if (m_iceField[j][i] != nullptr) {
                return false;
            }
        }
    }

    // Check if the location is occupied by a Boulder
    for (const auto& actor : m_actors) {
        if (dynamic_cast<Boulder*>(actor) != nullptr) {
            int boulderX = actor->getX();
            int boulderY = actor->getY();
            if (x >= boulderX && x < boulderX + 4 && y >= boulderY && y < boulderY + 4) {
                return false;
            }
        }
    }

    return true;
}


bool StudentWorld::isNearIceman(Actor* a, int radius) const
{
    double distance = sqrt(intPow(a->getX() - m_iceman->getX()) + intPow(a->getY() - m_iceman->getY()));
    if (distance <= radius) {
        return true;
    }
    return false;
}

//bool StudentWorld::checkRadius(int x, int y, double radius) const
//{
//    for (Actor* a : m_actors) {
//        int actor_x = a->getX();
//        int actor_y = a->getY();
//        double distance = sqrt(intPow(actor_x - x) + intPow(actor_y - y));
//        if (distance > radius)
//            continue;
//        else
//            return true;
//    }
//    return false;
//}

bool StudentWorld::checkRadius(int x1, int y1, int x2, int y2, double radius) const
{
    double dx = x1 - x2;
    double dy = y1 - y2;
    return (dx * dx + dy * dy) <= (radius * radius);
}

int StudentWorld::intPow(int x) const
{
    return x * x;
}

//bool StudentWorld::checkIce(int x, int y, GraphObject::Direction dir)
//{
//    if (m_iceField[x][y] != nullptr) return true;
//    for (int i = 0; i < 3; i++) {
//        switch (dir)
//        {
//        case GraphObject::up:
//            if (m_iceField[x + i][y] != nullptr) return true;
//            break;
//        case GraphObject::down:
//            if (m_iceField[x + i][y] != nullptr) return true;
//            break;
//        case GraphObject::left:
//            if (m_iceField[x][y + i] != nullptr) return true;
//            break;
//        case GraphObject::right:
//            if (m_iceField[x][y + i] != nullptr) return true;
//            break;
//        }
//    }
//    return false;
//}

bool StudentWorld::checkIce(int x, int y) {
    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT) {
        return false; // Out of bounds check
    }
    return m_iceField[x][y] != nullptr;
}


bool StudentWorld::checkIceBoulder(int x, int y, GraphObject::Direction dir)
{
    if (!checkIce(x, y) && canActorMoveTo(m_iceman, x, y))
        return true;
    return false;
}

void StudentWorld::squirtWater(int x, int y, GraphObject::Direction dir) {
    playSound(SOUND_PLAYER_SQUIRT);
    m_iceman->addWater(-1); // Decrease water count

    switch (dir) {
    case Actor::up:
        if (!checkIceBoulder(x, y + 4, dir)) {
            addActor(new Squirt(this, x, y + 4, dir));
        }
        break;
    case Actor::down:
        if (!checkIceBoulder(x, y - 4, dir)) {
            addActor(new Squirt(this, x, y - 4, dir));
        }
        break;
    case Actor::right:
        if (!checkIceBoulder(x + 4, y, dir)) {
            addActor(new Squirt(this, x + 4, y, dir));
        }
        break;
    case Actor::left:
        if (!checkIceBoulder(x - 4, y, dir)) {
            addActor(new Squirt(this, x - 4, y, dir));
        }
        break;
    default:
        break;
    }
}
//
//bool StudentWorld::checkCollisionWithBoulder(Actor* a)
//{
//
//}