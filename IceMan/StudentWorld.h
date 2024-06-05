#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
private:
	Iceman* m_iceman;
	Ice* m_iceField[64][64];
	vector<Actor*> actors;
	//vector<Protestor> protestors;

public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir){}

	virtual int init();

	virtual int move();

	virtual void cleanUp()
	{
	}

	void clearIce(int x, int y, int dir);

};

#endif // STUDENTWORLD_H_
