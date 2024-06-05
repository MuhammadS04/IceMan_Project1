#include "Actor.h"
#include "StudentWorld.h"


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
                // more stuff
                Squirt* s = new Squirt(getWorld(), getX(), getY(), getDirection());
                getWorld()->addActor(s);
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                m_water--;
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

//SQUIRT----------------------------------------------------------------
void Squirt::move()
{
    if (!isAlive()) {
        return;
    }

    // Check if Squirt annoys any nearby actors
    if (getWorld()->annoyAllNearbyActors(this, 2, 3)) {
        setDead();
        return;
    }

    // Check if Squirt has traveled its full distance
    if (m_travelDis == 0) {
        setDead();
        return;
    }

    // Check if Squirt can move one square in the current direction
    switch (getDirection()) {
    case up:
        if (!moveToIfPossible(getX(), getY() + 1)) {
            setDead();
            return;
        }
        break;
    case down:
        if (!moveToIfPossible(getX(), getY() - 1)) {
            setDead();
            return;
        }
        break;
    case left:
        if (!moveToIfPossible(getX() - 1, getY())) {
            setDead();
            return;
        }
        break;
    case right:
        if (!moveToIfPossible(getX() + 1, getY())) {
            setDead();
            return;
        }
        break;
    default:
        break;
    }

    m_travelDis--;
}


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
