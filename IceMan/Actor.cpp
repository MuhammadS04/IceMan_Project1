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


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
