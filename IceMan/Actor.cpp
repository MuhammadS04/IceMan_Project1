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
			setDead();
			break;
		case KEY_PRESS_DOWN:
                if (getDirection() == down)
                    moveToIfPossible(getX(), getY() - 1);
                else
                    setDirection(down);
                break;
			
		}
}

//Boulder-------------------------------------------------------------


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
