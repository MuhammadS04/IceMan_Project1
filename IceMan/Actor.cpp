#include "Actor.h"
#include "StudentWorld.h"
#include <queue>


bool Agent::annoy(unsigned int amount)
{
    if (m_hitPoints <= amount)
    {
        m_hitPoints = 0;
        setDead();
        return true;
    }
    else
    {
        m_hitPoints -= amount;
        return false;
    }
}

//ICEMAN__________________________________________________________
void Iceman::move()
{
    if (!isAlive()) return;


    int ch;
    if (getWorld()->getKey(ch))
        switch (ch)
        {
        case KEY_PRESS_ESCAPE:
            annoy(100);
            break;

        case KEY_PRESS_SPACE:
            if (getWater() > 0) {
                int squirtX = getX();
                int squirtY = getY();
                switch (getDirection()) {
                case up:
                    squirtY += 1;
                    break;
                case down:
                    squirtY -= 1;
                    break;
                case left:
                    squirtX -= 1;
                    break;
                case right:
                    squirtX += 1;
                    break;
                }
                if (getWorld()->canActorMoveTo(this, squirtX, squirtY)) {
                    Squirt* s = new Squirt(getWorld(), squirtX, squirtY, getDirection());
                    getWorld()->addActor(s);
                    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                    m_water--;
                }
            }
            break;
        case KEY_PRESS_DOWN:
            if (getDirection() == down)
                moveToIfPossible(getX(), getY() - 1);
            else
                setDirection(down);
            break;
        case KEY_PRESS_LEFT:
            if (getDirection() == left)
                moveToIfPossible(getX() - 1, getY());
            else
                setDirection(left);
            break;
        case KEY_PRESS_RIGHT:
            if (getDirection() == right)
            {
                moveToIfPossible(getX() + 1, getY());
            }
            else
                setDirection(right);
            break;
        case KEY_PRESS_UP:
            if (getDirection() == up)
                moveToIfPossible(getX(), getY() + 1);
            else
                setDirection(up);
            break;

        }

    getWorld()->clearIce(getX(), getY(), getDirection());

}

bool Iceman::annoy(int amount)
{
    if (Agent::annoy(amount)) {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
        return true;    // return true if IceMan is completely annoyed
    }
    return false;       // still got HP left
}



//Boulder-------------------------------------------------------------
void Boulder::move()
{
    if (!isAlive()) return;

    if (m_state == STATE_STABLE)
    {
        if (!isIceBelow())
        {
            m_state = STATE_WAITING;
            m_waitTicks = 30;
        }
    }
    else if (m_state == STATE_WAITING)
    {
        if (m_waitTicks > 0)
        {
            m_waitTicks--;
        }
        else
        {
            m_state = STATE_FALLING;
            getWorld()->playSound(SOUND_FALLING_ROCK);
        }

    }
    else if (m_state == STATE_FALLING)
    {
        //Doing check ice only for now, need to add check for other boulders too
        if (!isIceBelow())
        {
            moveTo(getX(), getY() - 1);
            // getWorld()->boulderProtestor(int x, int y);
             //^ this function should check if there is a protestor at that location and
             // annoy(100) if there is.
        }
        else
        {
            setDead();
        }

        /* if (canFallTo(getX(), getY() - 1))
         {
             moveTo(getX(), getY() - 1);
             getWorld()->checkr(this);
         }
         else
         {
             setDead();
         }*/
    }
}


bool Boulder::isIceBelow()
{
    int x = getX();
    int y = getY() - 1;

    for (int i = 0; i < 4; ++i)
    {
        if (getWorld()->checkIce(x + i, y))
        {
            return true;
        }
    }
    return false;
}

//bool Boulder::canFallTo(int x, int y) const
//{
//    if (y < 0 || getWorld()->checkIce(x, y)
//    {
//        return false;
//    }
//    return true;
//}



