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
                    squirtY += 4;
                    break;
                case down:
                    squirtY -= 4;
                    break;
                case left:
                    squirtX -= 4;
                    break;
                case right:
                    squirtX += 4;
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

//SQUIRT----------------------------------------------------------------
void Squirt::move()
{
        if (!isAlive()) return;

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

        // Determine the next position based on the current direction
        int newX = getX();
        int newY = getY();
        switch (getDirection()) {
        case up:
            newY += 1;
            break;
        case down:
            newY -= 1;
            break;
        case left:
            newX -= 1;
            break;
        case right:
            newX += 1;
            break;
        default:
            break;
        }

        // Check if the Squirt can move to the new position
        if (!getWorld()->canActorMoveTo(this, newX, newY)) {
            setDead();
        }
        else {
            moveTo(newX, newY);
            m_travelDis--;
        }
}


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
