#include "Actor.h"
#include "StudentWorld.h"

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
			setDirection(down);
			break;
		}
}

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