//SQUIRT----------------------------------------------------------------
void Squirt::move()
{
    if (!isAlive()) return;

    //Check if it hit protestors
    for (auto* protestor : getWorld()->getProtestors())
    {
        if (!protestor->isAlive() == false &&
            getWorld()->checkRadius(getX(), getY(), protestor->getX(), protestor->getY(), 3))
        {
            protestor->annoy(2);
            setDead();
            return;
        }

        int nextX = getX();
        int nextY = getY();

        switch (getDirection())
        {
        case left:  nextX--; break;
        case right: nextX++; break;
        case up:    nextY++; break;
        case down:  nextY--; break;
        default:    break;
        }

        //check if it hit ice or boulders
        if (getWorld()->checkIceBoulder(nextX, nextY, this->getDirection()))
        {
            setDead();
            return;
        }

        //int nextX = getX();
        //int nextY = getY();

        //switch (getDirection())
        //{
        //case left:  nextX--; break;
        //case right: nextX++; break;
        //case up:    nextY++; break;
        //case down:  nextY--; break;
        //default:    break;
        //}

        if (m_travelDis == 0)
        {
            setDead();
            return;
        }

        moveTo(nextX, nextY);
        m_travelDis--;
    }
}

void Protestor::findShortestPath(const int startX, const int startY, const int finalX, const int finalY) {
    std::queue<std::pair<int, int>> xy;
    int currStep = 0, currX, currY;

    stepArray[finalX][finalY] = 0; // Start from the target position
    xy.push(std::make_pair(finalX, finalY));

    while (!xy.empty()) {
        currX = xy.front().first;
        currY = xy.front().second;
        xy.pop();

        if (currX == startX && currY == startY) {
            didFindPath = true;
            step = stepArray[currX][currY];
            return;
        }

        currStep = stepArray[currX][currY];
        ++currStep;

        // Check all four directions
        std::vector<std::pair<int, int>> directions = {
            {currX, currY + 1}, // up
            {currX, currY - 1}, // down
            {currX - 1, currY}, // left
            {currX + 1, currY}  // right
        };

        for (const auto& dir : directions) {
            int nextX = dir.first;
            int nextY = dir.second;

            // Check bounds and if the cell is already visited
            if (nextX >= 0 && nextX < VIEW_WIDTH && nextY >= 0 && nextY < VIEW_HEIGHT
                && stepArray[nextX][nextY] == -1 && getWorld()->canActorMoveTo(this, nextX, nextY)) {
                stepArray[nextX][nextY] = currStep;
                xy.push(dir);
            }
        }
    }

    didFindPath = false; // If the path is not found
}

GraphObject::Direction Protestor::getDirectionToIceman() {
    int icemanX, icemanY;
    getWorld()->getIcemanPosition(icemanX, icemanY);

    // Clear the step array before performing a new search
    for (int i = 0; i < VIEW_WIDTH; ++i) {
        for (int j = 0; j < VIEW_HEIGHT; ++j) {
            stepArray[i][j] = -1;
        }
    }

    findShortestPath(getX(), getY(), icemanX, icemanY);

    // Determine the direction based on the shortest path
    if (didFindPath) {
        int currX = getX();
        int currY = getY();

        std::vector<std::pair<int, int>> directions = {
            {currX, currY + 1}, // up
            {currX, currY - 1}, // down
            {currX - 1, currY}, // left
            {currX + 1, currY}  // right
        };

        for (const auto& dir : directions) {
            int nextX = dir.first;
            int nextY = dir.second;

            if (nextX >= 0 && nextX < VIEW_WIDTH && nextY >= 0 && nextY < VIEW_HEIGHT
                && stepArray[nextX][nextY] == step - 1) {
                if (nextX == currX && nextY == currY + 1) return up;
                if (nextX == currX && nextY == currY - 1) return down;
                if (nextX == currX - 1 && nextY == currY) return left;
                if (nextX == currX + 1 && nextY == currY) return right;
            }
        }
    }

    return none; // If no path is found
}

void Protestor::move() {

    if (!isAlive()) return;

 

    // Determine direction to Iceman
    GraphObject::Direction dirToIceman = getDirectionToIceman();

    // Move the protestor based on the determined direction
    switch (dirToIceman) {
    case up:
        moveToIfPossible(getX(), getY() + 1);
        break;
    case down:
        moveToIfPossible(getX(), getY() - 1);
        break;
    case left:
        moveToIfPossible(getX() - 1, getY());
        break;
    case right:
        moveToIfPossible(getX() + 1, getY());
        break;
    default:
        break;
    }
}


// Add the method to check if a move is possible for the protestor
void Protestor::moveToIfPossible(int x, int y) {
    if (getWorld()->canActorMoveTo(this, x, y)) {
        moveTo(x, y);
    }
}

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
