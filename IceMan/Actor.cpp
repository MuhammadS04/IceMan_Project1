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
// Actor.cpp

void Iceman::move()
{
    if (!isAlive()) return;

    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() == down)
                {
                    if (moveToIfPossible(getX(), getY() - 1)) // Check and clear ice after moving
                        getWorld()->clearIce(getX(), getY());
                }
                else
                    setDirection(down);
                break;
            case KEY_PRESS_LEFT:
                if (getDirection() == left)
                {
                    if (moveToIfPossible(getX() - 1, getY())) // Check and clear ice after moving
                        getWorld()->clearIce(getX(), getY());
                }
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() == right)
                {
                    if (moveToIfPossible(getX() + 1, getY())) // Check and clear ice after moving
                        getWorld()->clearIce(getX(), getY());
                }
                else
                    setDirection(right);
                break;
            case KEY_PRESS_UP:
                if (getDirection() == up)
                {
                    if (moveToIfPossible(getX(), getY() + 1)) // Check and clear ice after moving
                        getWorld()->clearIce(getX(), getY());
                }
                else
                    setDirection(up);
                break;
        }
    }
}


//Boulder-------------------------------------------------------------


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
